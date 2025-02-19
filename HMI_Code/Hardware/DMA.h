#ifndef DMA_H
#define DMA_H
#include "sys.h"
void DMA_Config(DMA_Channel_TypeDef *DMA_Channelx,rt_uint32_t sourceAddress,rt_uint32_t destAddress,rt_uint32_t dataSize);
void DMANormalEnable(DMA_Channel_TypeDef*DMA_Channelx,rt_uint32_t bufferSize);
#endif
