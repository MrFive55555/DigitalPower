#ifndef CAN_H
#define CAN_H
#include "sys.h"
void CANInit(void);
void CAN_SendMsg(char *stringData);
void USB_LP_CAN1_RX0_IRQHandler(void);
#endif
