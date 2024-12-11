################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/components/ADXL343.c \
../Core/Src/components/LP5812.c \
../Core/Src/components/TCA9555.c \
../Core/Src/components/ZXB5210.c 

OBJS += \
./Core/Src/components/ADXL343.o \
./Core/Src/components/LP5812.o \
./Core/Src/components/TCA9555.o \
./Core/Src/components/ZXB5210.o 

C_DEPS += \
./Core/Src/components/ADXL343.d \
./Core/Src/components/LP5812.d \
./Core/Src/components/TCA9555.d \
./Core/Src/components/ZXB5210.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/components/%.o Core/Src/components/%.su Core/Src/components/%.cyclo: ../Core/Src/components/%.c Core/Src/components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSBPD_PORT_COUNT=1 -D_RTOS -DUSE_HAL_DRIVER -DSTM32G431xx -D_SNK -DUSBPDCORE_LIB_PD3_CONFIG_MINSNK -c -I../Core/Inc -I../USBPD/App -I../USBPD -I"/Volumes/jolanT/Ensea/3A/Projet/Github_qui_marche/2425_ESE_Projet_Tom_et_Jerry/software/Tom_n_Jerry_RTOS/asserv" -I"/Volumes/jolanT/Ensea/3A/Projet/Github_qui_marche/2425_ESE_Projet_Tom_et_Jerry/software/Tom_n_Jerry_RTOS/callBack" -I"/Volumes/jolanT/Ensea/3A/Projet/Github_qui_marche/2425_ESE_Projet_Tom_et_Jerry/software/Tom_n_Jerry_RTOS/shell" -I"/Volumes/jolanT/Ensea/3A/Projet/Github_qui_marche/2425_ESE_Projet_Tom_et_Jerry/software/Tom_n_Jerry_RTOS/changeMode" -I"/Volumes/jolanT/Ensea/3A/Projet/Github_qui_marche/2425_ESE_Projet_Tom_et_Jerry/software/Tom_n_Jerry_RTOS/Lidar" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/ST/STM32_USBPD_Library/Core/inc -I../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-components

clean-Core-2f-Src-2f-components:
	-$(RM) ./Core/Src/components/ADXL343.cyclo ./Core/Src/components/ADXL343.d ./Core/Src/components/ADXL343.o ./Core/Src/components/ADXL343.su ./Core/Src/components/LP5812.cyclo ./Core/Src/components/LP5812.d ./Core/Src/components/LP5812.o ./Core/Src/components/LP5812.su ./Core/Src/components/TCA9555.cyclo ./Core/Src/components/TCA9555.d ./Core/Src/components/TCA9555.o ./Core/Src/components/TCA9555.su ./Core/Src/components/ZXB5210.cyclo ./Core/Src/components/ZXB5210.d ./Core/Src/components/ZXB5210.o ./Core/Src/components/ZXB5210.su

.PHONY: clean-Core-2f-Src-2f-components

