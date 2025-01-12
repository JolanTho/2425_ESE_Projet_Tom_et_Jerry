/*
 * LP5812.h
 *
 *  Created on: Nov 13, 2024
 *      Author: romingo
 */

#ifndef INC_LP5812_H_
#define INC_LP5812_H_

#include <shell.h>
#include "stdio.h"
#include "i2c.h"
#include"cmsis_os.h"


typedef struct{
	uint16_t R;
	uint16_t G;
	uint16_t B;
}ConfigLED_t;

typedef struct{
	ConfigLED_t L1;
	ConfigLED_t L2;
	ConfigLED_t L3;
	ConfigLED_t L4;
}LED_t;
void LP5812_Init(void);

HAL_StatusTypeDef LP5812_WriteRegister(uint16_t reg_addr, uint8_t data);
HAL_StatusTypeDef LP5812_ReadRegister(uint16_t reg_addr, uint8_t *pData);
void LED_update(uint16_t regRGB, uint8_t RGB);
void chenillard_RGB(void);
#endif /* INC_LP5812_H_ */
