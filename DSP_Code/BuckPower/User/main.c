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
CPU_STK_SIZE main_task_stk[APP_CFG_TASK_STK_SIZE];

static void main_task(void *p_arg);

static void BSP_Tick_Init(void);
interrupt void cpu_timer0_isr(void);
static void allInit(void);

void main(void)
{
    allInit();
    OSInit();
    OSTaskCreateExt(main_task,
                    (void *)0,
                    (CPU_STK *)&main_task_stk[0],
                    (INT8U)main_task_prio,
                    (INT16U)main_task_prio,
                    (CPU_STK *)&main_task_stk[APP_CFG_TASK_STK_SIZE - 1u],
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
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;    // Enable TINT0 in the PIE: Group 1 interrupt 7
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
    // OS_CPU_SR  cpu_sr;
    // OS_ENTER_CRITICAL();                  // 进入临界区
    OSIntEnter(); // 通知内核进入ISR
    // 清除定时器中断标志
    // CpuTimer0Regs.TCR.bit.TIF = 1;
    EALLOW;
    PieCtrlRegs.PIEACK.bit.ACK1 = 1;
    EDIS;
    OSTimeTick(); // 调用系统节拍服务
    OSIntExit();  // 通知内核退出ISR，触发任务调度
    // OS_EXIT_CRITICAL();                   // 退出临界区
}
static void main_task(void *p_arg)
{
    (void)&p_arg;
    Uchar8 convertOKFlag = 0;
    while (DEF_TRUE)
    {
        adjustVoltage(POWER_UPDATE);
        ADCReadValue();
        convertOKFlag = ADCConvertCalData();
        if (convertOKFlag)
        {
            sendElecData();
        }
        if (receiveOKFlag)
        {
            receiveOKFlag = 0;
            updateUserSettingParamater();
            adjustVoltage(POWER_SETTING);
        }
        OSTimeDlyHMSM(0, 0, 0, 10);
    }
}
