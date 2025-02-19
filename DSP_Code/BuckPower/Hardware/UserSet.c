/*
 * UserSet.c
 *
 *  Created on: 2024��4��4��
 *      Author: MiniSforum
 */
#include "UserSet.h"
#include "Tool.h"
#include "Uart.h"
static Uchar8 parameterTemp[2 * PARAMETER_NUM];
uint32_t crc32Code;
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
    int8_t i;
    // head of data frame 6 bytes
    sendBuffer[length++] = 'P';
    sendBuffer[length++] = 'E';
    sendBuffer[length++] = 'D';
    sendBuffer[length++] = 'A';
    sendBuffer[length++] = 'T';
    sendBuffer[length++] = 'A';

    // data 4+4+2+5+8=23 bytes
    numToStr(elecData.outputVol, 4, 0, &sendBuffer[length], CHAR_ARRAY_TYPE);
    length += 4;
    sendBuffer[length++] = CHAR_BLANK;

    numToStr(elecData.inputVol, 4, 0, &sendBuffer[length], CHAR_ARRAY_TYPE);
    length += 4;
    sendBuffer[length++] = CHAR_BLANK;

    numToStr(elecData.tempVol, 2, 0, &sendBuffer[length], CHAR_ARRAY_TYPE);
    length += 2;
    sendBuffer[length++] = CHAR_BLANK;

    numToStr(elecData.current, 4, 0, &sendBuffer[length], CHAR_ARRAY_TYPE);
    length += 5;
    sendBuffer[length++] = CHAR_BLANK;

    //    numToStr(power, 8, 0, &sendBuffer[length], CHAR_ARRAY_TYPE);
    //    length += 8;
    //    sendBuffer[length++] = CHAR_BLANK;

    // tail of data frame 1 bytes
    sendBuffer[length++] = CHAR_ENTER;
    // crc32 of data frame 4 bytes
    crc32Code = crc32(&sendBuffer[6], length - 6); //calculate from data to tail of data frame
    for (i = 3; i >= 0; i--)
    {
        sendBuffer[length++] = (crc32Code >> (8 * i)) & 0xff;
    }
    sendBuffer[length] = '\0'; // just for print , don't be used to send
    // total 6+23+1+4=34 bytes , length is 33
    uartSendString(sendBuffer);
}
