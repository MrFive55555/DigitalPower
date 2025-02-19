#ifndef BUTTON_H
#define BUTTON_H
#include "sys.h"
#define KEY_NO 0
#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_RETURN 3
#define KEY_CONFIRM 4
void buttonInit(void);
uint8_t scanButton(uint8_t longPressEnable);
#endif
