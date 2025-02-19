#include "DMA.h"
void DMA_Config(DMA_Channel_TypeDef *DMA_Channelx,rt_uint32_t sourceAddress,rt_uint32_t destAddress,rt_uint32_t dataSize){
	DMA_InitTypeDef DMA_InitStructure;
	rt_uint32_t MemDataSize,perDataSize;
	//�趨�������ݳ���
	switch(dataSize){
		case 8:
			perDataSize = DMA_PeripheralDataSize_Byte;
			MemDataSize = DMA_MemoryDataSize_Byte;
			break;
		case 16:
			perDataSize = DMA_PeripheralDataSize_HalfWord;
			MemDataSize = DMA_MemoryDataSize_HalfWord;
			break;
		case 32:
			perDataSize = DMA_PeripheralDataSize_Word;
			MemDataSize = DMA_MemoryDataSize_Word;
			break;
	}
	DMA_DeInit(DMA_Channelx);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //don't forget to enable peripher clock
	
	DMA_InitStructure.DMA_MemoryBaseAddr = sourceAddress; //memory address
	DMA_InitStructure.DMA_PeripheralBaseAddr = destAddress; //peripher address
	DMA_InitStructure.DMA_BufferSize = 0; //transmitter data size
	DMA_InitStructure.DMA_MemoryDataSize = MemDataSize; //data unit is byte
	DMA_InitStructure.DMA_PeripheralDataSize = perDataSize; //make sure data unit keep same
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; //direction is memory to peripher
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; //memory address don't auto-increment
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //periph address dont't auto-increment
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //only transimtter once
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh ; //medium priority
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //don't use memory to memory
	
	DMA_Init(DMA_Channelx,&DMA_InitStructure); //channel3 is SPI_TX
}
//����һ�� DMA ����
void DMANormalEnable(DMA_Channel_TypeDef *DMA_Channelx,rt_uint32_t bufferSize)
{ 	//�ȹر���ʹ�� ������һ��DMA����
    DMA_Cmd(DMA_Channelx, DISABLE); //�ر�ͨ�� 
    DMA_SetCurrDataCounter(DMA_Channelx,bufferSize); //���� DMA ����Ĵ�С
    DMA_Cmd(DMA_Channelx, ENABLE); //ʹ��ͨ��
}
