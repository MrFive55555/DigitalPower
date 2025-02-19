#ifndef VAR_H
#define VAR_H
#include "sys.h"

#ifdef GLOBAL_VAR
#define EXT_VAR
#else
#define EXT_VAR extern
#endif

/**
 * ui parameters
*/
typedef struct 
{
    rt_uint8_t outputVol;
    rt_uint8_t buzzEnable;
    rt_uint8_t musicSwitch;
}SetData;
typedef struct 
{
    rt_uint16_t outputVol;
    rt_uint16_t inputVol;
    rt_uint16_t tempVol;
    rt_uint16_t current;
}ElecData;
#define PLUS 1
#define MINUS 0
#define MAX_VOUT 10
#define MAX_MUS 3
#define SET_DATA_LENGTH sizeof(SetData)
#define ELEC_DATA_LENGTH (sizeof(ElecData) / 2)
EXT_VAR SetData uiData;
EXT_VAR ElecData elecData;
EXT_VAR rt_uint8_t flashFlag;
//EXT_VAR SetData useData;

/**
 * usart dta
*/
#define STRING_TYPE 1
#define CHAR_ARRAY_TYPE 0
#define BUFFER_LEN 256
EXT_VAR rt_uint8_t sendBuffer[BUFFER_LEN];
EXT_VAR rt_uint8_t receiveBuffer[BUFFER_LEN];

/**
* events
*/
//measure thread event
#define MEASURE_HUM_TEM_EVENT_FLAG (1<<0)
//usart thread event
#define USART_SEND_SETTING_PARAMETERS_EVENT_FLAG (1<<0)
#define USART_RECEIVE_ELEC_DATA_EVENT_FLAG (1<<1)


/**
 * definete function
*/
void initGlobalVar(void);

#endif

