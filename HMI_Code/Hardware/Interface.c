#include "Interface.h"
#include "var.h"
#include "Tool.h"
#include "Usart.h"
#include "AHT20.h"
/**
 * get version of dsp and stm
*/
/* void sSECPVersion(void)
{
	uint8_t *TxBufferAddress = &TxBuffer[0];
    numToStr(123, 3, 0, &TxBufferAddress);
    *TxBufferAddress++ = 0x20; // space character
    numToStr(456, 1, 2, &TxBufferAddress);
	*TxBufferAddress++ = 0x0D; // enter character
    usartSendString(USART1,&TxBuffer[0]);
} */
// void sSECPHumTemp(void){
// 	//uint8_t *TxBufferAddress = &TxBuffer[0];
// 	uint16_t humData;
// 	uint16_t tempData;
//     getHumTempData(&humData,&tempData);
//    // numToStr(humData, 2, 0, &TxBufferAddress);
//    // *TxBufferAddress++ = 0x20; // space character
//    // numToStr(tempData, 2, 2, &TxBufferAddress);
//    // *TxBufferAddress++ = 0x0D; // enter character
//    // usartSendString(USART1,&TxBuffer[0]);
//     printf("hum is %d%%,temp is %.2fC\r\n",humData,tempData);
// }
