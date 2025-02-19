#include "CAN.h"
#include "Usart.h"
#define WAIT_SEND_DELAY 0xFFFF
extern uint8_t CANRxBuffer[256];
extern uint8_t RxBufferLength;
void CANInit(void){
    GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    //1.enable can clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
    //remap gpio for can
    GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE); //RX:PB8 TX:PB9
	
	//2. config IO for CAN
	//TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//3.config CAN
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	CAN_InitStructure.CAN_TTCM=DISABLE; //MCR-TTCM 关闭时间触发通信模式使能
	CAN_InitStructure.CAN_ABOM=ENABLE; //MCR-ABOM 使能自动离线管理
	CAN_InitStructure.CAN_AWUM=ENABLE; //MCR-AWUM 使用自动唤醒模式
	CAN_InitStructure.CAN_NART=DISABLE; //MCR-NART 禁止报文自动重传
	CAN_InitStructure.CAN_RFLM=DISABLE; //MCR-RFLM 接收 FIFO 不锁定
	CAN_InitStructure.CAN_TXFP=DISABLE; //MCR-TXFP 发送 FIFO 优先级 取决于报文标示符
	CAN_InitStructure.CAN_Mode =  CAN_Mode_LoopBack; //正常工作模式
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq; //BTR-SJW 重新同步跳跃宽度 1 个时间单位
	/* ss=1 bs1=5 bs2=3 位时间宽度为 (1+5+3) 波特率即为时钟周期 tq*(1+3+5) */
	CAN_InitStructure.CAN_BS1=CAN_BS1_5tq; //BTR-TS1 时间段 1 占用了 5 个时间单位
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq; //BTR-TS1 时间段 2 占用了 3 个时间单位
	/* CAN Baudrate = 1 MBps (1MBps 已为 stm32 的 CAN 最高速率) (CAN 时钟频率为 APB1 = 36 MHz) */
	//BTR-BRP 波特率分频器 定义了时间单元的时间长度 36/(1+5+3)/4=1 Mbps
	CAN_InitStructure.CAN_Prescaler = 4;
	CAN_Init(CAN1, &CAN_InitStructure);
	
	//4.config CAN Filter
	/*CAN 筛选器初始化 */
	CAN_FilterInitStructure.CAN_FilterNumber=0; //筛选器组 0
	//工作在掩码模式
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	//筛选器位宽为单个 32 位
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	/* 使能筛选器，按照标志符的内容进行比对筛选，扩展 ID 不是如下的就抛弃掉，是的话，会存入 FIFO0。 */
	//要筛选的 ID 高位，第 0 位保留，第 1 位为 RTR 标志，第 2 位为 IDE 标志，从第 3 位开始是 EXID
	CAN_FilterInitStructure.CAN_FilterIdHigh= ((((u32)0x1314<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF0000)>>16;
	//要筛选的 ID 低位
	CAN_FilterInitStructure.CAN_FilterIdLow= (((u32)0x1314<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;
	//筛选器高 16 位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0xFFFF;
	//筛选器低 16 位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0xFFFF;
	//筛选器被关联到 FIFO0
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
	//使能筛选器
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	/*CAN 通信中断使能 */
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	
	//5.config nvic for can receive
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn; //CAN RX 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//send msg
void CAN_SendMsg(char *stringData){
	uint8_t ubCounter = 0;
	uint32_t waitCounter = 0;
	uint8_t  TxMailBox;
	CanTxMsg TxMessage;
	TxMessage.ExtId = 0x1314;
	///TxMessage.StdId = 0x07FF;
	TxMessage.IDE = CAN_ID_EXT;
	TxMessage.RTR = CAN_RTR_Data;
	TxMessage.DLC = 8;
	while(*stringData!='\0'){
		TxMessage.Data[ubCounter++] = *stringData++;
		if(ubCounter>=8) {
			ubCounter = 0;
			TxMailBox = CAN_Transmit(CAN1,&TxMessage);
			while(CAN_TransmitStatus(CAN1,TxMailBox)!=CAN_TxStatus_Ok){
				if(++waitCounter>=WAIT_SEND_DELAY) {
					waitCounter = 0;
					printf("CAN send timeout!\r\n");
					return;
				}
			}
		}
	}	
	if(ubCounter!=0){
		TxMailBox = CAN_Transmit(CAN1,&TxMessage);
		while(CAN_TransmitStatus(CAN1,TxMailBox)!=CAN_TxStatus_Ok){
			printf("CAN send data.\r\n");
			if(++waitCounter>=WAIT_SEND_DELAY) {
				waitCounter = 0;
				printf("CAN send timeout!\r\n");
				return;
			}
		}
	}
}

//receive msg
/*void USB_LP_CAN1_RX0_IRQHandler(void){
	//printf("Enter CAN RX0 IRQHandler\r\n");
	uint8_t ubCounter;
	CanRxMsg RxMessage;
	CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);
	if((RxMessage.ExtId!=0x1314)||(RxMessage.IDE!=CAN_ID_EXT)||RxMessage.DLC!=8){
		printf("can receive failure!\r\n");
	}else{
		for(ubCounter=0;ubCounter<RxMessage.DLC;ubCounter++){
			CANRxBuffer[RxBufferLength++] = RxMessage.Data[ubCounter];
		}
	}
}
*/
