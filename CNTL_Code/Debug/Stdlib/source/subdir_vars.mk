################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../Stdlib/source/DSP2833x_ADC_cal.asm \
../Stdlib/source/DSP2833x_CSMPasswords.asm \
../Stdlib/source/DSP2833x_CodeStartBranch.asm \
../Stdlib/source/DSP2833x_DBGIER.asm \
../Stdlib/source/DSP2833x_DisInt.asm \
../Stdlib/source/DSP2833x_usDelay.asm 

C_SRCS += \
../Stdlib/source/DSP2833x_Adc.c \
../Stdlib/source/DSP2833x_CpuTimers.c \
../Stdlib/source/DSP2833x_DMA.c \
../Stdlib/source/DSP2833x_DefaultIsr.c \
../Stdlib/source/DSP2833x_ECan.c \
../Stdlib/source/DSP2833x_ECap.c \
../Stdlib/source/DSP2833x_EPwm.c \
../Stdlib/source/DSP2833x_EQep.c \
../Stdlib/source/DSP2833x_GlobalVariableDefs.c \
../Stdlib/source/DSP2833x_Gpio.c \
../Stdlib/source/DSP2833x_I2C.c \
../Stdlib/source/DSP2833x_Mcbsp.c \
../Stdlib/source/DSP2833x_MemCopy.c \
../Stdlib/source/DSP2833x_PieCtrl.c \
../Stdlib/source/DSP2833x_PieVect.c \
../Stdlib/source/DSP2833x_Sci.c \
../Stdlib/source/DSP2833x_Spi.c \
../Stdlib/source/DSP2833x_SysCtrl.c \
../Stdlib/source/DSP2833x_Xintf.c 

C_DEPS += \
./Stdlib/source/DSP2833x_Adc.d \
./Stdlib/source/DSP2833x_CpuTimers.d \
./Stdlib/source/DSP2833x_DMA.d \
./Stdlib/source/DSP2833x_DefaultIsr.d \
./Stdlib/source/DSP2833x_ECan.d \
./Stdlib/source/DSP2833x_ECap.d \
./Stdlib/source/DSP2833x_EPwm.d \
./Stdlib/source/DSP2833x_EQep.d \
./Stdlib/source/DSP2833x_GlobalVariableDefs.d \
./Stdlib/source/DSP2833x_Gpio.d \
./Stdlib/source/DSP2833x_I2C.d \
./Stdlib/source/DSP2833x_Mcbsp.d \
./Stdlib/source/DSP2833x_MemCopy.d \
./Stdlib/source/DSP2833x_PieCtrl.d \
./Stdlib/source/DSP2833x_PieVect.d \
./Stdlib/source/DSP2833x_Sci.d \
./Stdlib/source/DSP2833x_Spi.d \
./Stdlib/source/DSP2833x_SysCtrl.d \
./Stdlib/source/DSP2833x_Xintf.d 

OBJS += \
./Stdlib/source/DSP2833x_ADC_cal.obj \
./Stdlib/source/DSP2833x_Adc.obj \
./Stdlib/source/DSP2833x_CSMPasswords.obj \
./Stdlib/source/DSP2833x_CodeStartBranch.obj \
./Stdlib/source/DSP2833x_CpuTimers.obj \
./Stdlib/source/DSP2833x_DBGIER.obj \
./Stdlib/source/DSP2833x_DMA.obj \
./Stdlib/source/DSP2833x_DefaultIsr.obj \
./Stdlib/source/DSP2833x_DisInt.obj \
./Stdlib/source/DSP2833x_ECan.obj \
./Stdlib/source/DSP2833x_ECap.obj \
./Stdlib/source/DSP2833x_EPwm.obj \
./Stdlib/source/DSP2833x_EQep.obj \
./Stdlib/source/DSP2833x_GlobalVariableDefs.obj \
./Stdlib/source/DSP2833x_Gpio.obj \
./Stdlib/source/DSP2833x_I2C.obj \
./Stdlib/source/DSP2833x_Mcbsp.obj \
./Stdlib/source/DSP2833x_MemCopy.obj \
./Stdlib/source/DSP2833x_PieCtrl.obj \
./Stdlib/source/DSP2833x_PieVect.obj \
./Stdlib/source/DSP2833x_Sci.obj \
./Stdlib/source/DSP2833x_Spi.obj \
./Stdlib/source/DSP2833x_SysCtrl.obj \
./Stdlib/source/DSP2833x_Xintf.obj \
./Stdlib/source/DSP2833x_usDelay.obj 

ASM_DEPS += \
./Stdlib/source/DSP2833x_ADC_cal.d \
./Stdlib/source/DSP2833x_CSMPasswords.d \
./Stdlib/source/DSP2833x_CodeStartBranch.d \
./Stdlib/source/DSP2833x_DBGIER.d \
./Stdlib/source/DSP2833x_DisInt.d \
./Stdlib/source/DSP2833x_usDelay.d 

OBJS__QUOTED += \
"Stdlib\source\DSP2833x_ADC_cal.obj" \
"Stdlib\source\DSP2833x_Adc.obj" \
"Stdlib\source\DSP2833x_CSMPasswords.obj" \
"Stdlib\source\DSP2833x_CodeStartBranch.obj" \
"Stdlib\source\DSP2833x_CpuTimers.obj" \
"Stdlib\source\DSP2833x_DBGIER.obj" \
"Stdlib\source\DSP2833x_DMA.obj" \
"Stdlib\source\DSP2833x_DefaultIsr.obj" \
"Stdlib\source\DSP2833x_DisInt.obj" \
"Stdlib\source\DSP2833x_ECan.obj" \
"Stdlib\source\DSP2833x_ECap.obj" \
"Stdlib\source\DSP2833x_EPwm.obj" \
"Stdlib\source\DSP2833x_EQep.obj" \
"Stdlib\source\DSP2833x_GlobalVariableDefs.obj" \
"Stdlib\source\DSP2833x_Gpio.obj" \
"Stdlib\source\DSP2833x_I2C.obj" \
"Stdlib\source\DSP2833x_Mcbsp.obj" \
"Stdlib\source\DSP2833x_MemCopy.obj" \
"Stdlib\source\DSP2833x_PieCtrl.obj" \
"Stdlib\source\DSP2833x_PieVect.obj" \
"Stdlib\source\DSP2833x_Sci.obj" \
"Stdlib\source\DSP2833x_Spi.obj" \
"Stdlib\source\DSP2833x_SysCtrl.obj" \
"Stdlib\source\DSP2833x_Xintf.obj" \
"Stdlib\source\DSP2833x_usDelay.obj" 

C_DEPS__QUOTED += \
"Stdlib\source\DSP2833x_Adc.d" \
"Stdlib\source\DSP2833x_CpuTimers.d" \
"Stdlib\source\DSP2833x_DMA.d" \
"Stdlib\source\DSP2833x_DefaultIsr.d" \
"Stdlib\source\DSP2833x_ECan.d" \
"Stdlib\source\DSP2833x_ECap.d" \
"Stdlib\source\DSP2833x_EPwm.d" \
"Stdlib\source\DSP2833x_EQep.d" \
"Stdlib\source\DSP2833x_GlobalVariableDefs.d" \
"Stdlib\source\DSP2833x_Gpio.d" \
"Stdlib\source\DSP2833x_I2C.d" \
"Stdlib\source\DSP2833x_Mcbsp.d" \
"Stdlib\source\DSP2833x_MemCopy.d" \
"Stdlib\source\DSP2833x_PieCtrl.d" \
"Stdlib\source\DSP2833x_PieVect.d" \
"Stdlib\source\DSP2833x_Sci.d" \
"Stdlib\source\DSP2833x_Spi.d" \
"Stdlib\source\DSP2833x_SysCtrl.d" \
"Stdlib\source\DSP2833x_Xintf.d" 

ASM_DEPS__QUOTED += \
"Stdlib\source\DSP2833x_ADC_cal.d" \
"Stdlib\source\DSP2833x_CSMPasswords.d" \
"Stdlib\source\DSP2833x_CodeStartBranch.d" \
"Stdlib\source\DSP2833x_DBGIER.d" \
"Stdlib\source\DSP2833x_DisInt.d" \
"Stdlib\source\DSP2833x_usDelay.d" 

ASM_SRCS__QUOTED += \
"../Stdlib/source/DSP2833x_ADC_cal.asm" \
"../Stdlib/source/DSP2833x_CSMPasswords.asm" \
"../Stdlib/source/DSP2833x_CodeStartBranch.asm" \
"../Stdlib/source/DSP2833x_DBGIER.asm" \
"../Stdlib/source/DSP2833x_DisInt.asm" \
"../Stdlib/source/DSP2833x_usDelay.asm" 

C_SRCS__QUOTED += \
"../Stdlib/source/DSP2833x_Adc.c" \
"../Stdlib/source/DSP2833x_CpuTimers.c" \
"../Stdlib/source/DSP2833x_DMA.c" \
"../Stdlib/source/DSP2833x_DefaultIsr.c" \
"../Stdlib/source/DSP2833x_ECan.c" \
"../Stdlib/source/DSP2833x_ECap.c" \
"../Stdlib/source/DSP2833x_EPwm.c" \
"../Stdlib/source/DSP2833x_EQep.c" \
"../Stdlib/source/DSP2833x_GlobalVariableDefs.c" \
"../Stdlib/source/DSP2833x_Gpio.c" \
"../Stdlib/source/DSP2833x_I2C.c" \
"../Stdlib/source/DSP2833x_Mcbsp.c" \
"../Stdlib/source/DSP2833x_MemCopy.c" \
"../Stdlib/source/DSP2833x_PieCtrl.c" \
"../Stdlib/source/DSP2833x_PieVect.c" \
"../Stdlib/source/DSP2833x_Sci.c" \
"../Stdlib/source/DSP2833x_Spi.c" \
"../Stdlib/source/DSP2833x_SysCtrl.c" \
"../Stdlib/source/DSP2833x_Xintf.c" 


