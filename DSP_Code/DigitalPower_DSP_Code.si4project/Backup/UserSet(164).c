/*
 * UserSet.c
 *
 *  Created on: 2024Äê4ÔÂ4ÈÕ
 *      Author: MiniSforum
 */
#include "UserSet.h"
#include "Tool.h"
#include "Uart.h"
static Uchar8 parameterTemp[2 * PARAMETER_NUM];
void updateUserSettingParamater(void)
{
    // analyse received data (split string)
    Uchar8 length, num = 0;
    for (length = 0; receiveBuffer[length] != '\0'; length++)
    {
        if (receiveBuffer[length] != CHAR_BLANK && receiveBuffer[length] != CHAR_ENTER)
            parameterTemp[num++] = receiveBuffer[length];
    }
    // convert str to num
    num = 0;
    useData.outputVol = strToNum(2, &parameterTemp[num]);
    num += 2;
    useData.buzzEnable = strToNum(2, &parameterTemp[num]);
    num += 2;
    useData.musicSwitch = strToNum(2, &parameterTemp[num]);
}
void sendElecData(void)
{
    Uchar8 length = 0;
    sendBuffer[length++]='P';
    sendBuffer[length++]='E';
    sendBuffer[length++]='D';
    sendBuffer[length++]='A';
    sendBuffer[length++]='T';
    sendBuffer[length++]='A';
    numToStr(elecData.outputVol, 2, 2, &sendBuffer[length], CHAR_ARRAY_TYPE);
    length += 5;
    sendBuffer[length++] = CHAR_BLANK;

    numToStr(elecData.inputVol, 2, 2, &sendBuffer[length], CHAR_ARRAY_TYPE);
    length += 5;
    sendBuffer[length++] = CHAR_BLANK;

    numToStr(elecData.tempVol, 2, 0, &sendBuffer[length], CHAR_ARRAY_TYPE);
    length += 2;
    sendBuffer[length++] = CHAR_BLANK;

    numToStr(elecData.current, 2, 2, &sendBuffer[length], CHAR_ARRAY_TYPE);
    length += 5;
    sendBuffer[length++] = CHAR_BLANK;

//    numToStr(power, 8, 0, &sendBuffer[length], CHAR_ARRAY_TYPE);
//    length += 8;
//    sendBuffer[length++] = CHAR_BLANK;

    sendBuffer[length] = '\0';


    uartSendString(sendBuffer);
}
