/*
 * TCA9555.h
 *
 *  Created on: Nov 19, 2024
 *      Author: romingo
 */

#ifndef INC_COMPONENTS_TCA9555_H_
#define INC_COMPONENTS_TCA9555_H_

#include <shell.h>
#include "stdio.h"
#include "i2c.h"
#include "cmsis_os.h"

typedef struct{
	char* name;
	uint8_t reg;
}GPIOExpanderRegister_t;

void TCA9555_init(void);
HAL_StatusTypeDef TCA9555_WriteRegister(uint8_t reg, uint8_t data);
HAL_StatusTypeDef TCA9555_ReadRegister(uint8_t reg, uint8_t* recptData, uint8_t lenData);

#endif /* INC_COMPONENTS_TCA9555_H_ */
