/*
 * ZXB5210.h
 *
 *  Created on: Nov 20, 2024
 *      Author: romingo
 */

#ifndef INC_COMPONENTS_ZXB5210_H_
#define INC_COMPONENTS_ZXB5210_H_

#include "tim.h"
#include "shellv2.h"

void ZXB5210_init(void);
void ZXB5210_deinit(void);
void ZXB5210_speed_FWD(TIM_HandleTypeDef *htim ,uint8_t alpha);
void ZXB5210_speed_REV(TIM_HandleTypeDef *htim ,uint8_t alpha);
#endif /* INC_COMPONENTS_ZXB5210_H_ */
