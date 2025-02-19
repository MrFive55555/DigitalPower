/*
 * IO.h
 *
 *  Created on: 2023Äê8ÔÂ5ÈÕ
 *      Author: MiniSforum
 */

#ifndef HARDWARE_IO_H_
#define HARDWARE_IO_H_
#include "DSP28x_Project.h"
/**
 * Global variables
 * use defined GLOBAL_VARIABLES in var.h make sure only defined once.
 * but have a condition that variables can't be initialized values.
 */
#ifdef OS_GLOBALS
#define OS_EXT
#else
#define OS_EXT extern
#endif
/**
 * variable typedef name
 */
typedef enum{
    false = 0,
    true = !false
}bool;
enum{
	PORTA = 0,
	PORTB,
};
/**
 * Usart parameters
 */
#define CHAR_BLANK 32
#define CHAR_ENTER 10
#define BUFFER_LEN 256
#define STRING_TYPE 1
#define CHAR_ARRAY_TYPE 0
OS_EXT Uchar8 receiveBuffer[BUFFER_LEN];
OS_EXT Uchar8 receiveOKFlag;
OS_EXT Uchar8 sendBuffer[BUFFER_LEN];

/**
 *User setting parameters
 */
typedef struct
{
    Uchar8 outputVol;
    Uchar8 buzzEnable;
    Uchar8 musicSwitch;
}SetData;
typedef struct 
{
    Uint32 outputVol;
    Uint32 inputVol;
    Uint32 tempVol;
    int32  current;
}ElecData;
OS_EXT ElecData elecCalData;
OS_EXT ElecData elecData;
OS_EXT SetData useData;
OS_EXT int32 power;
#define PARAMETER_NUM sizeof(SetData)
#define POWER_ON 0
#define POWER_OFF 1
#define POWER_SETTING 2
#define POWER_UPDATE 3
/**
 * function declare
 */
void initGlobalVar(void);

#endif /* HARDWARE_IO_H_ */
