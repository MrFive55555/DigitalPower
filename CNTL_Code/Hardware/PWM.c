/*
 * PWM.c
 *
 *  Created on: 2023��8��14��
 *      Author: MiniSforum
 */
#include "PWM.h"
#define EXPANSION_FACTOR 100000
typedef struct
{
    Uint32 period;
    Uint16 pwmPeriod;
    Uint32 pwmDuty;
    Uint16 oldInputVol;
    int16 inputVolErr;
    Uint16 pidOutputVolTem;
} PWMStruct;
static PWMStruct pwmStruct;

#define SD(x)                               \
    if (x)                                  \
        GpioDataRegs.GPASET.bit.GPIO11 = 1; \
    else                                    \
        GpioDataRegs.GPACLEAR.bit.GPIO11 = 1

void pwmInit(Uint16 tbprd)
{
    pwmStruct.period = tbprd;
    // 1.enable clock by internal InitSysCtrl()

    // 2.initialize GPIO for EPWM
    InitEPwm6Gpio();

    EALLOW;

    // 3.initialize TB
    EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
    EPwm6Regs.TBCTL.bit.PHSEN = TB_DISABLE;
    EPwm6Regs.TBPHS.half.TBPHS = 0;
    EPwm6Regs.TBCTR = 0x0000;    // counter
    EPwm6Regs.TBPRD = tbprd - 1; // period actual period = tbprd - 1
    EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;
    EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1; // f = 150M/(1*1)
    EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    // 4.initialize CC
    EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm6Regs.CMPA.half.CMPA = 0; // setting duty
    EPwm6Regs.CMPB = 0;           // setting duty

    // 5.initialize AQ
    EPwm6Regs.AQCTLA.bit.ZRO = AQ_CLEAR; // Low Level
    EPwm6Regs.AQCTLA.bit.CAU = AQ_SET;   // High Level
    EPwm6Regs.AQCTLB.bit.ZRO = AQ_SET;
    EPwm6Regs.AQCTLB.bit.CBU = AQ_CLEAR;

    // 6.initialize ET
    //    EPwm6Regs.ETSEL.bit.INTSEL = ET_CTR_PRD; // Select INT on PRD event
    //    EPwm6Regs.ETSEL.bit.INTEN = 1; // Enable INT
    //    EPwm6Regs.ETPS.bit.INTPRD = ET_2ND; // Generate INT on 2nd event
    //    EPwm6Regs.ETPS.bit.INTCNT = 0;

    // config ET interrupt for peripheral->PIE->CPU
    //    PieCtrlRegs.PIEIER3.bit.INTx6 = 1;
    //    PieVectTable.EPWM6_INT = &EPWM6_INT_IRQn;
    //    IER |= M_INT3;
    //    EINT;
    //    ERTM;

    // DB
    //    EPwm6Regs.DBCTL.bit.IN_MODE = 0;
    //    EPwm6Regs.DBCTL.bit.POLSEL = 1; // all don't reverse
    //    EPwm6Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE; //two edges to delay
    //    EPwm6Regs.DBRED = 49; // 49*TBCLK(150M)
    //    EPwm6Regs.DBFED = 0;

    // TZ
    //    EPwm6Regs.TZSEL.bit.OSHT6 = 1; //one shot six
    //    EPwm6Regs.TZCTL.bit.TZA = 1; //force low
    //    EPwm6Regs.TZEINT.bit.OST = 1; //enable interrupt
    //    PieCtrlRegs.PIEIER2.bit.INTx6 = 1;
    //    PieVectTable.EPWM6_TZINT = &EPWM6_TZINT_IRQn;
    //    IER |= M_INT2;
    //    EINT;
    //    ERTM;

    // config SD for IR2104
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;   // enable pull up
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;  // normal IO
    GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;   // output
    GpioDataRegs.GPACLEAR.bit.GPIO11 = 1; // low level

    // 7.enable clock of PWM TB
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

    pwmStruct.inputVolErr = 0;
    pwmStruct.oldInputVol = 0;
    pwmStruct.pwmPeriod = 0;
    pwmStruct.pwmDuty = 0;

}

// modify duty
static void setCompare(enum_EPWM ePwmx, Uint16 period)
{
    switch (ePwmx)
    {
    case EPWMA:
        EPwm6Regs.CMPA.half.CMPA = period - 1;
        break;
    case EPWMB:
        EPwm6Regs.CMPB = period - 1;
        break;
    default:
        break;
    }
}

// adjust voltage by user setting
void adjustVoltage(Uchar8 status)
{
    if (status == POWER_ON)
    {
        SD(1); // enable IR2104
        setCompare(EPWMA, 0);
    }
    else if (status == POWER_OFF)
    {
        SD(0);
    }
    else
    {
        pwmStruct.inputVolErr = pwmStruct.oldInputVol - elecData.inputVol;
        if (pwmStruct.inputVolErr > 50 || pwmStruct.inputVolErr < -50 || status == POWER_SETTING)
        {
            // coarse tuning
            pwmStruct.pwmDuty = (useData.outputVol * EXPANSION_FACTOR) / elecData.inputVol;
            pwmStruct.pwmPeriod = pwmStruct.period - (pwmStruct.pwmDuty * pwmStruct.period / 1000);
            setCompare(EPWMA, pwmStruct.pwmPeriod);
            pwmStruct.oldInputVol = elecData.inputVol;
        }
    }
}
// EPWM ET interrupt
/*interrupt void EPWM6_INT_IRQn(void){
 ledCounter++; //200us add once
 if(ledCounter>=25000){ //2500*200 = 500000us = 500ms
 ledCounter = 0;
 // LED_TIMER_TOGGLE;
 EALLOW;
 EPwm6Regs.TZFRC.bit.OST = 1; //force trigger
 EDIS;
 }
 EALLOW;
 EPwm6Regs.ETCLR.bit.INT = 1; //must use here to clear ET interrupt flag
 PieCtrlRegs.PIEACK.bit.ACK3 = 1;
 EDIS;
 }*/

// EPWM TZ interrupt
/*interrupt void EPWM6_TZINT_IRQn(void){
 EALLOW;
 LED_TIMER_TOGGLE;
 EPwm6Regs.TZCLR.bit.INT = 1;
 EDIS;

 }*/
