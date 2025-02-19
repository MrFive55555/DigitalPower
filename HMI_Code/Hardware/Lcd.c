#include "Lcd.h"
#include "LcdFont.h"
#include "Usart.h"
#include "Tool.h"
#include "DMA.h"
#include "Flash.h"
#include "Buzzer.h"
/**
 * defined IO of LCD
 */
#define LCD_CS PAout(4)	 // CS
#define LCD_SCL PAout(5) // SCL = SCK
#define LCD_SDA PAout(7) // SDA = MOSI
#define LCD_RST PBout(7)
#define LCD_DC PBout(8)
#define LCD_BLK PBout(9)
#define SPIT_FLAG_TIMEOUT 65535

void lcdInit(void)
{
	lcdGpioInit();
	lcdSpiInit();
	LCD_RST = 0;
	rt_thread_mdelay(100);
	LCD_RST = 1;
	rt_thread_mdelay(100);
	LCD_BLK = 1;
	rt_thread_mdelay(100);
	// 3.Start Initial Sequence
	lcdWriteCommand(0x11); // Sleep out
	rt_thread_mdelay(120); // Delay 120ms
	lcdWriteCommand(0x36);
	if (USE_HORIZONTAL == 0)
		lcdWriteByte(0x00);
	else if (USE_HORIZONTAL == 1)
		lcdWriteByte(0xC0);
	else if (USE_HORIZONTAL == 2)
		lcdWriteByte(0x70);
	else
		lcdWriteByte(0xA0);

	lcdWriteCommand(0x3A);
	lcdWriteByte(0x05);

	lcdWriteCommand(0xB2);
	lcdWriteByte(0x0C);
	lcdWriteByte(0x0C);
	lcdWriteByte(0x00);
	lcdWriteByte(0x33);
	lcdWriteByte(0x33);

	lcdWriteCommand(0xB7);
	lcdWriteByte(0x35);

	lcdWriteCommand(0xBB);
	lcdWriteByte(0x32); // Vcom=1.35V

	lcdWriteCommand(0xC2);
	lcdWriteByte(0x01);

	lcdWriteCommand(0xC3);
	lcdWriteByte(0x15); // GVDD=4.8V  ??????

	lcdWriteCommand(0xC4);
	lcdWriteByte(0x20); // VDV, 0x20:0v

	lcdWriteCommand(0xC6);
	lcdWriteByte(0x0F); // 0x0F:60Hz

	lcdWriteCommand(0xD0);
	lcdWriteByte(0xA4);
	lcdWriteByte(0xA1);

	lcdWriteCommand(0xE0);
	lcdWriteByte(0xD0);
	lcdWriteByte(0x08);
	lcdWriteByte(0x0E);
	lcdWriteByte(0x09);
	lcdWriteByte(0x09);
	lcdWriteByte(0x05);
	lcdWriteByte(0x31);
	lcdWriteByte(0x33);
	lcdWriteByte(0x48);
	lcdWriteByte(0x17);
	lcdWriteByte(0x14);
	lcdWriteByte(0x15);
	lcdWriteByte(0x31);
	lcdWriteByte(0x34);

	lcdWriteCommand(0xE1);
	lcdWriteByte(0xD0);
	lcdWriteByte(0x08);
	lcdWriteByte(0x0E);
	lcdWriteByte(0x09);
	lcdWriteByte(0x09);
	lcdWriteByte(0x15);
	lcdWriteByte(0x31);
	lcdWriteByte(0x33);
	lcdWriteByte(0x48);
	lcdWriteByte(0x17);
	lcdWriteByte(0x14);
	lcdWriteByte(0x15);
	lcdWriteByte(0x31);
	lcdWriteByte(0x34);
	lcdWriteCommand(0x21);

	lcdWriteCommand(0x29);
}
// ???SPI?????????
void lcdSpiInit(void)
{
	// SPI2 ??????????? PB12_13_14_15 CS SCK MISO MOSI
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	// ???SPI2 GPIOB ???????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	/* 1.???? SPI ?? CS ???????? IO ???? ????????*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// CS????? ???????��????????
	LCD_CS = 1;
	/* ???? SPI ?? SCK ???? MOSI???? */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// SCK(SCL)???????? ???????????????
	/* 2.SPI ?????? ST7735S ???????P44 */
	// FLASH ��? ??? SPI ?? 0 ???? 3,??????? CPOL CPHA
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // ??0
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; // ???????72M/2 = 36M
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	/* ??? SPI */
	SPI_Cmd(SPI1, ENABLE);
}
void lcdGpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// ???GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	LCD_RST = 0; // ??��???? ???????????��????????
	LCD_DC = 1;	 // ???��????????
	LCD_BLK = 0; // ??????
}
void spiWriteByte(u8 data)
{
	u32 spiTimeOut;
	spiTimeOut = SPIT_FLAG_TIMEOUT;
	/* ????????????????TXE ??? */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		// ?????????
		if ((spiTimeOut--) == 0)
		{
			lcdDebugInfo(0);
			return;
		}
	}
	/* ��???????????????��???????��????????? */
	SPI_I2S_SendData(SPI1, data);
	/*spiTimeOut = SPIT_FLAG_TIMEOUT;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		// ?????????
		if ((spiTimeOut--) == 0)
		{
			lcdDebugInfo(1);
			return;
		}
	}*/
}
void lcdWriteByte(u8 data)
{
	LCD_CS = 0; //?????? (??)
	spiWriteByte(data);
	LCD_CS = 1; //?????
}
// ��??????????
void lcdWriteDoubleByte(u16 doubleData)
{
	lcdWriteByte(doubleData >> 8);
	lcdWriteByte(doubleData); // ??????8��
}
// ��?????
void lcdWriteCommand(u8 command)
{
	LCD_DC = 0; // ???????��?????
	lcdWriteByte(command);
	LCD_DC = 1;
}
// ???????
void lcdAddressSet(u16 x1, u16 y1, u16 x2, u16 y2)
{
	if (USE_HORIZONTAL == 0)
	{
		lcdWriteCommand(0x2a); // ?��??
		lcdWriteDoubleByte(x1);
		lcdWriteDoubleByte(x2);
		lcdWriteCommand(0x2b); // ?��??
		lcdWriteDoubleByte(y1 + 20);
		lcdWriteDoubleByte(y2 + 20);
		lcdWriteCommand(0x2c); // ?��??��
	}
	else if (USE_HORIZONTAL == 1)
	{
		lcdWriteCommand(0x2a); // ?��??
		lcdWriteDoubleByte(x1);
		lcdWriteDoubleByte(x2);
		lcdWriteCommand(0x2b); // ?��??
		lcdWriteDoubleByte(y1 + 20);
		lcdWriteDoubleByte(y2 + 20);
		lcdWriteCommand(0x2c); // ?��??��
	}
	else if (USE_HORIZONTAL == 2)
	{
		lcdWriteCommand(0x2a); // ?��??
		lcdWriteDoubleByte(x1 + 1);
		lcdWriteDoubleByte(x2 + 1);
		lcdWriteCommand(0x2b); // ?��??
		lcdWriteDoubleByte(y1 + 2);
		lcdWriteDoubleByte(y2 + 2);
		lcdWriteCommand(0x2c); // ?��??��
	}
	else
	{
		lcdWriteCommand(0x2a); // ?��??
		lcdWriteDoubleByte(x1 + 1);
		lcdWriteDoubleByte(x2 + 1);
		lcdWriteCommand(0x2b); // ?��??
		lcdWriteDoubleByte(y1 + 2);
		lcdWriteDoubleByte(y2 + 2);
		lcdWriteCommand(0x2c); // ?��??��
	}
}
// ??????????
void lcdDebugInfo(u8 errorCode)
{
	switch (errorCode)
	{
	case 0:
		printf("SPI wirtes bytes timeout!\n");
		break;
	case 1:
		printf("SPI recieves bytes timeout!\n");
		break;
	}
}
/******     2.LCD????             ****/
// ?????????????
void lcdFillColor(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	//	u16 i, j;
	//	lcdAddressSet(x1, y1, x2, y2);
	//	// ?????????
	//	for (i = y1; i < y2; i++)
	//	{
	//		for (j = x1; j < x2; j++)
	//		{
	//			lcdWriteDoubleByte(color);
	//		}
	//	}
	/**
	 *Question:SPI->DR value is different with DMA memory address value.
	 *have no resolution to slove this problem.
	 *solution:because DMA maximum transmitter data is 65535,so when transmitted data is more than the value,
	 *it will work abnormal,so need segment several data to transimt.
	 */
	rt_uint32_t bufferSize = (x2 - x1) * (y2 - y1);
	rt_uint8_t transCount = bufferSize / 65535 + 1;
	lcdAddressSet(x1, y1, x2 - 1, y2 - 1);
	/**
	 * because color code is half word,so need change data size to 16 bit
	 */
	SPI_Cmd(SPI1, DISABLE);
	SPI1->CR1 |= (1 << 11); // need first disable,then change
	SPI_Cmd(SPI1, ENABLE);
	LCD_CS = 0;
	DMA_Config(DMA1_Channel3, (u32)&color, (u32)&SPI1->DR, 16); // 16 is half word
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
	while (transCount)
	{
		if (transCount == 1)
		{
			DMANormalEnable(DMA1_Channel3, bufferSize);
		}
		else
		{
			DMANormalEnable(DMA1_Channel3, 65535);
			bufferSize -= 65535;
		}
		while (1)
		{
			if (DMA_GetFlagStatus(DMA1_FLAG_TC3) != RESET) // wait channel3 tranmit ok
			{
				DMA_ClearFlag(DMA1_FLAG_TC3); // clear ok flag bit
				break;
			}
		}
		while (SPI1->SR & 0x0080)
			; // wait SPI data transmit ok
		transCount--;
	}

	LCD_CS = 1;
	// send ok then return byte mode
	SPI_Cmd(SPI1, DISABLE);
	SPI1->CR1 &= ~(1 << 11);
	SPI_Cmd(SPI1, ENABLE);
}

// ????????
void lcdDrawPoint(u16 x, u16 y, u16 color)
{
	lcdAddressSet(x, y, x, y);
	lcdWriteDoubleByte(color);
}
// ????
void lcdDrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	u16 t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; //????????????
	delta_y = y2 - y1;
	uRow = x1; //???????????
	uCol = y1;
	if (delta_x > 0)
		incx = 1; //???????
	else if (delta_x == 0)
		incx = 0; //?????
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; //????
	else
	{
		incy = -1;
		delta_y = -delta_y;
	} //????????????????
	if (delta_x > delta_y)
		distance = delta_x;
	else
		distance = delta_y;
	for (t = 0; t < distance + 1; t++)
	{
		lcdDrawPoint(uRow, uCol, color); //????
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}
//??????
void lcdDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	lcdDrawLine(x1, y1, x2, y1, color);
	lcdDrawLine(x1, y2, x2, y2, color);
	lcdDrawLine(x1, y1, x1, y2, color);
	lcdDrawLine(x2, y1, x2, y1, color);
}
//???
void lcdDrawCircle(u16 x0, u16 y0, u8 r, u16 color)
{
	int a, b;
	a = 0;
	b = r;
	while (a <= b)
	{
		lcdDrawPoint(x0 - b, y0 - a, color); // 3
		lcdDrawPoint(x0 + b, y0 - a, color); // 0
		lcdDrawPoint(x0 - a, y0 + b, color); // 1
		lcdDrawPoint(x0 - a, y0 - b, color); // 2
		lcdDrawPoint(x0 + b, y0 + a, color); // 4
		lcdDrawPoint(x0 + a, y0 - b, color); // 5
		lcdDrawPoint(x0 + a, y0 + b, color); // 6
		lcdDrawPoint(x0 - b, y0 + a, color); // 7
		a++;
		if ((a * a + b * b) > (r * r)) //?��????????????
		{
			b--;
		}
	}
}
//??????
void lcdShowChar(u16 x, u16 y, u8 index, u16 fc, u16 bc, u8 sizey)
{
	//?????????????????????,????????????fc,????????????????
	u8 sizex, fontHeight, word;
	u8 i, j;
	sizex = sizey / 2;										//????x????��
	fontHeight = (sizex / 8 + (sizex % 8 ? 1 : 0)) * sizey; //????????��????
	index = index - ' ';									//????????????��?
	lcdAddressSet(x, y, x + sizex - 1, y + sizey - 1);		//????��????????
	for (i = 0; i < fontHeight; i++)
	{
		switch (sizey)
		{
		case 16:
			word = ascii_1608[index][i];
			break;
		case 32:
			word = ascii_3216[index][i];
			break;
		default:
			return;
		}
		for (j = 0; j < 8; j++)
		{
			//?��?????????????????????????
			if (word & (0x01 << j))
				lcdWriteDoubleByte(fc); //????????LCD????
			else
				lcdWriteDoubleByte(bc);
		}
	}
}

//????????
void lcdShowString(u16 x, u16 y, const char *s, u16 fc, u16 bc, u8 sizey)
{
	char temp[3];
	rt_uint8_t i;
	temp[2] = '\0';
	while (*s != '\0')
	{
		//???????0?????? ???????
		if (*s > 0)
		{
			lcdShowChar(x, y, *s++, fc, bc, sizey);
			x += sizey / 2; //????????��??
		}
		else
		{
			//???????????????????? ????��?????????????????
			for (i = 0; i < 2; i++)
			{
				temp[i] = *s++;
			}
			lcdShowChinese(x, y, temp, fc, bc, sizey);
			x += sizey;
		}
	}
}
/**
*????��??index???????????????��?
1.start -> length
2.?????????????? x??y??????????

????��?????????????????????????????????????????????????????
?????????????????????????????��???????????????????????????????????????????????��??
*/
//???????
/**
 *???????????????????????
 *?????��??????
 */
void lcdShowChinese(u16 x, u16 y, const char *s, u16 fc, u16 bc, u8 size)
{
	//????��????:n+ n*n = n(1+n)
	u8 i, j, k, word, keepIndex = 0, index = 0, pass = 0;
	for (i = 0; s[i] != '\0';)
	{
		//??????????  ????????????????????
		if (-s[i] == chinese16[index][i % 2 + 32])
			pass++; // i%2+32?????��?
		i++;
		if (i % 2 == 0)
		{ //?????????????????
			//???????????????????
			if (pass == 2)
			{
				lcdAddressSet(x, y, x + size - 1, y + size - 1); //??????????????????(size?16 ?????)
				//????? ??��??? ????(??????��)
				for (j = 0; j < size * 2; j++)
				{
					switch (size)
					{ //???????
					case 16:
						word = chinese16[index][j];
						break;
					default:
						return;
					}
					for (k = 0; k < 8; k++) //??????
					{
						//?��?????????????????????????
						if (word & (0x01 << k))
							lcdWriteDoubleByte(fc); //????????LCD????
						else
							lcdWriteDoubleByte(bc); //??????????
					}
				}
				// x+=size; ?????showString?? ???????????
				keepIndex = i; //???????????????��??
				index = 0;	   //???????????????????��??��??
			}
			else
			{
				index++; //?��???????????
				if (index > MAX_CHINESE_INDEX)
					break; //??????????�Y?????????????? ????????
			}
			i = keepIndex;
			pass = 0;
		}
	}
}
//?????
#if USE_IMAGE
void lcdShowImage(u8 x, u8 y, u8 width, u8 height)
{
	const unsigned char *p = gImage_nvdi;
	int i, j, k;
	unsigned char picH, picL;
	//???3*3 = 9 9????
	for (k = 0; k < 1; k++)
	{
		for (j = 0; j < 1; j++)
		{
			lcdAddressSet(x, y, x + width - 1, y + height - 1); //????????
			for (i = 0; i < width * height; i++)
			{
				picL = *(p + i * 2); //?????��???
				picH = *(p + i * 2 + 1);
				lcdWriteDoubleByte(picH << 8 | picL);
			}
		}
	}
}
#endif

/**
 *defined page num
 */
typedef void (*show_page)(rt_uint8_t pageNum);
static void lcdShowHomePage(rt_uint8_t pageNum);
// static void lcdShowHumTemPage(rt_uint8_t pageNum);
// static void lcdSetButtonMusicPage(rt_uint8_t pageNum);
static void lcdShowVersion(rt_uint8_t prePageNum);
static void clearPartScreen(rt_uint8_t entrySet);
static void lcdShowSettingPage(rt_uint8_t itemID, rt_uint8_t entrySetData, rt_uint8_t flicker);
static void lcdConvertToString(rt_uint8_t pageNum);
const show_page pages[] = {
	lcdShowHomePage,
	// lcdShowHumTemPage,
	// lcdSetButtonMusicPage,
	lcdShowVersion,
};
/**
 * user setting parameters
 */
const rt_uint8_t *setting_item_string[2][3] = {
	{"01", "02", "03"},
	{"VOP", "BUZ", "MUS"}};
static rt_int8_t prePageNum = -1;
// because function occupy 4 bytes
rt_uint8_t pagination = (sizeof(pages) / 4);
rt_uint8_t setting_item_num = (sizeof(*setting_item_string) / 4);
enum
{
	VOP = 0,
	BUZ,
	MUS,
};
static rt_uint8_t parameterString[4];
/**
 *lcd parameter
 */
rt_uint16_t hum, tem;
static rt_uint16_t version = 10;
rt_uint8_t musicSwitch = RT_TRUE;
/**
 * rt parameter
 */
// extern rt_mutex_t hum_tem_mutex;
/**
 * event
 */
extern rt_event_t measure_event;
/**
 * show current page
 */
void lcdShowPage(rt_uint8_t pageNum, rt_uint8_t entrySet, rt_uint8_t entrySetData, rt_uint8_t flicker)
{
	static rt_uint8_t preEntrySet = 0;
	if (preEntrySet != entrySet)
	{
		preEntrySet = entrySet;
		if (pageNum <= 0)
			prePageNum = -1; // fresh first page when change menu
		lcdFillColor(0, 0, LCD_W, LCD_H, LIGHTGREEN);
	}

	if (pageNum != prePageNum)
	{
		clearPartScreen(entrySet);
	}

	/**
	 *according entrySet to entry different menu
	 **/
	if (entrySet)
	{
		lcdShowSettingPage(pageNum, entrySetData, flicker);
	}
	else
	{
		pages[pageNum](pageNum);
	}
}
/**
 * application of LCD *****
 */
/**
 * part section refresh to accelerate refresh speed.
 */
static void clearPartScreen(rt_uint8_t entrySet)
{
	rt_uint8_t pBuf[2];
	if (0)
	{
		//		switch (prePageNum)
		//		{
		//		case /* constant-expression */:
		//			/* code */
		//			break;

		//		default:
		//			break;
		//		}
	}
	else
	{
		switch (prePageNum)
		{
		case homePage:
			lcdFillColor(40, 20, 200, 52, LIGHTGREEN);
			lcdFillColor(90, 52, 234, 84, LIGHTGREEN);
			lcdFillColor(10, 90, 202, 108, LIGHTGREEN);
			lcdFillColor(10, 130, 210, 52, LIGHTGREEN);
			lcdFillColor(10, 170, 210, 52, LIGHTGREEN);
			lcdFillColor(10, 210, 58, 52, LIGHTGREEN);
			lcdFillColor(10, 250, 66, 266, LIGHTGREEN);
			lcdFillColor(106, 250, 186, 266, LIGHTGREEN);
			break;
		// case humTemPage:
		// 	lcdFillColor(20, 24, 108, 41, LIGHTGREEN);
		// 	lcdFillColor(20, 44, 132, 61, LIGHTGREEN);
		// 	break;
		// case setButtonMusicPage:
		// 	lcdFillColor(10, 104, 170, 120, LIGHTGREEN);
		// 	break;
		case versionPage:
			lcdFillColor(10, 120, 162, 137, LIGHTGREEN);
			break;
		default:
			flashRecoverParameter(pBuf);
			if (pBuf[0] == '0')
			{
				// buttonMusic(RT_FALSE);
				musicSwitch = RT_FALSE;
			}
			else
			{
				// buttonMusic(RT_TRUE);
				musicSwitch = RT_TRUE;
			}
			lcdFillColor(0, 0, LCD_W, LCD_H, LIGHTGREEN);
			break;
		}
	}
}
static void lcdShowHomePage(rt_uint8_t pageNum)
{
	/**
	 * if page not change,it don't need reflash.
	 */
	rt_uint8_t dataString[10];
	if (pageNum != prePageNum)
	{
		lcdShowString(40, 20, "Buck Power", YELLOW, GRAYBLUE, 32);
		lcdShowString(90, 52, "--Mr.Five", YELLOW, GRAYBLUE, 32);
		lcdShowString(10, 90, "Input Voltage:", BLUE, GREEN, 16);
		lcdShowString(10, 130, "Output Voltage:", BLUE, GREEN, 16);
		lcdShowString(10, 170, "Output Current:", BLUE, GREEN, 16);
		lcdShowString(10, 210, "Power:", BLUE, GREEN, 16);
		lcdShowString(10, 250, "Hum:", BLUE, GREEN, 16);
		lcdShowChar(58, 250, '%', BLUE, GREEN, 16);
		lcdShowString(106, 250, "Tem:", BLUE, GREEN, 16);
		lcdShowChar(178, 250, 'C', BLUE, GREEN, 16);
		prePageNum = pageNum;
	}
	if(flashFlag){
		numToStr(elecData.inputVol,2,2,dataString,STRING_TYPE);
		lcdShowString(122, 90, dataString, BLUE, GREEN, 16);
		numToStr(elecData.outputVol,2,2,dataString,STRING_TYPE);
		lcdShowString(130, 130, dataString, BLUE, GREEN, 16);
		numToStr(elecData.current,4,0,dataString,STRING_TYPE);
		lcdShowString(130, 170, dataString, BLUE, GREEN, 16);
		flashFlag = 0;
		// numToStr(elecData.outputVol,,2,&dataString,STRING_TYPE);
		// lcdShowString(122, 90, dataString, BLUE, GREEN, 16);
	}
	numToStr(hum, 2, 0, &dataString[0], STRING_TYPE);
	lcdShowString(42, 250, dataString, BLUE, GREEN, 16);
	numToStr(tem, 2, 2, &dataString[0], STRING_TYPE);
	lcdShowString(138, 250, dataString, BLUE, GREEN, 16);
	rt_event_send(measure_event, MEASURE_HUM_TEM_EVENT_FLAG);
}
static void lcdShowSettingPage(rt_uint8_t pageNum, rt_uint8_t entrySetData, rt_uint8_t flicker)
{
	static rt_uint8_t preEntrySetData;
	if (pageNum != prePageNum) 
	{
		lcdConvertToString(pageNum);
		lcdShowString(73, 54, "ID", BLUE, GREEN, 32);
		lcdShowString(105, 54, setting_item_string[0][pageNum], BLUE, GREEN, 32); // ID number
		lcdShowString(40, 180, setting_item_string[1][pageNum], BLUE, GREEN, 32); // ID name
		lcdShowString(128, 180, parameterString, BLUE, GREEN, 32);				  // ID parameter
		prePageNum = pageNum;
	}
	// avoid last value in flicker mode
	if (entrySetData != preEntrySetData && preEntrySetData != 1)
	{
		preEntrySetData = entrySetData;
		lcdShowString(105, 54, setting_item_string[0][pageNum], BLUE, GREEN, 32);
	}
	else if (entrySetData != preEntrySetData && preEntrySetData)
	{
		preEntrySetData = entrySetData;
		lcdShowString(128, 180, parameterString, BLUE, GREEN, 32);
	}

	if (entrySetData != 1) // flicker setting ID
	{
		if (flicker)
		{
			lcdFillColor(105, 54, 137, 86, LIGHTGREEN);
		}
		else
		{
			lcdShowString(105, 54, setting_item_string[0][pageNum], BLUE, GREEN, 32);
		}
	}
	else // flicker parameter
	{
		if (flicker)
		{
			lcdFillColor(128, 180, 160, 212, LIGHTGREEN);
		}
		else
		{
			lcdShowString(128, 180, parameterString, BLUE, GREEN, 32);
		}
	}
}
// static void lcdShowHumTemPage(rt_uint8_t pageNum)
// {
// 	/* rt_uint8_t dataString[10]; // support max num is 8 place
// 	if (pageNum != prePageNum)
// 	{
// 		lcdShowString(20, 24, "Hum:", GREEN, BLUE, 16);
// 		lcdShowString(20, 44, "Tem:", GREEN, BLUE, 16);
// 		lcdShowChar(100, 24, '%', GREEN, BLUE, 16);
// 		lcdShowChar(124, 44, 'C', GREEN, BLUE, 16);
// 		prePageNum = pageNum;
// 	}
// 	//	err = rt_mutex_take(hum_tem_mutex, RT_WAITING_FOREVER);
// 	//	if (err == RT_EOK)
// 	//	{
// 	//		numToStr(hum, 2, 0, &dataString[0]);
// 	//		lcdShowString(84, 24, dataString, GREEN, BLUE, 16);
// 	//		numToStr(tem, 2, 2, &dataString[0]);
// 	//		lcdShowString(84, 44, dataString, GREEN, BLUE, 16);
// 	//		// need release mutex agin
// 	//		rt_mutex_release(hum_tem_mutex);
// 	//	}
// 	numToStr(hum, 2, 0, &dataString[0]);
// 	lcdShowString(84, 24, dataString, GREEN, BLUE, 16);
// 	numToStr(tem, 2, 2, &dataString[0]);
// 	lcdShowString(84, 44, dataString, GREEN, BLUE, 16);
// 	rt_event_send(measure_event, MEASURE_HUM_TEM_EVENT_FLAG); */
// }
// static void lcdSetButtonMusicPage(rt_uint8_t pageNum)
// {
// 	if (pageNum != prePageNum)
// 	{
// 		lcdShowString(10, 104, "Music Enable:", GREEN, BLUE, 16);
// 		prePageNum = pageNum;
// 	}
// 	if (musicSwitch)
// 	{
// 		lcdShowString(114, 104, "enable", GREEN, BLUE, 16);
// 	}
// 	else
// 	{
// 		lcdShowString(114, 104, "disable", GREEN, BLUE, 16);
// 	}
// }
static void lcdShowVersion(rt_uint8_t pageNum)
{
	if (pageNum != prePageNum)
	{
		rt_uint8_t dataString[4];
		lcdShowString(10, 120, "Current Version:", GREEN, BLUE, 16);
		numToStr(version, 1, 1, dataString, STRING_TYPE);
		lcdShowString(138, 120, dataString, GREEN, BLUE, 16);
		prePageNum = pageNum;
	}
}

void lcdPlusMinusParameter(rt_uint8_t pageNum, rt_uint8_t type)
{
	if (type == PLUS)
	{
		switch (pageNum)
		{
		case VOP: // 3-10
			uiData.outputVol++;
			if (uiData.outputVol > 10)
				uiData.outputVol = 3;
			break;
		case BUZ: // 0-1
			uiData.buzzEnable++;
			uiData.buzzEnable %= 2;
			break;
		case MUS:
			uiData.musicSwitch++;
			uiData.musicSwitch %= (MAX_MUS + 1);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (pageNum)
		{
		case VOP: // 3-10
			if (uiData.outputVol <= 3)
				uiData.outputVol = MAX_VOUT + 1;
			uiData.outputVol--;
			break;
		case BUZ: // 0-1
			if (uiData.buzzEnable <= 0)
				uiData.buzzEnable = 2;
			uiData.buzzEnable--;
			break;
		case MUS: // 0-3
			if (uiData.musicSwitch <= 0)
				uiData.musicSwitch = MAX_MUS + 1;
			uiData.musicSwitch--;
			break;
		default:
			break;
		}
	}
	lcdConvertToString(pageNum);
}
// convert settting parameter to string
static void lcdConvertToString(rt_uint8_t pageNum)
{
	switch (pageNum)
	{
	case VOP:
		numToStr(uiData.outputVol, 2, 0, parameterString, STRING_TYPE);
		break;
	case BUZ:
		/**
		 * quesiton:pBuf = "EN",it don't execute.
		 * because "EN" is const variable and had assigned address,so use pBuf = "EN",
		 * which just modify pBuf address,rather than modify array of char.
		 *
		 */
		if (uiData.buzzEnable)
		{
			parameterString[0] = 'E';
			parameterString[1] = 'N';
			parameterString[2] = '\0';
		}
		else
		{
			parameterString[0] = 'D';
			parameterString[1] = 'N';
			parameterString[2] = '\0';
		}
		break;
	case MUS:
		numToStr(uiData.musicSwitch, 2, 0, parameterString, STRING_TYPE);
		break;
	default:
		break;
	}
}
