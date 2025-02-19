#include "Usart.h"
#include "Tool.h"
#define CHAR_BLANK 32
#define CHAR_ENTER 10
#define STRING_BUFFER_MAX_LENGTH 256
rt_uint8_t parameterTemp[4 * ELEC_DATA_LENGTH];
extern rt_event_t usart_event;
rt_uint8_t string_buffer[STRING_BUFFER_MAX_LENGTH];
rt_uint8_t head_queue;
rt_uint8_t tail_queue;
rt_uint16_t data_count;
static void usartReceiveElecData(void);
/// 重定向 c 库函数 printf 到串口，重定向后可使用 printf 函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口 */
	USART_SendData(USART1, (uint8_t)ch);
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		;
	return (ch);
}
/// 重定向 c 库函数 scanf 到串口，重写向后可使用 scanf、getchar 等函数
int fgetc(FILE *f)
{
	/* 等待串口输入数据 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
		;
	return (int)USART_ReceiveData(USART1);
}
static void init_string_bufffer(void)
{
	/**
	 * uart manage data by circle queue
	 */
	head_queue = tail_queue = data_count = 0;
}
void usartInit(const u32 bound)
{
	// 初始化配置
	GPIO_InitTypeDef GPIO_InitStructure;   // GPIO
	USART_InitTypeDef USART_InitStructure; // 串口
	NVIC_InitTypeDef NVIC_InitStructure;   // 中断
	// 1.使能 串口 GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	// 2.GPIO 端口模式设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // USART1_TX PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);			// 初始化 GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		// USART1_RX PA.10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	// 下拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);			// 初始化 GPIOA.10
	// 3.串口参数初始化
	USART_InitStructure.USART_BaudRate = bound;										// 波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// 字长为 8 位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// 一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// 收发模式
	USART_Init(USART1, &USART_InitStructure);
	// 4.初始化 NVIC 分组2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); // 设置 NVIC 中断分组 2
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  // 子优先级 1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ 通道使能
	NVIC_Init(&NVIC_InitStructure);							  // 中断优先级初始化
	// 5.开启中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 开启中断
	// 6.使能串口
	USART_Cmd(USART1, ENABLE); // 使能串口

	init_string_bufffer();
}
// 发送字符
void usartSendByte(USART_TypeDef *USARTx, const u8 data)
{
	// 发送数据
	USART_SendData(USARTx, data);
	// 等待发送数据寄存器为空
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
		;
}
// 发送字符串
void usartSendString(USART_TypeDef *USARTx, const u8 *data)
{
	while (*data != '\0')
		usartSendByte(USARTx, *(data++));
	// 等待发送完成
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
		;
}
// 中断函数 接收到数据触发中断
/* void USART1_IRQHandler(void)
{
	static rt_uint8_t head_queue = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		receiveBuffer[head_queue] = USART_ReceiveData(USART1);
		if (receiveBuffer[head_queue++] == CHAR_ENTER)
		{
			receiveBuffer[head_queue] = '\0';
			head_queue = 0;
			// printf("%s\r\n",receiveBuffer);
			//printf("%s","receive all fill.\r\n");
			rt_event_send(usart_event, USART_RECEIVE_ELEC_DATA_EVENT_FLAG);
		}
	}
} */
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		if (data_count >= 256)
		{
			USART_ReceiveData(USART1);
			// printf("data had full,please take data.\r\n");
		}
		else
		{

			string_buffer[tail_queue] = USART_ReceiveData(USART1);
			if (tail_queue < STRING_BUFFER_MAX_LENGTH - 1)
				tail_queue++;
			else
				tail_queue = 0;
			data_count++;
		}
	}
}

/**
 * 2.Application
 */
// send setting parameters
void usartSendSettingParameters(void)
{
	rt_uint8_t head_queue = 0;

	numToStr(uiData.outputVol, 2, 0, &sendBuffer[head_queue], STRING_TYPE);
	head_queue += 2;
	sendBuffer[head_queue++] = CHAR_BLANK;

	numToStr(uiData.buzzEnable, 2, 0, &sendBuffer[head_queue], CHAR_ARRAY_TYPE);
	head_queue += 2;
	sendBuffer[head_queue++] = CHAR_BLANK;

	numToStr(uiData.musicSwitch, 2, 0, &sendBuffer[head_queue], CHAR_ARRAY_TYPE);
	head_queue += 2;

	sendBuffer[head_queue++] = CHAR_ENTER;
	sendBuffer[head_queue] = '\0';

	usartSendString(USART1, sendBuffer);
}

/**
 *old version which don't use circle queue.
 */
/*void usartReceiveElecData(void)
{
	rt_uint8_t head_queue, num = 0;
	for (head_queue = 0; receiveBuffer[head_queue] != '\0'; head_queue++)
	{
		if (receiveBuffer[head_queue] != CHAR_BLANK && receiveBuffer[head_queue] != CHAR_ENTER && receiveBuffer[head_queue] != '.')
			parameterTemp[num++] = receiveBuffer[head_queue];
	}
	// convert str to num
	num = 0;
	elecData.outputVol = strToNum(4, &parameterTemp[num]);
	num += 4;
	elecData.inputVol = strToNum(4, &parameterTemp[num]);
	num += 4;
	elecData.tempVol = strToNum(4, &parameterTemp[num]);
	num += 4;
	elecData.current = strToNum(4, &parameterTemp[num]);

	flashFlag = 1;
}*/
rt_uint8_t head_queue_temp;
void usartReceiveCommand(void)
{
	if (data_count <= 0)
	{
		// printf("circle queue of data is empty.\r\n");
	}
	else
	{
		head_queue_temp = head_queue;
		if (string_buffer[head_queue++] == 'P') // P indicates getting parameters
		{
			if (string_buffer[head_queue++] == 'E' && string_buffer[head_queue++] == 'D' && string_buffer[head_queue++] == 'A' && string_buffer[head_queue++] == 'T' && string_buffer[head_queue++] == 'A')
			{
				usartReceiveElecData();
			}
		}
		if (data_count != 0)
		{
			if (head_queue <= head_queue_temp)
			{
				data_count -= (STRING_BUFFER_MAX_LENGTH - head_queue_temp + head_queue);
			}
			else
			{
				data_count -= (head_queue - head_queue_temp);
			}
		}
	}
}
static void usartReceiveElecData(void)
{
	rt_uint8_t index = 0;

	while (string_buffer[head_queue] != '\0')
	{
		if (string_buffer[head_queue] != CHAR_BLANK && string_buffer[head_queue] != CHAR_ENTER && string_buffer[head_queue] != '.')
		{
			parameterTemp[index++] = string_buffer[head_queue];
		}
		if (head_queue < STRING_BUFFER_MAX_LENGTH - 1)
			head_queue++;
		else
			head_queue = 0;
	}

	// convert str to num
	// index = 0;
	// elecData.outputVol = strToNum(4, &parameterTemp[index]);
	// index += 4;
	// elecData.inputVol = strToNum(4, &parameterTemp[index]);
	// index += 4;
	// elecData.tempVol = strToNum(4, &parameterTemp[index]);
	// index += 4;
	// elecData.current = strToNum(4, &parameterTemp[index]);

	// flashFlag = 1;
}
