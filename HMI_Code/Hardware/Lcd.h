#ifndef TFT_H
#define TFT_H
#include "sys.h"
#define USE_HORIZONTAL 0 // 选择横竖屏模式

#if USE_HORIZONTAL == 0
#define LCD_W 240
#define LCD_H 280
#else
#define LCD_W 280
#define LCD_H 240
#endif
// 画笔颜色
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40
#define BRRED 0XFC07
#define GRAY 0X8430
#define DARKBLUE 0X01CF
#define LIGHTBLUE 0X7D7C
#define GRAYBLUE 0X5458
#define LIGHTGREEN 0X841F
#define LGRAY 0XC618
#define LGRAYBLUE 0XA651
#define LBBLUE 0X2B12
enum
{
	homePage = 0,
	// humTemPage,
	// setButtonMusicPage,
	versionPage,
};
// 1.初始化函数
void lcdInit(void);
void lcdSpiInit(void);
void lcdGpioInit(void);
void lcdWriteByte(u8 data);
void spiWriteByte(u8 data);
void lcdWriteDoubleByte(u16 doubleData);
void lcdWriteCommand(u8 command);
void lcdAddressSet(u16 x1, u16 y1, u16 x2, u16 y2);
void lcdDebugInfo(u8 errorCode);
// 2.功能函数
void lcdFillColor(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void lcdDrawPoint(u16 x, u16 y, u16 color);
void lcdDrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void lcdDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void lcdDrawCircle(u16 x0, u16 y0, u8 r, u16 color);
void lcdShowChar(u16 x, u16 y, u8 index, u16 fc, u16 bc, u8 sizey);
void lcdShowString(u16 x, u16 y, const char *s, u16 fc, u16 bc, u8 sizey);
void lcdShowChinese(u16 x, u16 y, const char *s, u16 fc, u16 bc, u8 size);
void lcdShowImage(u8 x, u8 y, u8 width, u8 height);
/**
 *application defined
 */
void lcdShowPage(rt_uint8_t pageNum, rt_uint8_t entrySet, rt_uint8_t entrySetData, rt_uint8_t flicker);
void lcdPlusMinusParameter(rt_uint8_t pageNum, rt_uint8_t type);
#endif
