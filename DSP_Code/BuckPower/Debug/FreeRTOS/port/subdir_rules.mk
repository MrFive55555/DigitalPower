################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/port/%.obj: ../FreeRTOS/port/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="F:/MyProjects/DigitalPower/Code/BuckPower" --include_path="F:/MyProjects/DigitalPower/Code/BuckPower/Hardware" --include_path="D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --include_path="F:/MyProjects/DigitalPower/Code/BuckPower/FreeRTOS/src" --include_path="F:/MyProjects/DigitalPower/Code/BuckPower/FreeRTOS/include" --include_path="F:/MyProjects/DigitalPower/Code/BuckPower/FreeRTOS/port" --include_path="F:/MyProjects/DigitalPower/Code/BuckPower/Stdlib/include" --include_path="F:/MyProjects/DigitalPower/Code/BuckPower/Stdlib/src" --include_path="F:/MyProjects/DigitalPower/Code/BuckPower/User" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="FreeRTOS/port/$(basename $(<F)).d_raw" --obj_directory="FreeRTOS/port" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

FreeRTOS/port/%.obj: ../FreeRTOS/port/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="F:/MyProjects/DigitalPower/Code/BuckPower" --include_path="F:/MyProjects/DigitalPower/Code/BuckPower/Hardware" --include_path="D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --include_path="F:/MyProjects/DigitalPower/Code/BuckPower/FreeRTOS/src" --include_path="F:/MyProjects/DigitalPower/Code/BuckPower/FreeRTOS/include" --include_path="F:/MyProjects/DigitalPower/Code/BuckPower/FreeRTOS/port" --include_path="F:/MyProjects/DigitalPower/Code/BuckPower/Stdlib/include" --include_path="F:/MyProjects/DigitalPower/Code/BuckPower/Stdlib/src" --include_path="F:/MyProjects/DigitalPower/Code/BuckPower/User" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="FreeRTOS/port/$(basename $(<F)).d_raw" --obj_directory="FreeRTOS/port" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


