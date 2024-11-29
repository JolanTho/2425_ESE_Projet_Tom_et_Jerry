/*
 * TCA9555.c
 *
 *  Created on: Nov 19, 2024
 *      Author: romingo
 *
 *      Datasheet 		: https://www.ti.com/lit/ds/symlink/tca9555.pdf?ts=1731986124746&ref_url=https%253A%252F%252Feu.mouser.com%252F
 *
 */

#include "components/TCA9555.h"
#define TCA9555_ADRR 	0x20 // A0=A1=A2 = GND
#define I2C_MAIN hi2c3

GPIOExpanderRegister_t GPIOExpRegister[]={
		{"INPUT PORT 0",	0x00},	//Only read
		{"INPUT PORT 1",	0x01},	//Only read
		{"OUTUPUT PORT 0",	0x02},	//Only read
		{"OUTUPUT PORT 1",	0x03}, 	//Only read
		{"POLARITY INVERSION PORT 0",	0x04},
		{"POLARITY INVERSION PORT 1",	0x05},
		{"CONFIGURATION PORT 0",	0x06},
		{"CONFIGURATION PORT 1",	0x07},
};
void TCA9555_init(void){
	debug(INFORMATION,"TCA9555 - INIT");

	TCA9555_WriteRegister(0x06, 0b11111111)!=HAL_OK ? debug(D_ERROR,"I2C TRANSMIT in WriteREGISTER"):(void)0;
	TCA9555_WriteRegister(0x07, 0b11111111)!=HAL_OK ? debug(D_ERROR,"I2C TRANSMIT in WriteREGISTER"):(void)0;

	uint8_t startReg = 0x00;
	uint8_t endReg = 0x07;
	for (int i = 0; i <= endReg-startReg; i++) {
		uint8_t ret=0;
		TCA9555_ReadRegister(GPIOExpRegister[i].reg, &ret,1);
		printf("READ - 0x%02X (%s): 0x%02X\r\n", GPIOExpRegister[i].reg,GPIOExpRegister[i].name, ret);
	}
}

HAL_StatusTypeDef TCA9555_WriteRegister(uint8_t reg, uint8_t data){
	uint8_t buffer[2]={reg,data};
	return HAL_I2C_Master_Transmit(&I2C_MAIN, (uint16_t)(TCA9555_ADRR<<1), buffer, 2 ,HAL_MAX_DELAY);
}
HAL_StatusTypeDef TCA9555_ReadRegister(uint8_t reg, uint8_t* recptData, uint8_t lenData){
	HAL_I2C_Master_Transmit(&I2C_MAIN, (uint16_t)(TCA9555_ADRR<<1), &reg, 1 ,HAL_MAX_DELAY) !=HAL_OK ? debug(D_ERROR,"I2C TRANSMIT in ReadREGISTER"):(void)0;
	return HAL_I2C_Master_Receive(&I2C_MAIN, (uint16_t)(TCA9555_ADRR<<1), recptData, lenData ,HAL_MAX_DELAY);
}
