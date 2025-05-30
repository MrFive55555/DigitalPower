/*
* main.c
*/
#include "DSP28x_Project.h"
#include "PWM.h"
#include "Uart.h"
#include "ADC.h"
#include "IO.h"
#include "UserSet.h"
#include "Tool.h"
Uchar8 convertOKFlag = 0;
void main(void)
{

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
    uartInit(9600, PORTA);
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
            convertOKFlag = 0;
            sendElecData();
        }
        DELAY_US(5000);
    }
}
