/*
 * ADC.h
 *
 *  Created on: 2023Äê8ÔÂ22ÈÕ
 *      Author: MiniSforum
 */

#ifndef HARDWARE_ADC_H_
#define HARDWARE_ADC_H_
#include "DSP28x_Project.h"
void ADCInit(void);
Uint16 ADCReadValue(void);
Uint16 ADCConvertCalData(void);

#endif /* HARDWARE_ADC_H_ */
