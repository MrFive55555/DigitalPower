/*
 * Tool.c
 *
 *  Created on: 2023Äê8ÔÂ14ÈÕ
 *      Author: MiniSforum
 */
#include "Tool.h"
void numToStr(int32_t num, uint8_t intBits, uint8_t floatBits, uint8_t *strAddress, uint8_t type)
{
    // numIndex include '\0'
    uint8_t *saveAddress = 0;
    uint8_t numIndex = (floatBits > 0 ? intBits + floatBits + 1 : intBits + floatBits);
    if(num<0){
		*(strAddress) = '-';
		strAddress++;
    	}
	//extra address to save '-' character
    strAddress += (type != CHAR_ARRAY_TYPE ? numIndex : numIndex - 1); // offset address num index
    saveAddress = strAddress;                                          // save offseted address of str
    if (type != CHAR_ARRAY_TYPE)
    {
        *(strAddress)-- = '\0';
    }
    if (num != 0)
    {
        while (num != 0)
        {
            // when decimals save finined,add dot into array
            if (((saveAddress - strAddress) == (type != CHAR_ARRAY_TYPE ? intBits + 1 : intBits)) && floatBits > 0)
            {
                *(strAddress)-- = '.';
            }
            else
            {
                *(strAddress)-- = num % 10 + '0';
                num /= 10;
            }
        }
    }
    else
    {
        *strAddress = '0';
    }
    // fill '0' to surplus
    while (saveAddress - strAddress < numIndex) //exist risk,because there array overrach
    {
        *(strAddress)-- = '0';
    }
}
//convert str to num
Uint32 strToNum(Uchar8 numBit, Uchar8 *strAddress)
{
    Uint32 num = 0;
    if (numBit <= 0 || numBit > 10)
        return num;
    else
    {
        while (numBit)
        {
            if (numBit > 1)
            {
                num = ((*strAddress++) - '0' + num) * 10;
            }
            else
            {
                num = (*strAddress++) - '0' + num;
            }
            numBit--;
        }
    }
    return num;
}
// get length of string
uint16_t getStringLength(uint8_t *pBuf)
{
    uint16_t length = 0;
    while (*pBuf != '\0')
    {
        length++;
        pBuf++;
    }
    return length;
}
// get abs value
uint32_t getAbsValue(int32_t value)
{
    if (value < 0)
        return -value;
    return value;
}
