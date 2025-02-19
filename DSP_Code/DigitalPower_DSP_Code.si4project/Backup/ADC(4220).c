/*
 * ADC.c
 *
 *  Created on: 2023/8/5
 *      Author: MiniSforum
 */
#include "ADC.h"

/**
 * ADC parameters
 */
static Uint32 samplingValue[4];   // sampling value array
static Uchar8 samplingOkFlag = 0; // sampling OK flag bit
#define ADC_VALUE_MAXIMUM 4095    // 12 bit sampling
#define MAX_SAMPLING_CHANNEL 4    // sampling channel
#define SAMPLING_COUNT 200        // sampling count
#define CAL_EXPANSION_FACTOR 1000 // expansion factor to decrease error
#define VOL_REDUCTION_FACTOR 10   // by resistant to reduce 10 times
const Uint16 tempTable[] = {
    3413, // 0
    3351,
    3290,
    3228,
    3167,
    3107,
    3046,
    2987,
    2927,
    2868,
    2810,
    2753,
    2695,
    2639,
    2582,
    2527,
    2472,
    2418,
    2365,
    2312,
    2260,
    2208,
    2158,
    2108,
    2059,
    2011,
    1963,
    1916,
    1870,
    1825,
    1781,
    1737,
    1695,
    1653,
    1612,
    1572,
    1533,
    1494,
    1457,
    1420,
    1384,
    1349,
    1314,
    1281,
    1248,
    1216,
    1184,
    1154,
    1124,
    1095,
    1067,
    1039,
    1012,
    986,
    961,
    936,
    912,
    888,
    865,
    843,
    830,
    800,
    779,
    760,
    740,
    721,
    703,
    685,
    668,
    650,
    634,
    618,
    602,
    588,
    573,
    567, //75
};                                       // number:76 temperature table
#define MAX_TEMP_COUNT sizeof(tempTable) // temperature table size
#define PER_CURRENT_VOL 25               // 1mv = 2.5mA
#define ZERO_CURRENT_VOL 2500            // CC6900_5V VCC = 5V, so zero vol is 2.5V
typedef struct
{
    Uint16 outputVolAdjust;
    Uint16 inputVolAdjust;
    Uint16 tempVolAdjust;
    Uint16 currentAdjust;
} CalAdjust;
static CalAdjust calAdjust; // adjust converted value

/*
 * ADC functions
 * all electronic parameters keep two decimals
 */
static void ADCCalAdjust(uint32_t src, uint32_t dest, uint16_t *adjust);
static void ADCGetTemp(void);
static void ADCGetCurrent(void);
static void ADCGetPower(void);
void ADCInit(void)
{
    // 1.config ADC
    InitAdc();
    EALLOW;
    AdcRegs.ADCTRL1.bit.ACQ_PS = 0xf;
    AdcRegs.ADCTRL3.bit.ADCCLKPS = 0x1;                          // 25/1 = 25MHZ
    AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;                            // cascade mode
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x8;                       // B0 sampling channel
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0xA;                       // B2
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0xC;                       // B4
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0xE;                       // B6
    AdcRegs.ADCTRL1.bit.CONT_RUN = 1;                            // continuous run
    AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = MAX_SAMPLING_CHANNEL - 1; // max sampling channel
    // start SEQ1
    AdcRegs.ADCTRL2.all = 0x2000;
    EDIS;

    calAdjust.outputVolAdjust = 0;
    calAdjust.inputVolAdjust = 0;
    calAdjust.tempVolAdjust = 0;
    calAdjust.currentAdjust = 0;
}
Uint16 ADCReadValue(void)
{
    static Uint16 calCount = 0;
    while (AdcRegs.ADCST.bit.INT_SEQ1 != 1)
        ;
    samplingValue[0] += AdcRegs.ADCRESULT0 >> 4;
    samplingValue[1] += AdcRegs.ADCRESULT1 >> 4;
    samplingValue[2] += AdcRegs.ADCRESULT2 >> 4;
    samplingValue[3] += AdcRegs.ADCRESULT3 >> 4;
    if ((++calCount >= SAMPLING_COUNT) && (samplingOkFlag != 1))
    {
        elecCalData.outputVol = samplingValue[3] / SAMPLING_COUNT;
        elecCalData.inputVol = samplingValue[2] / SAMPLING_COUNT;
        elecCalData.tempVol = samplingValue[1] / SAMPLING_COUNT;
        elecCalData.current = samplingValue[0] / SAMPLING_COUNT;
        for (calCount = 0; calCount < MAX_SAMPLING_CHANNEL; calCount++)
            samplingValue[calCount] = 0;
        calCount = 0;
        samplingOkFlag = 1;
    }
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1; // clear flag bit

    return 1;
}

Uint16 ADCConvertCalData(void)
{
    if (samplingOkFlag)
    {
        elecData.outputVol =
            ((elecCalData.outputVol + calAdjust.outputVolAdjust) * 3 * CAL_EXPANSION_FACTOR) / ADC_VALUE_MAXIMUM;
        elecData.inputVol = ((elecCalData.inputVol + calAdjust.inputVolAdjust) * 3 * CAL_EXPANSION_FACTOR) / ADC_VALUE_MAXIMUM;
        elecData.tempVol = ((elecCalData.tempVol + calAdjust.tempVolAdjust) * 3 * CAL_EXPANSION_FACTOR) / ADC_VALUE_MAXIMUM;
        elecData.current = ((elecCalData.current + calAdjust.currentAdjust) * 3 * CAL_EXPANSION_FACTOR) / ADC_VALUE_MAXIMUM;
        ADCCalAdjust(elecData.outputVol, useData.outputVol * 100,
                     &calAdjust.outputVolAdjust);
        ADCCalAdjust(elecData.inputVol, 1600, &calAdjust.inputVolAdjust);
        ADCGetTemp();
        ADCGetCurrent();
        ADCGetPower();
        samplingOkFlag = 0;
        return 1;
    }
    return 0;
}

static void ADCCalAdjust(uint32_t src, uint32_t dest, uint16_t *adjust)
{
    int16 error;
    error = src - dest;
    if (error > 5)
    {
        if (*adjust != 0)
            (*adjust)--; // in dsp, *adjust-- is first minus address,then use value,so need add parentheses to first use.
        // but *adjus-- is not good habit,because the address had changed,but i need only value need changed.
    }
    else if (error < -5)
    {
        (*adjust)++;
    }
}

static void ADCGetTemp(void)
{
    Uint16 min, max;
    min = MAX_TEMP_COUNT - 1, max = 0; // temperature table index equals temperature
    while (max <= min)
    {
        elecData.tempVol = (max + min) / 2;
        if (elecCalData.tempVol > tempTable[elecData.tempVol])
        {
            min = elecData.tempVol - 1;
        }
        else if (elecCalData.tempVol < tempTable[elecData.tempVol])
        {
            max = elecData.tempVol + 1;
        }
        else
        {
            elecData.tempVol += 25; // from 25�� start count
            break;
        }
    }
    elecData.tempVol += 25;
}
static void ADCGetCurrent(void)
{
    //elecData.current = (elecData.current - (ZERO_CURRENT_VOL * ADC_VALUE_MAXIMUM / 3000)) * PER_CURRENT_VOL / 10; // unit: mA
}
static void ADCGetPower(void)
{
    power = elecData.current * (elecData.outputVol * VOL_REDUCTION_FACTOR); // uint:mA*mv=mW
}
