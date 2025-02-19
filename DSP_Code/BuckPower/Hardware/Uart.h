/*
 * Uart.h
 *
 *  Created on: 2023��8��27��
 *      Author: MiniSforum
 */

#ifndef HARDWARE_UART_H_
#define HARDWARE_UART_H_
#include "DSP28x_Project.h"
void uartInit(Uint32 baud,Uint16 port);
void uartSendByte(Uchar8 data);
void uartSendString(Uchar8 *stringData);
Uchar8 uartReceiveByte(void);
#endif /* HARDWARE_UART_H_ */
