################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../init/init.c 

OBJS += \
./init/init.o 

C_DEPS += \
./init/init.d 


# Each subdirectory must supply rules for building sources it contributes
init/%.o init/%.su init/%.cyclo: ../init/%.c init/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSBPD_PORT_COUNT=1 -D_RTOS -DUSE_HAL_DRIVER -DSTM32G431xx -D_SNK -DUSBPDCORE_LIB_PD3_CONFIG_MINSNK -c -I../Core/Inc -I../USBPD/App -I../USBPD -I"/Users/romingo/Desktop/ENSEA/3A/PROJET/2425_ESE_Projet_Tom_et_Jerry/software/Tom_n_Jerry_RTOS/asserv" -I"/Users/romingo/Desktop/ENSEA/3A/PROJET/2425_ESE_Projet_Tom_et_Jerry/software/Tom_n_Jerry_RTOS/callBack" -I"/Users/romingo/Desktop/ENSEA/3A/PROJET/2425_ESE_Projet_Tom_et_Jerry/software/Tom_n_Jerry_RTOS/shell" -I"/Users/romingo/Desktop/ENSEA/3A/PROJET/2425_ESE_Projet_Tom_et_Jerry/software/Tom_n_Jerry_RTOS/changeMode" -I"/Users/romingo/Desktop/ENSEA/3A/PROJET/2425_ESE_Projet_Tom_et_Jerry/software/Tom_n_Jerry_RTOS/Lidar" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/ST/STM32_USBPD_Library/Core/inc -I../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"/Users/romingo/Desktop/ENSEA/3A/PROJET/2425_ESE_Projet_Tom_et_Jerry/software/Tom_n_Jerry_RTOS/init" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-init

clean-init:
	-$(RM) ./init/init.cyclo ./init/init.d ./init/init.o ./init/init.su

.PHONY: clean-init

