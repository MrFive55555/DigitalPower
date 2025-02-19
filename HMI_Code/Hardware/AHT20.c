#include "AHT20.h"
#include "IIC.h"
#include "Usart.h"
#define WRITE_ADDR 0x70
#define READ_ADDR 0x71
#define PRECESION 0x000FFFFF // 2^20
// measured data by send IIC protocol
static uint32_t humData;
static uint32_t tempData;
static uint8_t crcData;
static void measureData(void);
static void readData(void);

static void measureData(void)
{
	uint8_t ack;
	IICStart();
	IICWriteByte(WRITE_ADDR);
	ack = IICWaitACK();
	if (ack)
		printf("IIC comunication failed.\r\n");
	IICWriteByte(0xAC);
	ack = IICWaitACK();
	if (ack)
		printf("IIC comunication failed.\r\n");
	IICWriteByte(0x33);
	ack = IICWaitACK();
	if (ack)
		printf("IIC comunication failed.\r\n");
	IICWriteByte(0x00);
	ack = IICWaitACK();
	if (ack)
		printf("IIC comunication failed.\r\n");
	IICStop();
	humData = 0x00000000;
	tempData = 0x00000000;
	crcData = 0x00;
	rt_thread_mdelay(100); // at least wait 80 ms to finied measure.
}
// get data
static void readData(void)
{
	uint8_t mixData, ack;
	IICStart();
	IICWriteByte(READ_ADDR);
	ack = IICWaitACK();
	if (ack)
		printf("IIC comunication failed.\r\n");
	IICReadByte(0); // 0 is send ack to slave device
	// read humdiness data
	humData |= (IICReadByte(0) << 12); // 19 bit to 12 bit
	humData |= (IICReadByte(0) << 4);  // 11 bit t0 4 bit
	mixData = IICReadByte(0);
	humData |= (mixData >> 4); // 3 bit to 0 bit
	// read temperature data
	tempData |= ((mixData & 0x0F) << 16); // 19 bit to 16 bit
	tempData |= (IICReadByte(0) << 8);	  // 15 bit to 8 bit
	tempData |= IICReadByte(0);
	crcData |= IICReadByte(1);
	// printf("humData is %d,tempData is %d\r\n",humData,tempData);
}
// convert digital data to actual data
void getHumTempData(uint16_t *hum, uint16_t *tem)
{
	float fTem;
	measureData();
	readData();
	/**
	 *humdiness conversion: actual humdiness = humData/PRECESION * 100%
	 *temp conversion:actual temp = tempData/PRECESION *200 -50
	 */
	*hum = (humData * 100) >> 20;
	// tem = (humData * 200) >>20 - 50; //if use this convert way,it will loss more precision
	fTem = (float)(tempData * 200) / PRECESION - 50;
	*tem = fTem * 100; //convert ftem to int,save two decimals
	//*tem = fTemp * 100; //save two dot bits and convert uint type
	// printf("hum is %u,temp is %u\r\n",*hum,*tem);
}
