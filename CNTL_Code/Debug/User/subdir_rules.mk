################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
User/%.obj: ../User/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="F:/MyProjects/DigitalPower/CNTL_Code" --include_path="F:/MyProjects/DigitalPower/CNTL_Code/Stdlib/include" --include_path="F:/MyProjects/DigitalPower/CNTL_Code/User" --include_path="F:/MyProjects/DigitalPower/CNTL_Code/Hardware" --include_path="D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="User/$(basename $(<F)).d_raw" --obj_directory="User" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


