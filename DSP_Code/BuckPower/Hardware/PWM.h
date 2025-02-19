/*
 * PWM.h
 *
 *  Created on: 2023��8��14��
 *      Author: MiniSforum
 */

#ifndef HARDWARE_PWM_H_
#define HARDWARE_PWM_H_
#include "DSP28x_Project.h"
typedef enum{
    EPWMA,
    EPWMB
}enum_EPWM;
void pwmInit(Uint16 tbprd);
void adjustVoltage(Uchar8 status);
//void EPWM6_INT_IRQn(void);
//void EPWM6_TZINT_IRQn(void);

#endif /* HARDWARE_PWM_H_ */
