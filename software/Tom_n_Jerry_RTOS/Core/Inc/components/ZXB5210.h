/*
 * ZXB5210.h
 *
 *  Created on: Nov 20, 2024
 *      Author: romingo
 */

#ifndef INC_COMPONENTS_ZXB5210_H_
#define INC_COMPONENTS_ZXB5210_H_

#include <shell.h>
#include "tim.h"

typedef struct{
	uint32_t Tim_Channel;
	volatile uint32_t* CCR_Channel;
	uint32_t pulseGoal;
	int32_t offset;
}MDriver_Config_t;

typedef struct{
	TIM_HandleTypeDef* htim;
	MDriver_Config_t* FWD;
	MDriver_Config_t* REV;
}MDriver_t;

void ZXB5210_init(void);
void ZXB5210_deinit(void);
#ifdef notsmooting
void ZXB5210_speed_FWD(TIM_HandleTypeDef *htim ,uint8_t alpha);
void ZXB5210_speed_REV(TIM_HandleTypeDef *htim ,uint8_t alpha);
#endif
void ZXB5210_speed_FWD(MDriver_t* MDriver ,uint8_t alpha);
void ZXB5210_speed_REV(MDriver_t* MDriver ,uint8_t alpha);
void IT_ZXB5210_speed_UPDATE(MDriver_t* MDriver, MDriver_Config_t* SENS);
 void ZXB5210_calc_speed(MDriver_Config_t* MDriver_Config,uint32_t initial_Pulse);


#endif /* INC_COMPONENTS_ZXB5210_H_ */
