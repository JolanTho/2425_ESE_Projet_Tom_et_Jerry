/*
 * ZXB5210.c
 *
 *  Created on: Nov 20, 2024
 *      Author: romingo
 */
#include "components/ZXB5210.h"
#include "changeMode.h"
#include "callBack.h"

#define MAX_ALPHA_SPEED 30.0
#define V2
MDriver_t MDriver1;
MDriver_t MDriver2;

MDriver_Config_t MDriver1_FWD_Config;
MDriver_Config_t MDriver1_REV_Config;
MDriver_Config_t MDriver2_FWD_Config;
MDriver_Config_t MDriver2_REV_Config;

int isSpeedInit = 0;
int ANGLE_GLOBAL = -1;
extern int workMode;
extern int isSpeedActualisationIsBlocked;

void ZXB5210_init(void){
	debug(INFORMATION,"ZXB5210 - INIT");
	MDriver1_FWD_Config = (MDriver_Config_t){.Tim_Channel = TIM_CHANNEL_3,.CCR_Channel = &htim2.Instance->CCR3,.pulseGoal = 0, .offset=0}; //28 Surement inutlie si l'asservissement en vitesse fonctionne un jour
	MDriver1_REV_Config = (MDriver_Config_t){.Tim_Channel = TIM_CHANNEL_4,.CCR_Channel = &htim2.Instance->CCR4,.pulseGoal = 0, .offset=0}; //36
	MDriver2_FWD_Config = (MDriver_Config_t){.Tim_Channel = TIM_CHANNEL_3,.CCR_Channel = &htim3.Instance->CCR3,.pulseGoal = 0, .offset=0};
	MDriver2_REV_Config = (MDriver_Config_t){.Tim_Channel = TIM_CHANNEL_4,.CCR_Channel = &htim3.Instance->CCR4,.pulseGoal = 0, .offset=0};

	MDriver1 = (MDriver_t){
		.htim = &htim2,
				.FWD = &MDriver1_FWD_Config,
				.REV = &MDriver1_REV_Config,
				.ACTIVE = NULL,
	};
	MDriver2 = (MDriver_t){
		.htim = &htim3,
				.FWD = &MDriver2_FWD_Config,
				.REV = &MDriver2_REV_Config,
				.ACTIVE = NULL,
	};
	HAL_TIM_PWM_Start(MDriver1.htim,MDriver1.FWD->Tim_Channel) == HAL_OK ? debug(START,"DRIVER1 - PWM CHANNEL FWD"):(void)0;
	HAL_TIM_PWM_Start(MDriver1.htim,MDriver1.REV->Tim_Channel) == HAL_OK ? debug(START,"DRIVER1 - PWM CHANNEL REV"):(void)0;

	HAL_TIM_PWM_Start(MDriver2.htim,MDriver2.FWD->Tim_Channel) == HAL_OK ? debug(START,"DRIVER2 - PWM CHANNEL FWD"):(void)0;
	HAL_TIM_PWM_Start(MDriver2.htim,MDriver2.REV->Tim_Channel) == HAL_OK ? debug(START,"DRIVER2 - PWM CHANNEL REV"):(void)0;

}
// Tout les channels ne sont pas cabler pareil

void ZXB5210_deinit(void){
	HAL_TIM_PWM_Stop(MDriver1.htim,MDriver1.FWD->Tim_Channel) == HAL_OK ? debug(STOP,"DRIVER1 - PWM CHANNEL FWD"):(void)0;
	HAL_TIM_PWM_Stop(MDriver1.htim,MDriver1.REV->Tim_Channel) == HAL_OK ? debug(STOP,"DRIVER1 - PWM CHANNEL REV"):(void)0;

	HAL_TIM_PWM_Stop(MDriver2.htim,MDriver2.FWD->Tim_Channel) == HAL_OK ? debug(STOP,"DRIVER2 - PWM CHANNEL FWD"):(void)0;
	HAL_TIM_PWM_Stop(MDriver2.htim,MDriver2.REV->Tim_Channel) == HAL_OK ? debug(STOP,"DRIVER2 - PWM CHANNEL REV"):(void)0;
}

void ZXB5210_speed_FWD(MDriver_t* MDriver ,uint8_t alpha){
	MDriver->FWD->pulseGoal=(MDriver->htim->Instance->ARR * alpha)/100;
	MDriver->ACTIVE = MDriver->FWD;

	__HAL_TIM_SET_COMPARE(MDriver->htim,MDriver->REV->Tim_Channel,0);
	MDriver->REV->pulseGoal = 0;
}
void ZXB5210_speed_REV(MDriver_t *MDriver ,uint8_t alpha)
{	//ZXB5210_calc_speed(MDriver->REV,(MDriver->htim->Instance->ARR * alpha)/100);
	MDriver->REV->pulseGoal=(MDriver->htim->Instance->ARR * alpha)/100;
	MDriver->ACTIVE = MDriver->REV;

	__HAL_TIM_SET_COMPARE(MDriver->htim,MDriver->FWD->Tim_Channel,0);
	MDriver->FWD->pulseGoal = 0;
}

void IT_ZXB5210_speed_UPDATE(MDriver_t* MDriver, MDriver_Config_t* SENS){
	uint32_t pulse = *(SENS->CCR_Channel);
	pulse < SENS->pulseGoal ? // Si le pulse est trop petit
			pulse ++ :
			pulse --;
	__HAL_TIM_SET_COMPARE(MDriver->htim,SENS->Tim_Channel,pulse);
	if (SENS->pulseGoal == pulse){ // Si on a atteint l'objectif de pulse
		MDriver->ACTIVE = NULL;
	}
}

void ZXB5210_calc_speed(MDriver_Config_t* MDriver_Config,uint32_t initial_Pulse){
	uint32_t pulseActu = initial_Pulse - MDriver_Config->offset;
	MDriver_Config->pulseGoal =  pulseActu >= 100 ? pulseActu : 100;
}

void ZXB5210_angle(int angle){
	uint8_t alpha_1;
	uint8_t alpha_2;
	if(angle <0){return;}
	if(!workMode){
		angle = ((angle + 180) % 360 + 360) % 360;
	}
	ANGLE_GLOBAL = angle;
#ifdef V2
	if (!isSpeedActualisationIsBlocked){
		if(angle <=90){
			alpha_1 =  MAX_ALPHA_SPEED/2 - MAX_ALPHA_SPEED/180.0 * angle ;
			alpha_2 = MAX_ALPHA_SPEED/2 + MAX_ALPHA_SPEED/180.0 * angle;

			alpha_1 = alpha_1 > MAX_ALPHA_SPEED ? MAX_ALPHA_SPEED : alpha_1;
			alpha_2 = alpha_2 > MAX_ALPHA_SPEED ? MAX_ALPHA_SPEED : alpha_2 ;
			ZXB5210_speed_FWD(&MDriver1 , alpha_1);
			ZXB5210_speed_FWD(&MDriver2 , alpha_2);
		}
		else if(angle <=270){
			alpha_1 =  MAX_ALPHA_SPEED/180.0 * (angle -90) ;
			alpha_2 = MAX_ALPHA_SPEED -  MAX_ALPHA_SPEED/180.0 * (angle - 90);

			alpha_1 = alpha_1 > MAX_ALPHA_SPEED ? MAX_ALPHA_SPEED : alpha_1;
			alpha_2 = alpha_2 > MAX_ALPHA_SPEED ? MAX_ALPHA_SPEED : alpha_2 ;
			ZXB5210_speed_REV(&MDriver1 , alpha_1);
			ZXB5210_speed_REV(&MDriver2 , alpha_2);
		}
		else if(angle <=360){
			alpha_2 = MAX_ALPHA_SPEED/180.0 * (angle-270);
			alpha_1 = MAX_ALPHA_SPEED -  MAX_ALPHA_SPEED/180.0 * (angle - 270);

			alpha_1 = alpha_1 > MAX_ALPHA_SPEED ? MAX_ALPHA_SPEED : alpha_1;
			alpha_2 = alpha_2 > MAX_ALPHA_SPEED ? MAX_ALPHA_SPEED : alpha_2 ;
			ZXB5210_speed_FWD(&MDriver1 , alpha_1);
			ZXB5210_speed_FWD(&MDriver2 , alpha_2);
		}
	}
#endif
}
