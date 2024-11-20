/*
 * callBack.h
 *
 *  Created on: Nov 19, 2024
 *      Author: romingo
 */

#ifndef CALLBACK_H_
#define CALLBACK_H_

#include "tim.h"
#include "shellv2.h"
#include "components/ADXL343.h"

void CUSTOM_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif /* CALLBACK_H_ */
