/*
 * var.h
 *
 *  Created on: 2023Äê8ÔÂ17ÈÕ
 *      Author: MiniSforum
 */
#define OS_GLOBALS
#include "IO.h"
void initGlobalVar(void){
    receiveOKFlag = 0;

    elecCalData.outputVol = 0;
    elecCalData.inputVol = 0;
    elecCalData.tempVol = 0;
    elecCalData.current = 0;

    elecData.outputVol = 0;
    elecData.inputVol = 0;
    elecData.tempVol = 0;
    elecData.current = 0;

    useData.outputVol = 3;
    useData.buzzEnable = 1;
    useData.musicSwitch = 0;
}
