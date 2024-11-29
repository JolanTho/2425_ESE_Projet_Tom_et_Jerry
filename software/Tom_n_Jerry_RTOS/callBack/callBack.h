/*
 * callBack.h
 *
 *  Created on: Nov 19, 2024
 *      Author: romingo
 */

#ifndef CALLBACK_H_
#define CALLBACK_H_

#include <shell.h>
#include "tim.h"
#include "components/ADXL343.h"

void CUSTOM_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif /* CALLBACK_H_ */
