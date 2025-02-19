#ifndef BUZZER_H
#define BUZZER_H
#include "sys.h"
void buzzerInit(uint16_t period,uint16_t pulse);
void playMusic(void);
void buttonMusic(rt_bool_t musicEnable);
#endif

