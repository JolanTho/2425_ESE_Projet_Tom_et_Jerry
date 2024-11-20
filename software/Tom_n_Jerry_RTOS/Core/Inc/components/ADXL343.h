/*
 * ADXL343.h
 *
 *  Created on: Nov 18, 2024
 *      Author: romingo
 */

#ifndef INC_COMPONENTS_ADXL343_H_
#define INC_COMPONENTS_ADXL343_H_

#include "stdio.h"
#include "i2c.h"
#include "tim.h"
#include "cmsis_os.h"
#include "shellv2.h"
#include "math.h"

typedef struct{
	int16_t X;
	int16_t Y;
	int16_t Z;
}XYZ_t;

typedef struct{
	char *name;
	uint8_t reg;
}IMURegister_t;


void ADXL343_init(void);
HAL_StatusTypeDef ADXL343_WriteRegister(uint8_t reg, uint8_t data);
HAL_StatusTypeDef ADXL343_ReadRegister(uint8_t reg, uint8_t* pData, uint8_t lenData);
XYZ_t ADXL343_getAcc(void);


#endif /* INC_COMPONENTS_ADXL343_H_ */



