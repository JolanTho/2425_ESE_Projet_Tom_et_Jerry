################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/gpio_expander/tca9555_driver.c 

OBJS += \
./Core/Src/gpio_expander/tca9555_driver.o 

C_DEPS += \
./Core/Src/gpio_expander/tca9555_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/gpio_expander/%.o Core/Src/gpio_expander/%.su Core/Src/gpio_expander/%.cyclo: ../Core/Src/gpio_expander/%.c Core/Src/gpio_expander/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-gpio_expander

clean-Core-2f-Src-2f-gpio_expander:
	-$(RM) ./Core/Src/gpio_expander/tca9555_driver.cyclo ./Core/Src/gpio_expander/tca9555_driver.d ./Core/Src/gpio_expander/tca9555_driver.o ./Core/Src/gpio_expander/tca9555_driver.su

.PHONY: clean-Core-2f-Src-2f-gpio_expander

