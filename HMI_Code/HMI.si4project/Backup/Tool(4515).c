#include "Tool.h"
#include "var.h"
/**
 * num to str
 * num,int bits,float bits
 * shortage:
 * 1.can't identify error num and bits
 * 2.need specified length
 */
/*void numToStr(uint32_t num, uint8_t intBits, uint8_t floatBits,uint8_t** strAddress)
{
    uint8_t numIndex = floatBits > 0 ? intBits + floatBits : intBits + floatBits - 1;
    *strAddress+=numIndex; //offset address num index
    uint8_t *saveAddress = *strAddress; //save offseted address of str
    if (num != 0)
    {
        while (num != 0)
        {
            //when decimals save finined,add dot into array
            if ((saveAddress - *strAddress) == intBits+1 && floatBits > 0)
            {
                *(*strAddress)-- = '.';
                //continue;
            }else{
                 *(*strAddress)-- = num % 10 + '0';
                 num /= 10;
            }
        }
    }else{
        **strAddress = '0';
    }
    *strAddress = saveAddress + 1; //modify address of str for next convert
}*/

void numToStr(uint32_t num, uint8_t intBits, uint8_t floatBits, uint8_t *strAddress, rt_uint8_t type)
{
    uint8_t *saveAddress = 0;
    if (num < 0)
    {
        *strAddress++ = '-';
        num = -num;
    }
    uint8_t numIndex = (floatBits > 0 ? intBits + floatBits + 1 : intBits + floatBits);
    strAddress += (type != CHAR_ARRAY_TYPE ? numIndex : numIndex - 1); // offset address num index
    saveAddress = strAddress;                                          // save offseted address of str
    if (type != CHAR_ARRAY_TYPE)
    {
        *strAddress-- = '\0';
    }
    if (num != 0)
    {
        while (num != 0)
        {
            // when decimals save finined,add dot into array
            if (((saveAddress - strAddress) == (type != CHAR_ARRAY_TYPE ? intBits + 1 : intBits)) && floatBits > 0)
            {
                *strAddress-- = '.';
            }
            else
            {
                *strAddress-- = num % 10 + '0';
                num /= 10;
            }
        }
    }
    else
    {
        *strAddress = '0';
    }
    // fill '0' to surplus
    if (type != CHAR_ARRAY_TYPE)
    {
        while (saveAddress - strAddress <= numIndex) // exist risk,because there array overrach
        {
            *strAddress-- = '0';
        }
    }
    else
    {
        while (saveAddress - strAddress < numIndex) // exist risk,because there array overrach
        {
            *strAddress-- = '0';
        }
    }
}
rt_int32_t strToNum(rt_uint8_t numBit, rt_uint8_t *strAddress)
{
    rt_int32_t num = 0;
    rt_uint8_t firstData = *strAddress;
    
    if (numBit <= 0 || numBit > 10)
        return num;

    if (firstData == '-')
    {
        strAddress++;
    }

    while (numBit)
    {
        num = (*(strAddress)++ - '0') + num * 10;
        numBit--;
    }

    if (firstData == '-')
    {
        num = -num;
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
    return value < 0 ? -value : value;
}
