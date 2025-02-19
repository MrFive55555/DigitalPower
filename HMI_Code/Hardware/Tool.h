#ifndef TOOL_H
#define TOOL_H
#include "sys.h"
void numToStr(int32_t num, uint8_t intBits, uint8_t floatBits, uint8_t *strAddress, rt_uint8_t type);
rt_int32_t strToNum(rt_uint8_t numBit, rt_uint8_t *strAddress);
uint16_t getStringLength(uint8_t *pBuf);
uint32_t getAbsValue(int32_t value);
uint32_t crc32(uint8_t *data, uint16_t length);
#endif
