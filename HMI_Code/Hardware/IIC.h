#ifndef IIC_H
#define IIC_H
#include "sys.h"
void IICInit(void);
void IICStart(void);
void IICStop(void);
uint8_t IICWaitACK(void);
void IICACK(void);
void IICNOACK(void);
void IICWriteByte(uint8_t dat);
uint8_t IICReadByte(uint8_t ack);
#endif
