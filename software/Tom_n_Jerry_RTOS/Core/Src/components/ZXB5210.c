/*
 * ZXB5210.c
 *
 *  Created on: Nov 20, 2024
 *      Author: romingo
 */
#include "components/ZXB5210.h"
MDriver_t MDriver1;
MDriver_t MDriver2;
int isSpeedInit = 0;

void ZXB5210_init(void){
	debug(INFORMATION,"ZXB5210 - INIT");
	MDriver1 = (MDriver_t){
		.htim = &htim2,
				.FWD = (MDriver_Config_t){.Tim_Channel = TIM_CHANNEL_3,.CCR_Channel = &htim2.Instance->CCR3,.pulseGoal = 0,},
				.REV = (MDriver_Config_t){.Tim_Channel = TIM_CHANNEL_4,.CCR_Channel = &htim2.Instance->CCR4,.pulseGoal = 0,},
	};
	MDriver2 = (MDriver_t){
		.htim = &htim3,
				.FWD = (MDriver_Config_t){.Tim_Channel = TIM_CHANNEL_4,.CCR_Channel = &htim3.Instance->CCR4,.pulseGoal = 0,},
				.REV = (MDriver_Config_t){.Tim_Channel = TIM_CHANNEL_3,.CCR_Channel = &htim3.Instance->CCR3,.pulseGoal = 0,},
	};
	HAL_TIM_PWM_Start(MDriver1.htim,MDriver1.FWD.Tim_Channel) == HAL_OK ? debug(START,"DRIVER1 - PWM CHANNEL FWD"):(void)0;
	HAL_TIM_PWM_Start(MDriver1.htim,MDriver1.REV.Tim_Channel) == HAL_OK ? debug(START,"DRIVER1 - PWM CHANNEL REV"):(void)0;

	HAL_TIM_PWM_Start(MDriver2.htim,MDriver2.FWD.Tim_Channel) == HAL_OK ? debug(START,"DRIVER2 - PWM CHANNEL FWD"):(void)0;
	HAL_TIM_PWM_Start(MDriver2.htim,MDriver2.REV.Tim_Channel) == HAL_OK ? debug(START,"DRIVER2 - PWM CHANNEL REV"):(void)0;

	isSpeedInit = 1;
}
// Tout les channels ne sont pas cabler pareil

void ZXB5210_deinit(void){
	HAL_TIM_PWM_Stop(MDriver1.htim,MDriver1.FWD.Tim_Channel) == HAL_OK ? debug(STOP,"DRIVER1 - PWM CHANNEL FWD"):(void)0;
	HAL_TIM_PWM_Stop(MDriver1.htim,MDriver1.REV.Tim_Channel) == HAL_OK ? debug(STOP,"DRIVER1 - PWM CHANNEL REV"):(void)0;

	HAL_TIM_PWM_Stop(MDriver2.htim,MDriver2.FWD.Tim_Channel) == HAL_OK ? debug(STOP,"DRIVER2 - PWM CHANNEL FWD"):(void)0;
	HAL_TIM_PWM_Stop(MDriver2.htim,MDriver2.REV.Tim_Channel) == HAL_OK ? debug(STOP,"DRIVER2 - PWM CHANNEL REV"):(void)0;
}


#ifdef notsmoothing
void ZXB5210_speed_FWD(TIM_HandleTypeDef *htim ,uint8_t alpha){
	uint32_t pulse = (htim->Instance->ARR * alpha)/100;
	__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_3,pulse);
}
void ZXB5210_speed_REV(TIM_HandleTypeDef *htim ,uint8_t alpha){
	uint32_t pulse = (htim->Instance->ARR * alpha)/100;
	__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_4,pulse);
}
#endif

void ZXB5210_speed_FWD(MDriver_t* MDriver ,uint8_t alpha){
	MDriver->FWD.pulseGoal = (MDriver->htim->Instance->ARR * alpha)/100;
	MDriver->REV.pulseGoal = 0;
}
void ZXB5210_speed_REV(MDriver_t *MDriver ,uint8_t alpha){
	MDriver->REV.pulseGoal = (MDriver->htim->Instance->ARR * alpha)/100;
	MDriver->FWD.pulseGoal = 0;
}
void IT_ZXB5210_speed_UPDATE(MDriver_t* MDriver, MDriver_Config_t* SENS){
	uint32_t pulse = *(SENS->CCR_Channel);
	pulse < SENS->pulseGoal ? // Si le pulse est trop petit
			pulse ++ :
			pulse --;
	__HAL_TIM_SET_COMPARE(MDriver->htim,SENS->Tim_Channel,pulse);
}



