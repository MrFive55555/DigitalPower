///**
// * main.c
// */
/*#include "DSP28x_Project.h"
#include "PWM.h"
#include "Uart.h"
#include "ADC.h"
#include "IO.h"
#include "UserSet.h"
#include "Tool.h"
void main(void)
{
    Uchar8 convertOKFlag = 0;
    //initialize clock
    InitSysCtrl();
    //initialized pie interrupt
    InitPieCtrl();
    IER = 0x0000; //CPU interrupt enable bits
    IFR = 0x0000; //flag bits
    InitPieVectTable();
    //FLASH mode config
    //MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd,&RamfuncsRunStart);
    //InitFlash();
    //initialize module
    initGlobalVar();
    ADCInit();
    uartInit(9600, PORTB);
    pwmInit(3000); //150M/(2999+1) = 50KHZ
    adjustVoltage(POWER_ON);
    while (1)
    {
        if (receiveOKFlag)
        {
            receiveOKFlag = 0;
            updateUserSettingParamater();
            adjustVoltage(POWER_SETTING);
        }
        adjustVoltage(POWER_UPDATE);
        ADCReadValue();
        convertOKFlag = ADCConvertCalData();
        if (convertOKFlag)
        {
            sendElecData();
        }
        DELAY_US(5000);
    }
}*/

#include <app_cfg.h>
#include <ucos_ii.h>
#include <cpu_core.h>
#include <lib_def.h>
#include <os_cpu.h>
#include <app_cfg.h>
#include <ucos_ii.h>
#include <cpu_core.h>
#include <lib_def.h>
#include <os_cpu.h>
#include "PWM.h"
#include "Uart.h"
#include "ADC.h"
#include "IO.h"
#include "UserSet.h"
#include "Tool.h"
#include "DSP28x_Project.h"

#define LED0 GpioDataRegs.GPATOGGLE.bit.GPIO11
CPU_STK_SIZE create_task_stk[APP_CFG_TASK_STK_SIZE];
CPU_STK_SIZE measure_task_stk[APP_CFG_TASK_STK_SIZE];
CPU_STK_SIZE communication_task_stk[APP_CFG_TASK_STK_SIZE];
// static OS_EVENT *measureOkSem;

static void create_task(void *p_arg);
static void measure_task(void *p_arg);
static void communication_task(void *p_arg);

static void BSP_Tick_Init(void);
interrupt void cpu_timer0_isr(void);
static void allInit(void);

/**
 * event flag
 */
#define ADC_CONVERT_OK_FLAG 0x0001
#define UART_RECEIVE_OK_FLAG 0x0002
OS_FLAG_GRP *elecDatatFlag;

void main(void)
{
    allInit();
    OSInit();
    OSTaskCreateExt(create_task,
                    (void *)0,
                    (CPU_STK *)&create_task_stk[0],
                    (INT8U)create_task_prio,
                    (INT16U)create_task_prio,
                    (CPU_STK *)&create_task_stk[APP_CFG_TASK_STK_SIZE - 1u],
                    (INT32U)APP_CFG_TASK_STK_SIZE,
                    (void *)0,
                    (INT16U)(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    OSStart();
    while (DEF_TRUE)
    {
        ;
    }
}
static void allInit(void)
{
    InitSysCtrl();
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
    EINT; // Enable Global interrupt INTM
    ERTM; // Enable Global realtime interrupt DBGM

    // initialize module
    BSP_Tick_Init();
    initGlobalVar();
    ADCInit();
    uartInit(9600, PORTB);
    pwmInit(3000); // 150M/(2999+1) = 50KHZ
    adjustVoltage(POWER_ON);
}
static void BSP_Tick_Init(void)
{
    EALLOW;
    PieVectTable.TINT0 = &cpu_timer0_isr; // Timer0 interrupt
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;     // Enable TINT0 in the PIE: Group 1 interrupt 7
    // PieVectTable.RTOSINT = &RTOSINT_ISR;    // test
    PieVectTable.OS_CPU_RTOSINT = &OS_CPU_RTOSINT_Handler; // RTOS
    // MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    // InitFlash();
    EDIS;
    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer0, 150, 1000); // test 500000
    CpuTimer0Regs.TCR.all = 0x4001;        // TSS bit = 0 to start the timer
    IER |= M_INT1;                         // CPU-Timer 0 CPU INT1
    EINT;                                  // INTM
    ERTM;                                  // DBGM
}
interrupt void cpu_timer0_isr(void)
{
    // LED0=1;     // test
    // CpuTimer0.InterruptCount++;  //test
    //OS_CPU_SR  cpu_sr;
   // OS_ENTER_CRITICAL();                  // 进入临界区
    OSIntEnter();                         // 通知内核进入ISR
    // 清除定时器中断标志
    //CpuTimer0Regs.TCR.bit.TIF = 1;
    EALLOW;
	PieCtrlRegs.PIEACK.bit.ACK1 = 1;
	EDIS;
    OSTimeTick();                         // 调用系统节拍服务
    OSIntExit();                          // 通知内核退出ISR，触发任务调度
    //OS_EXIT_CRITICAL();                   // 退出临界区
}
static void create_task(void *p_arg)
{
    (void)&p_arg;
    CPU_INT08U os_err;
    elecDatatFlag = OSFlagCreate(0, &os_err);
    OSTaskCreateExt(measure_task,
                    (void *)0,
                    (CPU_STK *)&measure_task_stk[0],
                    (INT8U)measure_task_prio,
                    (INT16U)measure_task_prio,
                    (CPU_STK *)&measure_task_stk[APP_CFG_TASK_STK_SIZE - 1u],
                    (INT32U)APP_CFG_TASK_STK_SIZE,
                    (void *)0,
                    (INT16U)(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    OSTaskCreateExt(communication_task,
                    (void *)0,
                    (CPU_STK *)&communication_task_stk[0],
                    (INT8U)communication_task_prio,
                    (INT16U)communication_task_prio,
                    (CPU_STK *)&communication_task_stk[APP_CFG_TASK_STK_SIZE - 1u],
                    (INT32U)APP_CFG_TASK_STK_SIZE,
                    (void *)0,
                    (INT16U)(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    OSTaskDel(OS_PRIO_SELF);
}
static void measure_task(void *p_arg)
{
    Uchar8 convertOKFlag = 0;
    CPU_INT08U os_err;
    (void)&p_arg;
    while (DEF_TRUE)
    {
        // conver ADC data cmplete to post flag
        adjustVoltage(POWER_UPDATE);
        ADCReadValue();
        convertOKFlag = ADCConvertCalData();
        if (convertOKFlag)
        {
            OSFlagPost(elecDatatFlag, ADC_CONVERT_OK_FLAG, OS_FLAG_SET, &os_err);
        }
        //recevie data from uart
        if (receiveOKFlag)
        {
            OSFlagPost(elecDatatFlag, UART_RECEIVE_OK_FLAG, OS_FLAG_SET, &os_err);
        }
        LED0 = 1;
        OSTimeDlyHMSM(0, 0, 0, 5);
    }
}
static void communication_task(void *p_arg)
{
    (void)&p_arg;
    CPU_INT08U os_err;
    OS_FLAGS flags;
    while (DEF_TRUE)
    {
        flags = OSFlagPend(elecDatatFlag, ADC_CONVERT_OK_FLAG | UART_RECEIVE_OK_FLAG, 0, OS_FLAG_WAIT_SET_ANY | OS_FLAG_CONSUME, &os_err);
        if (os_err == OS_ERR_NONE)
        {
            if (flags & ADC_CONVERT_OK_FLAG == ADC_CONVERT_OK_FLAG)
            {
                sendElecData();
            }
            if (flags & UART_RECEIVE_OK_FLAG == UART_RECEIVE_OK_FLAG)
            {
                receiveOKFlag = 0;
                updateUserSettingParamater();
                adjustVoltage(POWER_SETTING);
            }
        }
//		LED0 = 1;
        OSTimeDlyHMSM(0, 0, 0, 10);
    }
}
