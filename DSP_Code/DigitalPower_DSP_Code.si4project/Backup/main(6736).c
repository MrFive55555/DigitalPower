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
CPU_STK_SIZE App_TaskStartStk[APP_CFG_TASK_STK_SIZE];
CPU_STK_SIZE App_TaskPendStk[APP_CFG_TASK_STK_SIZE];
CPU_STK_SIZE App_TaskPostStk[APP_CFG_TASK_STK_SIZE];

static OS_EVENT *AppTaskObjSem;

static void App_TaskStart(void *p_arg);
static void App_TaskPing(void *p_arg);
static void App_TaskPong(void *p_arg);

interrupt void cpu_timer0_isr(void); // 锟叫讹拷锟斤拷锟斤拷
void BSP_Tick_Init(void);

void BSP_Tick_Init(void)
{
    EALLOW;
    PieVectTable.TINT0 = &cpu_timer0_isr; // 锟斤拷时锟斤拷锟叫断碉拷址
    // PieVectTable.RTOSINT = &RTOSINT_ISR;    // test
    PieVectTable.OS_CPU_RTOSINT = &OS_CPU_RTOSINT_Handler; // RTOS
    // MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    // InitFlash();  //锟斤拷始锟斤拷FLASH
    EDIS;
    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer0, 150, 1000); // test 500000
    CpuTimer0Regs.TCR.all = 0x4001;       // 锟斤拷锟斤拷 TSS bit = 0
    IER |= M_INT1;                        // CPU-Timer 0锟斤拷锟斤拷CPU INT1锟斤拷使锟斤拷:
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;    // Enable TINT0 in the PIE: Group 1 interrupt 7
    EINT;                                 // 使锟斤拷INTM锟叫讹拷
    ERTM;                                 // 使锟斤拷DBGM锟叫讹拷
}
void main(void)
{
    InitSysCtrl();
    // InitGpio();  // 锟斤拷锟斤拷未锟矫碉拷  // 锟节讹拷锟斤拷锟斤拷锟斤拷始锟斤拷GPIO:
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    EALLOW;
    //   GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;  // LED0
    //   GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
    EDIS;
    EINT; // 使锟斤拷INTM锟叫讹拷
    ERTM; // 使锟斤拷DBGM锟叫讹拷

    initGlobalVar();
    ADCInit();
    uartInit(9600, PORTB);
    pwmInit(3000); // 150M/(2999+1) = 50KHZ
    adjustVoltage(POWER_ON);

    OSInit();
    OSTaskCreateExt(App_TaskStart,
                    (void *)0,
                    (CPU_STK *)&App_TaskStartStk[0],
                    (INT8U)APP_CFG_TASK_START_PRIO,
                    (INT16U)APP_CFG_TASK_START_PRIO,
                    (CPU_STK *)&App_TaskStartStk[APP_CFG_TASK_STK_SIZE - 1u],
                    (INT32U)APP_CFG_TASK_STK_SIZE,
                    (void *)0,
                    (INT16U)(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    OSStart();
    while (DEF_TRUE)
    {
        ;
    }
}

static void App_TaskStart(void *p_arg)
{
    Uchar8 convertOKFlag = 0;
    volatile CPU_INT08U os_err; // test
    (void)&p_arg;
    BSP_Tick_Init();
    AppTaskObjSem = OSSemCreate(0);

    OSTaskCreateExt(App_TaskPing,
                    (void *)0,
                    (CPU_STK *)&App_TaskPendStk[0],
                    (INT8U)APP_CFG_TASK_PEND_PRIO,
                    (INT16U)APP_CFG_TASK_PEND_PRIO,
                    (CPU_STK *)&App_TaskPendStk[APP_CFG_TASK_STK_SIZE - 1u],
                    (INT32U)APP_CFG_TASK_STK_SIZE,
                    (void *)0,
                    (INT16U)(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    OSTaskCreateExt(App_TaskPong,
                    (void *)0,
                    (CPU_STK *)&App_TaskPostStk[0],
                    (INT8U)APP_CFG_TASK_POST_PRIO,
                    (INT16U)APP_CFG_TASK_POST_PRIO,
                    (CPU_STK *)&App_TaskPostStk[APP_CFG_TASK_STK_SIZE - 1u],
                    (INT32U)APP_CFG_TASK_STK_SIZE,
                    (void *)0,
                    (INT16U)(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    while (DEF_TRUE)
    {
        os_err = OSSemPost(AppTaskObjSem);
        OSTimeDlyHMSM(0, 0, 0, 1);
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
//        DELAY_US(5000); //don't use in rtos,it will halt task to switch
    }
}
static void App_TaskPing(void *p_arg)
{
    CPU_INT08U os_err;
    (void)&p_arg;
    while (DEF_TRUE)
    {
        OSSemPend(AppTaskObjSem,
                  0,
                  &os_err);
    }
}
static void App_TaskPong(void *p_arg)
{
    (void)&p_arg;
    while (DEF_TRUE)
    {
        OSTimeDlyHMSM(0, 0, 0, 300); // test   before is 300
//        LED0 = 1;
        CpuTimer0.InterruptCount++; // test
    }
}
interrupt void cpu_timer0_isr(void)
{
//     LED0=1;     // test
    // CpuTimer0.InterruptCount++;  //test
    OSIntEnter();
    OSTimeTick();
    OSIntExit();
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
