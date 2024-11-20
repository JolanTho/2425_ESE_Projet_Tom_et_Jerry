/*
 * LP5812.h
 *
 *  Created on: Nov 13, 2024
 *      Author: romingo
 */

#ifndef INC_LP5812_H_
#define INC_LP5812_H_

#include "stdio.h"
#include "i2c.h"
#include"cmsis_os.h"
#include "shellv2.h"

typedef struct{

}led_params;
void LP5812_Init(void);

HAL_StatusTypeDef LP5812_WriteRegister(uint16_t reg_addr, uint8_t data);
HAL_StatusTypeDef LP5812_ReadRegister(uint16_t reg_addr, uint8_t *pData);

#endif /* INC_LP5812_H_ */
