#include "Flash.h"
#include "Usart.h"
#include "Tool.h"
// IO of W25Q128 flash
#define FLASH_MISO PBin(10)
#define FLASH_MOSI PBout(11)
#define FLASH_SCL PBout(12)
#define FLASH_CS PBout(14)
#define WEL 0x06		   // write enable
#define STATUS_SR1 0x05	   // status of SR1
#define READ_DATA 0x03	   // read data
#define PAGE_WRITE 0x02	   // page write 256 bytes
#define SECTOR_ERASE 0x020 // erase sector = 16 page * 256 byte = 4Kbyte
#define ERASE_ALL 0xC7	   // erase all data
#define DEVICE_ID 0x9F	   // id of W25Q128
#define FLASH_ID 0xEF4018  // device ID
#define FLASH_NOBUSY 0x00  // No busy
static uint8_t SPIWriteReadByte(uint8_t dat);
static void flashWaitNoBusy(void);
static void flashEnableWrite(void);
static void flashWriteByte(uint32_t pageAddress, uint8_t *pBuf);
// intialize falsh
void flashInit(void)
{
	// 1.initialize IO port
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	FLASH_SCL = 0;
	FLASH_MOSI = 0;
	FLASH_CS = 1;
}
static uint8_t SPIWriteReadByte(uint8_t dat)
{
	uint8_t i, readData = 0x00;
	// FLASH_CS = 0;
	// rising edge to write
	for (i = 8; i > 0; i--)
	{
		FLASH_SCL = 0;
		rt_hw_us_delay(5);
		if (dat & (0x01 << (i - 1)))
			FLASH_MOSI = 1;
		else
			FLASH_MOSI = 0;
		rt_hw_us_delay(5);
		readData |= FLASH_MISO << (i - 1);
		FLASH_SCL = 1;
		rt_hw_us_delay(5);
	}
	FLASH_SCL = 0;
	// FLASH_CS = 1;
	/**
	 *Notice:don't enable CS in per WriteRead,make sure the CS until competed all steps.
	 *otherwise,the received data time sequence will be error.
	 */
	return readData;
}
// wait falsh free
static void flashWaitNoBusy(void)
{
	uint8_t SR1;
	FLASH_CS = 0;
	SPIWriteReadByte(STATUS_SR1);
	do
	{
		SR1 = SPIWriteReadByte(0xFF);
		// printf("ka si le..\r\n");
	} while ((SR1 & 0x01) != FLASH_NOBUSY); // wait falsh free,then can be write next
	FLASH_CS = 1;
}
static void flashEnableWrite(void)
{
	FLASH_CS = 0;
	SPIWriteReadByte(WEL); // must open write enable before write
	FLASH_CS = 1;
}
void flashEraseSector(uint32_t eraseSector)
{
	// the address of eraseSector must 4kb per.
	flashEnableWrite();
	flashWaitNoBusy();
	FLASH_CS = 0;
	SPIWriteReadByte(SECTOR_ERASE);
	SPIWriteReadByte((eraseSector & 0xFF0000) >> 16); // 24 bit of sector address
	SPIWriteReadByte((eraseSector & 0xFF00) >> 8);
	SPIWriteReadByte(eraseSector & 0xFF);
	FLASH_CS = 1;
	flashWaitNoBusy();
}
// read ID
uint32_t readFlashID(void)
{
	uint32_t id = 0x00000000;
	FLASH_CS = 0;
	SPIWriteReadByte(DEVICE_ID);
	id |= SPIWriteReadByte(0xFF) << 16;
	id |= SPIWriteReadByte(0xFF) << 8;
	id |= SPIWriteReadByte(0xFF);
	/**notice:cant use as follow ways,because write and read run on same time.
	 *if accord fllow ways,the read had occured in past.
	 */
	//	SPIWriteByte(DEVICE_ID);
	//	SPIReadByte();
	//	SPIReadByte();
	//	SPIReadByte();
	FLASH_CS = 1;
	return id;
}
// write data
static void flashWriteByte(uint32_t pageAddress, uint8_t *pBuf)
{
	/**
	*max writed count is 256 once write,
	if over 256,it will cover previous data.
	*/
	flashEnableWrite();
	// flashWaitNoBusy();
	FLASH_CS = 0;
	SPIWriteReadByte(PAGE_WRITE);
	SPIWriteReadByte((pageAddress & 0xFF0000) >> 16); // 24 bit of pageAddress
	SPIWriteReadByte((pageAddress & 0xFF00) >> 8);
	SPIWriteReadByte(pageAddress & 0xFF);
	while (*pBuf != '\0')
	{
		// printf("pBuf is %c\t",*pBuf);
		SPIWriteReadByte(*pBuf);
		pBuf++;
	}
	FLASH_CS = 1;
	flashWaitNoBusy();
}
// write data by full of page
void flashWriteBuffer(uint32_t writeAddress, uint8_t *pBuf)
{
	uint16_t buffSize = getStringLength(pBuf);
	uint16_t pageSize = buffSize / 256 + 1;
	while (pageSize--)
	{
		flashWriteByte(writeAddress, pBuf);
		writeAddress += 256;
		pBuf += 256;
	}
}
// read data
void flashReadBuffer(uint32_t pageAddress, uint8_t *pBuf, uint16_t readDataNum)
{
	FLASH_CS = 0;
	SPIWriteReadByte(READ_DATA);
	SPIWriteReadByte((pageAddress & 0xFF0000) >> 16); // 24 bit of readAddress
	SPIWriteReadByte((pageAddress & 0xFF00) >> 8);
	SPIWriteReadByte(pageAddress & 0xFF);
	while (readDataNum--)
		*pBuf++ = SPIWriteReadByte(0xFF);
	*pBuf = '\0';
	FLASH_CS = 1;
}
/**
 * application of flash
 */
/*defination*/
static void flashSaveButtonMusicParameter(void);
typedef void (*save_parameter)(void);
const save_parameter parameters[] = {
	flashSaveButtonMusicParameter,
};
#define PARAMETER_SIZE sizeof(parameters) / 4
extern rt_uint8_t musicSwitch;
/*function*/
void flashRecoverParameter(rt_uint8_t *pBuf)
{
	flashReadBuffer(0x7F0000, pBuf, PARAMETER_SIZE);
}
void flashSaveParmeter(rt_uint8_t saveParameter)
{
	parameters[saveParameter]();
}
static void flashSaveButtonMusicParameter(void)
{
	rt_uint32_t id;
	rt_uint8_t pBuf[2];
	id = readFlashID();
	if (id != FLASH_ID)
	{
		printf("device connect failed.please try again.\r\n");
	}
	else
	{
		if (musicSwitch)
		{
			pBuf[0] = '1';
		}
		else
		{
			pBuf[0] = '0';
		}
		pBuf[1] = '\0';
		flashEraseSector(0x7F0000);
		flashWriteBuffer(0x7F0000, pBuf);
		printf("save buttonMusicParamerter ok!\r\n");
	}
}
