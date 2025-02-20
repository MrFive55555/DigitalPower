################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Hardware/ADC.obj" \
"./Hardware/PWM.obj" \
"./Hardware/Tool.obj" \
"./Hardware/Uart.obj" \
"./Hardware/UserSet.obj" \
"./Stdlib/source/DSP2833x_ADC_cal.obj" \
"./Stdlib/source/DSP2833x_Adc.obj" \
"./Stdlib/source/DSP2833x_CSMPasswords.obj" \
"./Stdlib/source/DSP2833x_CodeStartBranch.obj" \
"./Stdlib/source/DSP2833x_CpuTimers.obj" \
"./Stdlib/source/DSP2833x_DBGIER.obj" \
"./Stdlib/source/DSP2833x_DMA.obj" \
"./Stdlib/source/DSP2833x_DefaultIsr.obj" \
"./Stdlib/source/DSP2833x_DisInt.obj" \
"./Stdlib/source/DSP2833x_ECan.obj" \
"./Stdlib/source/DSP2833x_ECap.obj" \
"./Stdlib/source/DSP2833x_EPwm.obj" \
"./Stdlib/source/DSP2833x_EQep.obj" \
"./Stdlib/source/DSP2833x_GlobalVariableDefs.obj" \
"./Stdlib/source/DSP2833x_Gpio.obj" \
"./Stdlib/source/DSP2833x_I2C.obj" \
"./Stdlib/source/DSP2833x_Mcbsp.obj" \
"./Stdlib/source/DSP2833x_MemCopy.obj" \
"./Stdlib/source/DSP2833x_PieCtrl.obj" \
"./Stdlib/source/DSP2833x_PieVect.obj" \
"./Stdlib/source/DSP2833x_Sci.obj" \
"./Stdlib/source/DSP2833x_Spi.obj" \
"./Stdlib/source/DSP2833x_SysCtrl.obj" \
"./Stdlib/source/DSP2833x_Xintf.obj" \
"./Stdlib/source/DSP2833x_usDelay.obj" \
"./User/main.obj" \
"./User/os_var.obj" \
"../targetConfigs/28335_RAM_lnk.cmd" \
"../targetConfigs/DSP2833x_Headers_nonBIOS.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include Hardware/subdir_vars.mk
-include Stdlib/source/subdir_vars.mk
-include User/subdir_vars.mk
-include targetConfigs/subdir_vars.mk
-include Hardware/subdir_rules.mk
-include Stdlib/source/subdir_rules.mk
-include User/subdir_rules.mk
-include targetConfigs/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
BuckPower.out 

EXE_OUTPUTS__QUOTED += \
"BuckPower.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "BuckPower.out"

# Tool invocations
BuckPower.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi -z -m"BuckPower.map" --stack_size=0x300 --warn_sections -i"D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/lib" -i"D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="BuckPower_linkInfo.xml" --rom_model -o "BuckPower.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "Hardware\ADC.obj" "Hardware\PWM.obj" "Hardware\Tool.obj" "Hardware\Uart.obj" "Hardware\UserSet.obj" "Stdlib\source\DSP2833x_ADC_cal.obj" "Stdlib\source\DSP2833x_Adc.obj" "Stdlib\source\DSP2833x_CSMPasswords.obj" "Stdlib\source\DSP2833x_CodeStartBranch.obj" "Stdlib\source\DSP2833x_CpuTimers.obj" "Stdlib\source\DSP2833x_DBGIER.obj" "Stdlib\source\DSP2833x_DMA.obj" "Stdlib\source\DSP2833x_DefaultIsr.obj" "Stdlib\source\DSP2833x_DisInt.obj" "Stdlib\source\DSP2833x_ECan.obj" "Stdlib\source\DSP2833x_ECap.obj" "Stdlib\source\DSP2833x_EPwm.obj" "Stdlib\source\DSP2833x_EQep.obj" "Stdlib\source\DSP2833x_GlobalVariableDefs.obj" "Stdlib\source\DSP2833x_Gpio.obj" "Stdlib\source\DSP2833x_I2C.obj" "Stdlib\source\DSP2833x_Mcbsp.obj" "Stdlib\source\DSP2833x_MemCopy.obj" "Stdlib\source\DSP2833x_PieCtrl.obj" "Stdlib\source\DSP2833x_PieVect.obj" "Stdlib\source\DSP2833x_Sci.obj" "Stdlib\source\DSP2833x_Spi.obj" "Stdlib\source\DSP2833x_SysCtrl.obj" "Stdlib\source\DSP2833x_Xintf.obj" "Stdlib\source\DSP2833x_usDelay.obj" 
	-$(RM) "User\main.obj" "User\os_var.obj" 
	-$(RM) "Hardware\ADC.d" "Hardware\PWM.d" "Hardware\Tool.d" "Hardware\Uart.d" "Hardware\UserSet.d" "Stdlib\source\DSP2833x_Adc.d" "Stdlib\source\DSP2833x_CpuTimers.d" "Stdlib\source\DSP2833x_DMA.d" "Stdlib\source\DSP2833x_DefaultIsr.d" "Stdlib\source\DSP2833x_ECan.d" "Stdlib\source\DSP2833x_ECap.d" "Stdlib\source\DSP2833x_EPwm.d" "Stdlib\source\DSP2833x_EQep.d" "Stdlib\source\DSP2833x_GlobalVariableDefs.d" "Stdlib\source\DSP2833x_Gpio.d" "Stdlib\source\DSP2833x_I2C.d" "Stdlib\source\DSP2833x_Mcbsp.d" "Stdlib\source\DSP2833x_MemCopy.d" "Stdlib\source\DSP2833x_PieCtrl.d" "Stdlib\source\DSP2833x_PieVect.d" "Stdlib\source\DSP2833x_Sci.d" "Stdlib\source\DSP2833x_Spi.d" "Stdlib\source\DSP2833x_SysCtrl.d" "Stdlib\source\DSP2833x_Xintf.d" "User\main.d" "User\os_var.d" 
	-$(RM) "Stdlib\source\DSP2833x_ADC_cal.d" "Stdlib\source\DSP2833x_CSMPasswords.d" "Stdlib\source\DSP2833x_CodeStartBranch.d" "Stdlib\source\DSP2833x_DBGIER.d" "Stdlib\source\DSP2833x_DisInt.d" "Stdlib\source\DSP2833x_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

