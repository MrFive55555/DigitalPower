/*
 * Tool.h
 *
 *  Created on: 2024��3��12��
 *      Author: MiniSforum
 */

#ifndef HARDWARE_TOOL_H_
#define HARDWARE_TOOL_H_
#include "DSP28x_Project.h"
void numToStr(int32_t num, uint8_t intBits, uint8_t floatBits, uint8_t *strAddress, uint8_t type);
Uint32 strToNum(Uchar8 numBit, Uchar8 *strAddress);
uint16_t getStringLength(uint8_t *pBuf);
uint32_t getAbsValue(int32_t value);
uint32_t crc32(uint8_t *data, uint16_t length);

#endif /* HARDWARE_TOOL_H_ */
