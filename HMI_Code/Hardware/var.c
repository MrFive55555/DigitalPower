#define GLOBAL_VAR
#include "var.h"
void initGlobalVar(void){
    uiData.outputVol = 3;
    uiData.buzzEnable = 1;
    uiData.musicSwitch = 0;
    elecData.outputVol = 0;
    elecData.inputVol = 0;
    elecData.tempVol = 0;
    elecData.current = 0;
    flashFlag = 0;
}
