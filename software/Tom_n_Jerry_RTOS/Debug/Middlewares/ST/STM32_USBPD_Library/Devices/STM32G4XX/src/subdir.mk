################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_cad_hw_if.c \
../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_hw.c \
../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_hw_if_it.c \
../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_phy.c \
../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_phy_hw_if.c \
../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_pwr_hw_if.c \
../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_timersserver.c 

OBJS += \
./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_cad_hw_if.o \
./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_hw.o \
./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_hw_if_it.o \
./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_phy.o \
./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_phy_hw_if.o \
./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_pwr_hw_if.o \
./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_timersserver.o 

C_DEPS += \
./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_cad_hw_if.d \
./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_hw.d \
./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_hw_if_it.d \
./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_phy.d \
./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_phy_hw_if.d \
./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_pwr_hw_if.d \
./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_timersserver.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/%.o Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/%.su Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/%.cyclo: ../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/%.c Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSBPD_PORT_COUNT=1 -D_RTOS -DUSE_HAL_DRIVER -DSTM32G431xx -D_SNK -DUSBPDCORE_LIB_PD3_CONFIG_MINSNK -c -I../Core/Inc -I../USBPD/App -I../USBPD -I"/Volumes/jolanT/Ensea/3A/Projet/Github_qui_marche/2425_ESE_Projet_Tom_et_Jerry/software/Tom_n_Jerry_RTOS/asserv" -I"/Volumes/jolanT/Ensea/3A/Projet/Github_qui_marche/2425_ESE_Projet_Tom_et_Jerry/software/Tom_n_Jerry_RTOS/callBack" -I"/Volumes/jolanT/Ensea/3A/Projet/Github_qui_marche/2425_ESE_Projet_Tom_et_Jerry/software/Tom_n_Jerry_RTOS/shell" -I"/Volumes/jolanT/Ensea/3A/Projet/Github_qui_marche/2425_ESE_Projet_Tom_et_Jerry/software/Tom_n_Jerry_RTOS/changeMode" -I"/Volumes/jolanT/Ensea/3A/Projet/Github_qui_marche/2425_ESE_Projet_Tom_et_Jerry/software/Tom_n_Jerry_RTOS/Lidar" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/ST/STM32_USBPD_Library/Core/inc -I../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-STM32_USBPD_Library-2f-Devices-2f-STM32G4XX-2f-src

clean-Middlewares-2f-ST-2f-STM32_USBPD_Library-2f-Devices-2f-STM32G4XX-2f-src:
	-$(RM) ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_cad_hw_if.cyclo ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_cad_hw_if.d ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_cad_hw_if.o ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_cad_hw_if.su ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_hw.cyclo ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_hw.d ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_hw.o ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_hw.su ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_hw_if_it.cyclo ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_hw_if_it.d ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_hw_if_it.o ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_hw_if_it.su ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_phy.cyclo ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_phy.d ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_phy.o ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_phy.su ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_phy_hw_if.cyclo ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_phy_hw_if.d ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_phy_hw_if.o ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_phy_hw_if.su ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_pwr_hw_if.cyclo ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_pwr_hw_if.d ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_pwr_hw_if.o ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_pwr_hw_if.su ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_timersserver.cyclo ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_timersserver.d ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_timersserver.o ./Middlewares/ST/STM32_USBPD_Library/Devices/STM32G4XX/src/usbpd_timersserver.su

.PHONY: clean-Middlewares-2f-ST-2f-STM32_USBPD_Library-2f-Devices-2f-STM32G4XX-2f-src

