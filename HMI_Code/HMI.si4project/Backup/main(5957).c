#include "Button.h"
#include "IIC.h"
#include "Usart.h"
#include "Lcd.h"
#include "AHT20.h"
#include "Buzzer.h"
#include "Flash.h"
#include "PWM.h"
#include "PID.h"
#define USING_VIEW_CURRENT_THREAD 0
/**
 * create thread
 */
static rt_thread_t create_tid = RT_NULL;
static void create_thread(void *parameter);
#define CREATE_STACK_SIZE 256
#define CREATE_PRIO 0
#define CREATE_TICK 20
/**
 * usart thread
 */
static rt_thread_t usart_tid = RT_NULL;
static void usart_thread(void *parameter);
#define USART_STACK_SIZE 512
#define USART_PRIO 28
#define USART_TICK 20
/**
 * ui thread
 */
static rt_thread_t ui_tid = RT_NULL;
static void ui_thread(void *parameter);
#define UI_STACK_SIZE 1024
#define UI_PRIO 30
extern rt_uint8_t pagination;
#define UI_TICK 20
extern rt_uint8_t setting_item_num;
extern rt_uint8_t musicSwitch;
#define PRESS_TIME 100
typedef struct
{
	rt_uint8_t keyValue;
	rt_uint8_t preKeyValue;
	rt_uint8_t pageNum;
	rt_uint8_t entrySet;
	rt_uint8_t entrySetData;
	rt_uint8_t entryFirst;
	rt_uint8_t pressTime;
	rt_uint8_t flicker;
	rt_uint32_t timerCount;
} UI_thread_struct;
enum
{
	LONG_PRESS_ENABLE = 1,
	LONG_PRESS_DISABLE = !LONG_PRESS_ENABLE
};
/**
 * measure thread
 */
static rt_thread_t measure_tid = RT_NULL;
static void measure_thread(void *parameter);
#define MEASURE_STACK_SIZE 256
#define MEASURE_PRIO 29
#define MEASURE_TICK 20
/**
 * timer
 * onlyt support 10ms * n , n is integer
 */
#define MAX_TIMER_COUNT 400000000 // 4 billions
static rt_timer_t timer_base_10ms = RT_NULL;
static void timer_base_10ms_out(void *parameter);
static rt_bool_t timerArrived(rt_uint32_t *last_timer_count, rt_uint32_t ms);
static rt_uint32_t timerCount = 0;

static rt_timer_t timer_uart_receive_1s = RT_NULL;
static void timer_uart_receive_data(void *parameter);
/**
 * ipc
 */
// rt_mutex_t hum_tem_mutex = RT_NULL;
extern rt_uint16_t hum, tem;
/**
 * event
 */
rt_event_t measure_event = RT_NULL;
rt_event_t usart_event = RT_NULL;
/**
 * scheduler hook
 */
#if USING_VIEW_CURRENT_THREAD
static void hook_of_scheduler(struct rt_thread *from, struct rt_thread *to);
static void hook_of_scheduler(struct rt_thread *from, struct rt_thread *to)
{
	printf("from:%s--->to:%s\r\n", from->name, to->name);
}
#endif
void allInit(void)
{

    //SCB->VTOR = FLASH_BASE|0x2000; //offset 8KB address of vector table
	//NVIC_SetVectorTable(FLASH_BASE, 0x2000);
	IICInit();
	initGlobalVar();
	flashInit();
	buzzerInit(0, 0);
	//pwm_init(500,500); //5KHZ default
	// pid_init(2800,2,1,1); //28 degree pid=1,1,1
	usartInit(9600);
	buttonInit();
	lcdInit();
}
/**
 *because main thread also be seted higher priority,so created other thread can't be executed.
 *So need lower main priority.It's default priority is 3.
 *1.modify it's priority. 2.don't use while in main thread.
 */
int main(void)
{
	allInit();
	printf("Second APP,Start!\r\n");
	printf("all inited.\r\n");
	create_tid = rt_thread_create("create", create_thread, RT_NULL, CREATE_STACK_SIZE, CREATE_PRIO, CREATE_TICK);
	if (create_tid != RT_NULL)
		rt_thread_startup(create_tid);
	else
		printf("create had failed.");
}
static void create_thread(void *parameter)
{
#if USING_VIEW_CURRENT_THREAD
	// set scheduler hook
	rt_scheduler_sethook(hook_of_scheduler);
#endif

	// craete events
	measure_event = rt_event_create("mevent", RT_IPC_FLAG_PRIO);
	if (measure_event == RT_NULL)
		printf("measured event had failed.\r\n");
	usart_event = rt_event_create("uevent", RT_IPC_FLAG_PRIO);
	if (usart_event == RT_NULL)
		printf("usart event had failed.\r\n");

	// create mutex
	//	hum_tem_mutex = rt_mutex_create("htmutex", RT_IPC_FLAG_PRIO);
	//	if (hum_tem_mutex == RT_NULL)
	//		printf("humutex had failed.\r\n");

	// create usart thread
	usart_tid = rt_thread_create("usart", usart_thread, RT_NULL, USART_STACK_SIZE, USART_PRIO, USART_TICK);
	if (usart_tid != RT_NULL)
		rt_thread_startup(usart_tid);
	else
		printf("usart had failed.\r\n");

	// create ui thread
	ui_tid = rt_thread_create("ui", ui_thread, RT_NULL, UI_STACK_SIZE, UI_PRIO, UI_TICK);
	if (ui_tid != RT_NULL)
		rt_thread_startup(ui_tid);
	else
		printf("ui had failed.\r\n");

	// create measured thread
	measure_tid = rt_thread_create("measure", measure_thread, RT_NULL, MEASURE_STACK_SIZE, MEASURE_PRIO, MEASURE_TICK);
	if (measure_tid != RT_NULL)
		rt_thread_startup(measure_tid);
	else
		printf("measure had failed.\r\n");

	// creater timer
	timer_base_10ms = rt_timer_create("timer0", timer_base_10ms_out, RT_NULL, 10, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
	if (timer_base_10ms != RT_NULL)
		rt_timer_start(timer_base_10ms);
	else
		printf("timer_base_10ms had failed.\r\n");

	timer_uart_receive_1s = rt_timer_create("timer1", timer_uart_receive_data, RT_NULL, 1000, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
	if (timer_uart_receive_1s != RT_NULL)
		rt_timer_start(timer_uart_receive_1s);
	else
		printf("timer_uart_receive_1s had failed.\r\n");

	rt_thread_delete(create_tid);
}
// ui thread
static void ui_thread(void *parameter)
{
	UI_thread_struct thread_struct;
	thread_struct.keyValue = 0;
	thread_struct.preKeyValue = 0;
	thread_struct.pageNum = 0;
	thread_struct.entryFirst = 0;
	thread_struct.entrySet = 0;
	thread_struct.entrySetData = 0;
	thread_struct.pressTime = PRESS_TIME;
	thread_struct.flicker = 0;
	thread_struct.timerCount = 0;
	while (1)
	{
		thread_struct.keyValue = scanButton(LONG_PRESS_ENABLE);
		rt_thread_mdelay(10);												   // remove button jitter
		if (thread_struct.keyValue != KEY_NO && thread_struct.entryFirst != 1) // button judgement
		{
			if (thread_struct.preKeyValue != thread_struct.keyValue)
			{
				thread_struct.preKeyValue = thread_struct.keyValue;
				buttonMusic(RT_TRUE);
			}
			if (thread_struct.preKeyValue == KEY_CONFIRM && thread_struct.entrySet != 1)
			{
				if (--thread_struct.pressTime <= 0)
				{
					thread_struct.entryFirst = 1;
					thread_struct.entrySet = 1;
					thread_struct.pageNum = 0;
					thread_struct.pressTime = PRESS_TIME;
					thread_struct.preKeyValue = KEY_NO;
					buttonMusic(RT_FALSE);
				}
			}
		}
		else
		{
			if (thread_struct.keyValue == KEY_NO)
				thread_struct.entryFirst = 0;
			if (thread_struct.entrySet != 1) // main pages
			{
				if (thread_struct.preKeyValue == KEY_UP)
				{
					thread_struct.pageNum++;
					thread_struct.pageNum %= pagination;
				}
				else if (thread_struct.preKeyValue == KEY_DOWN)
				{
					if (thread_struct.pageNum <= 0)
						thread_struct.pageNum = pagination;
					thread_struct.pageNum--;
				}
				thread_struct.pressTime = PRESS_TIME;
			}
			else
			{
				if (thread_struct.entrySetData != 1) // setting pages
				{
					if (thread_struct.preKeyValue == KEY_UP)
					{
						thread_struct.pageNum++;
						thread_struct.pageNum %= setting_item_num;
					}
					else if (thread_struct.preKeyValue == KEY_DOWN)
					{
						if (thread_struct.pageNum <= 0)
							thread_struct.pageNum = setting_item_num;
						thread_struct.pageNum--;
					}
					else if (thread_struct.preKeyValue == KEY_RETURN)
					{
						thread_struct.entrySet = 0;
						thread_struct.pageNum = 0;
						rt_event_send(usart_event, USART_SEND_SETTING_PARAMETERS_EVENT_FLAG);
					}
					else if (thread_struct.preKeyValue == KEY_CONFIRM)
					{
						thread_struct.entrySetData = 1;
					}
				}
				else // adjust setting parameters
				{
					if (thread_struct.preKeyValue == KEY_UP)
					{
						lcdPlusMinusParameter(thread_struct.pageNum, PLUS);
					}
					else if (thread_struct.preKeyValue == KEY_DOWN)
					{
						lcdPlusMinusParameter(thread_struct.pageNum, MINUS);
					}
					else if (thread_struct.preKeyValue == KEY_RETURN)
					{
						thread_struct.entrySetData = 0;
					}
				}
			}
			thread_struct.preKeyValue = KEY_NO;
			buttonMusic(RT_FALSE);
		}
		if (thread_struct.entrySet || thread_struct.entrySetData)
		{
			if (timerArrived(&thread_struct.timerCount, 400))
				thread_struct.flicker = !thread_struct.flicker;
		}
		lcdShowPage(thread_struct.pageNum, thread_struct.entrySet, thread_struct.entrySetData, thread_struct.flicker);
	}
}
// usart thread
static void usart_thread(void *parameter)
{
	rt_err_t err;
	rt_uint32_t event;
	while (1)
	{
		err = rt_event_recv(usart_event, USART_SEND_SETTING_PARAMETERS_EVENT_FLAG | USART_RECEIVE_ELEC_DATA_EVENT_FLAG, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &event);
		if (err == RT_EOK)
		{
			if ((event & USART_SEND_SETTING_PARAMETERS_EVENT_FLAG) == USART_SEND_SETTING_PARAMETERS_EVENT_FLAG)
			{
				usartSendSettingParameters();
			}
			else if ((event & USART_RECEIVE_ELEC_DATA_EVENT_FLAG) == USART_RECEIVE_ELEC_DATA_EVENT_FLAG)
			{
				usartReceiveCommand();
			}
		}
		else
		{
			printf("recv failed.\r\n");
		}
	}
}
// measure thread
static void measure_thread(void *parameter)
{
	rt_err_t err;
	rt_uint32_t event;
	while (1)
	{
		err = rt_event_recv(measure_event, MEASURE_HUM_TEM_EVENT_FLAG, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &event);
		if (err == RT_EOK)
		{
			if ((event & MEASURE_HUM_TEM_EVENT_FLAG) == MEASURE_HUM_TEM_EVENT_FLAG)
			{
				getHumTempData(&hum, &tem);
				rt_thread_mdelay(500);
			}
		}
		else
		{
			printf("recv failed.\r\n");
		}
	}
}

// timer callback
static void timer_base_10ms_out(void *parameter)
{
	if (timerCount < MAX_TIMER_COUNT)
	{
		timerCount++;
	}
	else
	{
		timerCount = 0;
	}
}
// don't have delay function, just record time had past.
static rt_bool_t timerArrived(rt_uint32_t *last_timer_count, rt_uint32_t ms)
{
	if (*last_timer_count <= 0)
		*last_timer_count = timerCount;			   // record current time
	if (timerCount - *last_timer_count >= ms / 10) // now time -  last time is past time
	{
		*last_timer_count = 0;
		return RT_TRUE;
	}
	return RT_FALSE;
}

static void timer_uart_receive_data(void *parameter)
{
	//printf("send elec data.\r\n");
	rt_event_send(usart_event,USART_RECEIVE_ELEC_DATA_EVENT_FLAG);
}
