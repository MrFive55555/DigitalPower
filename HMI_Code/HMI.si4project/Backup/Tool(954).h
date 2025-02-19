#ifndef TOOL_H
#define TOOL_H
#include "sys.h"
void numToStr(uint32_t num, uint8_t intBits, uint8_t floatBits, uint8_t *strAddress, rt_uint8_t type);
rt_uint32_t strToNum(rt_uint8_t numBit, rt_uint8_t *strAddress);
uint16_t getStringLength(uint8_t *pBuf);
uint32_t getAbsValue(int32_t value);
#endif
