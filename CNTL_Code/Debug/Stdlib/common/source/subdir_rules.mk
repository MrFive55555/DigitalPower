################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Stdlib/common/source/%.obj: ../Stdlib/common/source/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="F:/MyProjects/DigitalPower/CNTL_Code" --include_path="F:/MyProjects/DigitalPower/CNTL_Code/Stdlib/common/include" --include_path="F:/MyProjects/DigitalPower/CNTL_Code/Stdlib/headers/include" --include_path="F:/MyProjects/DigitalPower/CNTL_Code/User" --include_path="F:/MyProjects/DigitalPower/CNTL_Code/Hardware" --include_path="D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="Stdlib/common/source/$(basename $(<F)).d_raw" --obj_directory="Stdlib/common/source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Stdlib/common/source/%.obj: ../Stdlib/common/source/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="F:/MyProjects/DigitalPower/CNTL_Code" --include_path="F:/MyProjects/DigitalPower/CNTL_Code/Stdlib/common/include" --include_path="F:/MyProjects/DigitalPower/CNTL_Code/Stdlib/headers/include" --include_path="F:/MyProjects/DigitalPower/CNTL_Code/User" --include_path="F:/MyProjects/DigitalPower/CNTL_Code/Hardware" --include_path="D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="Stdlib/common/source/$(basename $(<F)).d_raw" --obj_directory="Stdlib/common/source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


