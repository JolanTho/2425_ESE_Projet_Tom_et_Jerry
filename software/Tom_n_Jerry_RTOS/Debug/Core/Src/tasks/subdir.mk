################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/tasks/edgeDetection.c \
../Core/Src/tasks/shell.c 

OBJS += \
./Core/Src/tasks/edgeDetection.o \
./Core/Src/tasks/shell.o 

C_DEPS += \
./Core/Src/tasks/edgeDetection.d \
./Core/Src/tasks/shell.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/tasks/%.o Core/Src/tasks/%.su Core/Src/tasks/%.cyclo: ../Core/Src/tasks/%.c Core/Src/tasks/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSBPD_PORT_COUNT=1 -D_RTOS -D_SRC -DUSBPDCORE_LIB_NO_PD -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../shell -I../USBPD/App -I../USBPD -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/ST/STM32_USBPD_Library/Core/inc -I../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"/Users/romingo/Desktop/ENSEA/3A/PROJET/2425_ESE_Projet_Tom_et_Jerry/software/Tom_n_Jerry_RTOS/shell" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-tasks

clean-Core-2f-Src-2f-tasks:
	-$(RM) ./Core/Src/tasks/edgeDetection.cyclo ./Core/Src/tasks/edgeDetection.d ./Core/Src/tasks/edgeDetection.o ./Core/Src/tasks/edgeDetection.su ./Core/Src/tasks/shell.cyclo ./Core/Src/tasks/shell.d ./Core/Src/tasks/shell.o ./Core/Src/tasks/shell.su

.PHONY: clean-Core-2f-Src-2f-tasks

