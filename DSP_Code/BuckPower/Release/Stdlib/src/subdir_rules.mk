################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Stdlib/src/%.obj: ../Stdlib/src/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -O2 --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower" --include_path="F:/VoltronicPower/MyProjects/DigitalPower/Code/BuckPower/User" --include_path="F:/VoltronicPower/MyProjects/DigitalPower/Code/BuckPower/Hardware" --include_path="F:/VoltronicPower/MyProjects/DigitalPower/Code/common/include" --include_path="F:/VoltronicPower/MyProjects/DigitalPower/Code/headers/include" --include_path="D:/TI/ucos-ii/Micrium/Examples/TI/TMDSDOCK28335/OS2" --include_path="D:/TI/ucos-ii/Micrium/Examples/TI/TMDSDOCK28335/BSP/OS/uCOS-II" --include_path="D:/TI/ucos-ii/Micrium/Examples/TI/TMDSDOCK28335/BSP" --include_path="D:/TI/ucos-ii/Micrium/Software/uC-CPU/C28x/CCS" --include_path="D:/TI/ucos-ii/Micrium/Software/uC-CPU" --include_path="D:/TI/ucos-ii/Micrium/Software/uC-LIB" --include_path="D:/TI/ucos-ii/Micrium/Software/uCOS-II/Ports/C28x/Generic/CCS" --include_path="D:/TI/ucos-ii/Micrium/Software/uCOS-II/Source" --include_path="D:/TI/ucos-ii/Micrium/Software/uC-LIB" --include_path="D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="Stdlib/src/$(basename $(<F)).d_raw" --obj_directory="Stdlib/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Stdlib/src/%.obj: ../Stdlib/src/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 -O2 --include_path="F:/MyProjects/DigitalPower/DSP_Code/BuckPower" --include_path="F:/VoltronicPower/MyProjects/DigitalPower/Code/BuckPower/User" --include_path="F:/VoltronicPower/MyProjects/DigitalPower/Code/BuckPower/Hardware" --include_path="F:/VoltronicPower/MyProjects/DigitalPower/Code/common/include" --include_path="F:/VoltronicPower/MyProjects/DigitalPower/Code/headers/include" --include_path="D:/TI/ucos-ii/Micrium/Examples/TI/TMDSDOCK28335/OS2" --include_path="D:/TI/ucos-ii/Micrium/Examples/TI/TMDSDOCK28335/BSP/OS/uCOS-II" --include_path="D:/TI/ucos-ii/Micrium/Examples/TI/TMDSDOCK28335/BSP" --include_path="D:/TI/ucos-ii/Micrium/Software/uC-CPU/C28x/CCS" --include_path="D:/TI/ucos-ii/Micrium/Software/uC-CPU" --include_path="D:/TI/ucos-ii/Micrium/Software/uC-LIB" --include_path="D:/TI/ucos-ii/Micrium/Software/uCOS-II/Ports/C28x/Generic/CCS" --include_path="D:/TI/ucos-ii/Micrium/Software/uCOS-II/Source" --include_path="D:/TI/ucos-ii/Micrium/Software/uC-LIB" --include_path="D:/TI/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="Stdlib/src/$(basename $(<F)).d_raw" --obj_directory="Stdlib/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


