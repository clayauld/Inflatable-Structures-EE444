################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
DecrementVcore.obj: ../DecrementVcore.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.2.LTS/bin/cl430" -vmspx --data_model=restricted -O1 --opt_for_speed=4 --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Clayton Auld/Documents/TI_Workspace/EE444_main" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.2.LTS/include" --advice:power="all" --define=__MSP430F5529__ -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="DecrementVcore.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

IncrementVcore.obj: ../IncrementVcore.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.2.LTS/bin/cl430" -vmspx --data_model=restricted -O1 --opt_for_speed=4 --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Clayton Auld/Documents/TI_Workspace/EE444_main" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.2.LTS/include" --advice:power="all" --define=__MSP430F5529__ -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="IncrementVcore.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

cmd_functions.obj: ../cmd_functions.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.2.LTS/bin/cl430" -vmspx --data_model=restricted -O1 --opt_for_speed=4 --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Clayton Auld/Documents/TI_Workspace/EE444_main" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.2.LTS/include" --advice:power="all" --define=__MSP430F5529__ -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="cmd_functions.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

init_setups.obj: ../init_setups.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.2.LTS/bin/cl430" -vmspx --data_model=restricted -O1 --opt_for_speed=4 --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Clayton Auld/Documents/TI_Workspace/EE444_main" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.2.LTS/include" --advice:power="all" --define=__MSP430F5529__ -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="init_setups.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.2.LTS/bin/cl430" -vmspx --data_model=restricted -O1 --opt_for_speed=4 --use_hw_mpy=F5 --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/Clayton Auld/Documents/TI_Workspace/EE444_main" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.2.LTS/include" --advice:power="all" --define=__MSP430F5529__ -g --printf_support=full --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


