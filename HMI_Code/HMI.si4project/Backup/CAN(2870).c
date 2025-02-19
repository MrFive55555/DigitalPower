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
	CAN_InitStructure.CAN_TTCM=DISABLE; //MCR-TTCM �ر�ʱ�䴥��ͨ��ģʽʹ��
	CAN_InitStructure.CAN_ABOM=ENABLE; //MCR-ABOM ʹ���Զ����߹���
	CAN_InitStructure.CAN_AWUM=ENABLE; //MCR-AWUM ʹ���Զ�����ģʽ
	CAN_InitStructure.CAN_NART=DISABLE; //MCR-NART ��ֹ�����Զ��ش�
	CAN_InitStructure.CAN_RFLM=DISABLE; //MCR-RFLM ���� FIFO ������
	CAN_InitStructure.CAN_TXFP=DISABLE; //MCR-TXFP ���� FIFO ���ȼ� ȡ���ڱ��ı�ʾ��
	CAN_InitStructure.CAN_Mode =  CAN_Mode_LoopBack; //��������ģʽ
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq; //BTR-SJW ����ͬ����Ծ��� 1 ��ʱ�䵥λ
	/* ss=1 bs1=5 bs2=3 λʱ����Ϊ (1+5+3) �����ʼ�Ϊʱ������ tq*(1+3+5) */
	CAN_InitStructure.CAN_BS1=CAN_BS1_5tq; //BTR-TS1 ʱ��� 1 ռ���� 5 ��ʱ�䵥λ
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq; //BTR-TS1 ʱ��� 2 ռ���� 3 ��ʱ�䵥λ
	/* CAN Baudrate = 1 MBps (1MBps ��Ϊ stm32 �� CAN �������) (CAN ʱ��Ƶ��Ϊ APB1 = 36 MHz) */
	//BTR-BRP �����ʷ�Ƶ�� ������ʱ�䵥Ԫ��ʱ�䳤�� 36/(1+5+3)/4=1 Mbps
	CAN_InitStructure.CAN_Prescaler = 4;
	CAN_Init(CAN1, &CAN_InitStructure);
	
	//4.config CAN Filter
	/*CAN ɸѡ����ʼ�� */
	CAN_FilterInitStructure.CAN_FilterNumber=0; //ɸѡ���� 0
	//����������ģʽ
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	//ɸѡ��λ��Ϊ���� 32 λ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	/* ʹ��ɸѡ�������ձ�־�������ݽ��бȶ�ɸѡ����չ ID �������µľ����������ǵĻ�������� FIFO0�� */
	//Ҫɸѡ�� ID ��λ���� 0 λ�������� 1 λΪ RTR ��־���� 2 λΪ IDE ��־���ӵ� 3 λ��ʼ�� EXID
	CAN_FilterInitStructure.CAN_FilterIdHigh= ((((u32)0x1314<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF0000)>>16;
	//Ҫɸѡ�� ID ��λ
	CAN_FilterInitStructure.CAN_FilterIdLow= (((u32)0x1314<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;
	//ɸѡ���� 16 λÿλ����ƥ��
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0xFFFF;
	//ɸѡ���� 16 λÿλ����ƥ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0xFFFF;
	//ɸѡ���������� FIFO0
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
	//ʹ��ɸѡ��
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	/*CAN ͨ���ж�ʹ�� */
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	
	//5.config nvic for can receive
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn; //CAN RX �ж�
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
