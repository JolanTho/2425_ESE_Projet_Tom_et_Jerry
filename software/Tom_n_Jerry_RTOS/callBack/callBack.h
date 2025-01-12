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
#include "usart.h"
#include "components/ADXL343.h"
#include "components/LP5812.h"
#include "components/ZXB5210.h"

void CUSTOM_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void CUSTOM_EXTI_IRQHandler(void);

#endif /* CALLBACK_H_ */
