/*
 * ZXB5210.c
 *
 *  Created on: Nov 20, 2024
 *      Author: romingo
 */
#include "components/ZXB5210.h"

void ZXB5210_init(void){
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3) == HAL_OK ? debug(START,"TIM2 - PWM CHANNEL 3"):(void)0;
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4) == HAL_OK ? debug(START,"TIM2 - PWM CHANNEL 4"):(void)0;

	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3) == HAL_OK ? debug(START,"TIM3 - PWM CHANNEL 3"):(void)0;
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4) == HAL_OK ? debug(START,"TIM3 - PWM CHANNEL 4"):(void)0;
}
// Tout les channels ne sont pas cabler pareil

void ZXB5210_deinit(void){
	HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_3) == HAL_OK ? debug(STOP,"TIM2 - PWM CHANNEL 3"):(void)0;
	HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_4) == HAL_OK ? debug(STOP,"TIM2 - PWM CHANNEL 4"):(void)0;

	HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_3) == HAL_OK ? debug(STOP,"TIM3 - PWM CHANNEL 3"):(void)0;
	HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_4) == HAL_OK ? debug(STOP,"TIM3 - PWM CHANNEL 4"):(void)0;
}
void ZXB5210_speed_FWD(TIM_HandleTypeDef *htim ,uint8_t alpha){
	uint32_t pulse = (htim->Instance->ARR * alpha)/100;
	__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_3,pulse);
}
void ZXB5210_speed_REV(TIM_HandleTypeDef *htim ,uint8_t alpha){
	uint32_t pulse = (htim->Instance->ARR * alpha)/100;
	__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_4,pulse);
}
