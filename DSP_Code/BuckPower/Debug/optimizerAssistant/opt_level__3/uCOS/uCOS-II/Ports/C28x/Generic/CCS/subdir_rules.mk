################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
uCOS/uCOS-II/Ports/C28x/Generic/CCS/%.obj: ../uCOS/uCOS-II/Ports/C28x/Generic/CCS/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -O3 --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/uCOS/config" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/uCOS/uC-CPU" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/uCOS/uC-CPU/C28x/CCS" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/uCOS/uC-LIB" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/uCOS/uCOS-II/Ports/C28x/Generic/CCS" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/uCOS/uCOS-II/Source" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/Hardware" --include_path="D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/Stdlib/include" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/Stdlib/src" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/User" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="uCOS/uCOS-II/Ports/C28x/Generic/CCS/$(basename $(<F)).d_raw" --obj_directory="uCOS/uCOS-II/Ports/C28x/Generic/CCS" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

uCOS/uCOS-II/Ports/C28x/Generic/CCS/%.obj: ../uCOS/uCOS-II/Ports/C28x/Generic/CCS/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -O3 --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/uCOS/config" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/uCOS/uC-CPU" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/uCOS/uC-CPU/C28x/CCS" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/uCOS/uC-LIB" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/uCOS/uCOS-II/Ports/C28x/Generic/CCS" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/uCOS/uCOS-II/Source" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/Hardware" --include_path="D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/Stdlib/include" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/Stdlib/src" --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower/User" --advice:performance=all -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="uCOS/uCOS-II/Ports/C28x/Generic/CCS/$(basename $(<F)).d_raw" --obj_directory="uCOS/uCOS-II/Ports/C28x/Generic/CCS" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


