################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../Stdlib/src/DSP2833x_ADC_cal.asm \
../Stdlib/src/DSP2833x_CSMPasswords.asm \
../Stdlib/src/DSP2833x_CodeStartBranch.asm \
../Stdlib/src/DSP2833x_DBGIER.asm \
../Stdlib/src/DSP2833x_DisInt.asm \
../Stdlib/src/DSP2833x_usDelay.asm 

C_SRCS += \
../Stdlib/src/DSP2833x_Adc.c \
../Stdlib/src/DSP2833x_CpuTimers.c \
../Stdlib/src/DSP2833x_DMA.c \
../Stdlib/src/DSP2833x_DefaultIsr.c \
../Stdlib/src/DSP2833x_ECan.c \
../Stdlib/src/DSP2833x_ECap.c \
../Stdlib/src/DSP2833x_EPwm.c \
../Stdlib/src/DSP2833x_EQep.c \
../Stdlib/src/DSP2833x_GlobalVariableDefs.c \
../Stdlib/src/DSP2833x_Gpio.c \
../Stdlib/src/DSP2833x_I2C.c \
../Stdlib/src/DSP2833x_Mcbsp.c \
../Stdlib/src/DSP2833x_MemCopy.c \
../Stdlib/src/DSP2833x_PieCtrl.c \
../Stdlib/src/DSP2833x_PieVect.c \
../Stdlib/src/DSP2833x_Sci.c \
../Stdlib/src/DSP2833x_Spi.c \
../Stdlib/src/DSP2833x_SysCtrl.c \
../Stdlib/src/DSP2833x_Xintf.c 

C_DEPS += \
./Stdlib/src/DSP2833x_Adc.d \
./Stdlib/src/DSP2833x_CpuTimers.d \
./Stdlib/src/DSP2833x_DMA.d \
./Stdlib/src/DSP2833x_DefaultIsr.d \
./Stdlib/src/DSP2833x_ECan.d \
./Stdlib/src/DSP2833x_ECap.d \
./Stdlib/src/DSP2833x_EPwm.d \
./Stdlib/src/DSP2833x_EQep.d \
./Stdlib/src/DSP2833x_GlobalVariableDefs.d \
./Stdlib/src/DSP2833x_Gpio.d \
./Stdlib/src/DSP2833x_I2C.d \
./Stdlib/src/DSP2833x_Mcbsp.d \
./Stdlib/src/DSP2833x_MemCopy.d \
./Stdlib/src/DSP2833x_PieCtrl.d \
./Stdlib/src/DSP2833x_PieVect.d \
./Stdlib/src/DSP2833x_Sci.d \
./Stdlib/src/DSP2833x_Spi.d \
./Stdlib/src/DSP2833x_SysCtrl.d \
./Stdlib/src/DSP2833x_Xintf.d 

OBJS += \
./Stdlib/src/DSP2833x_ADC_cal.obj \
./Stdlib/src/DSP2833x_Adc.obj \
./Stdlib/src/DSP2833x_CSMPasswords.obj \
./Stdlib/src/DSP2833x_CodeStartBranch.obj \
./Stdlib/src/DSP2833x_CpuTimers.obj \
./Stdlib/src/DSP2833x_DBGIER.obj \
./Stdlib/src/DSP2833x_DMA.obj \
./Stdlib/src/DSP2833x_DefaultIsr.obj \
./Stdlib/src/DSP2833x_DisInt.obj \
./Stdlib/src/DSP2833x_ECan.obj \
./Stdlib/src/DSP2833x_ECap.obj \
./Stdlib/src/DSP2833x_EPwm.obj \
./Stdlib/src/DSP2833x_EQep.obj \
./Stdlib/src/DSP2833x_GlobalVariableDefs.obj \
./Stdlib/src/DSP2833x_Gpio.obj \
./Stdlib/src/DSP2833x_I2C.obj \
./Stdlib/src/DSP2833x_Mcbsp.obj \
./Stdlib/src/DSP2833x_MemCopy.obj \
./Stdlib/src/DSP2833x_PieCtrl.obj \
./Stdlib/src/DSP2833x_PieVect.obj \
./Stdlib/src/DSP2833x_Sci.obj \
./Stdlib/src/DSP2833x_Spi.obj \
./Stdlib/src/DSP2833x_SysCtrl.obj \
./Stdlib/src/DSP2833x_Xintf.obj \
./Stdlib/src/DSP2833x_usDelay.obj 

ASM_DEPS += \
./Stdlib/src/DSP2833x_ADC_cal.d \
./Stdlib/src/DSP2833x_CSMPasswords.d \
./Stdlib/src/DSP2833x_CodeStartBranch.d \
./Stdlib/src/DSP2833x_DBGIER.d \
./Stdlib/src/DSP2833x_DisInt.d \
./Stdlib/src/DSP2833x_usDelay.d 

OBJS__QUOTED += \
"Stdlib\src\DSP2833x_ADC_cal.obj" \
"Stdlib\src\DSP2833x_Adc.obj" \
"Stdlib\src\DSP2833x_CSMPasswords.obj" \
"Stdlib\src\DSP2833x_CodeStartBranch.obj" \
"Stdlib\src\DSP2833x_CpuTimers.obj" \
"Stdlib\src\DSP2833x_DBGIER.obj" \
"Stdlib\src\DSP2833x_DMA.obj" \
"Stdlib\src\DSP2833x_DefaultIsr.obj" \
"Stdlib\src\DSP2833x_DisInt.obj" \
"Stdlib\src\DSP2833x_ECan.obj" \
"Stdlib\src\DSP2833x_ECap.obj" \
"Stdlib\src\DSP2833x_EPwm.obj" \
"Stdlib\src\DSP2833x_EQep.obj" \
"Stdlib\src\DSP2833x_GlobalVariableDefs.obj" \
"Stdlib\src\DSP2833x_Gpio.obj" \
"Stdlib\src\DSP2833x_I2C.obj" \
"Stdlib\src\DSP2833x_Mcbsp.obj" \
"Stdlib\src\DSP2833x_MemCopy.obj" \
"Stdlib\src\DSP2833x_PieCtrl.obj" \
"Stdlib\src\DSP2833x_PieVect.obj" \
"Stdlib\src\DSP2833x_Sci.obj" \
"Stdlib\src\DSP2833x_Spi.obj" \
"Stdlib\src\DSP2833x_SysCtrl.obj" \
"Stdlib\src\DSP2833x_Xintf.obj" \
"Stdlib\src\DSP2833x_usDelay.obj" 

C_DEPS__QUOTED += \
"Stdlib\src\DSP2833x_Adc.d" \
"Stdlib\src\DSP2833x_CpuTimers.d" \
"Stdlib\src\DSP2833x_DMA.d" \
"Stdlib\src\DSP2833x_DefaultIsr.d" \
"Stdlib\src\DSP2833x_ECan.d" \
"Stdlib\src\DSP2833x_ECap.d" \
"Stdlib\src\DSP2833x_EPwm.d" \
"Stdlib\src\DSP2833x_EQep.d" \
"Stdlib\src\DSP2833x_GlobalVariableDefs.d" \
"Stdlib\src\DSP2833x_Gpio.d" \
"Stdlib\src\DSP2833x_I2C.d" \
"Stdlib\src\DSP2833x_Mcbsp.d" \
"Stdlib\src\DSP2833x_MemCopy.d" \
"Stdlib\src\DSP2833x_PieCtrl.d" \
"Stdlib\src\DSP2833x_PieVect.d" \
"Stdlib\src\DSP2833x_Sci.d" \
"Stdlib\src\DSP2833x_Spi.d" \
"Stdlib\src\DSP2833x_SysCtrl.d" \
"Stdlib\src\DSP2833x_Xintf.d" 

ASM_DEPS__QUOTED += \
"Stdlib\src\DSP2833x_ADC_cal.d" \
"Stdlib\src\DSP2833x_CSMPasswords.d" \
"Stdlib\src\DSP2833x_CodeStartBranch.d" \
"Stdlib\src\DSP2833x_DBGIER.d" \
"Stdlib\src\DSP2833x_DisInt.d" \
"Stdlib\src\DSP2833x_usDelay.d" 

ASM_SRCS__QUOTED += \
"../Stdlib/src/DSP2833x_ADC_cal.asm" \
"../Stdlib/src/DSP2833x_CSMPasswords.asm" \
"../Stdlib/src/DSP2833x_CodeStartBranch.asm" \
"../Stdlib/src/DSP2833x_DBGIER.asm" \
"../Stdlib/src/DSP2833x_DisInt.asm" \
"../Stdlib/src/DSP2833x_usDelay.asm" 

C_SRCS__QUOTED += \
"../Stdlib/src/DSP2833x_Adc.c" \
"../Stdlib/src/DSP2833x_CpuTimers.c" \
"../Stdlib/src/DSP2833x_DMA.c" \
"../Stdlib/src/DSP2833x_DefaultIsr.c" \
"../Stdlib/src/DSP2833x_ECan.c" \
"../Stdlib/src/DSP2833x_ECap.c" \
"../Stdlib/src/DSP2833x_EPwm.c" \
"../Stdlib/src/DSP2833x_EQep.c" \
"../Stdlib/src/DSP2833x_GlobalVariableDefs.c" \
"../Stdlib/src/DSP2833x_Gpio.c" \
"../Stdlib/src/DSP2833x_I2C.c" \
"../Stdlib/src/DSP2833x_Mcbsp.c" \
"../Stdlib/src/DSP2833x_MemCopy.c" \
"../Stdlib/src/DSP2833x_PieCtrl.c" \
"../Stdlib/src/DSP2833x_PieVect.c" \
"../Stdlib/src/DSP2833x_Sci.c" \
"../Stdlib/src/DSP2833x_Spi.c" \
"../Stdlib/src/DSP2833x_SysCtrl.c" \
"../Stdlib/src/DSP2833x_Xintf.c" 


