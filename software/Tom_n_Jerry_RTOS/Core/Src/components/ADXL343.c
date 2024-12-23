/*
 * ADXL343.c
 *
 *  Created on: Nov 18, 2024
 *      Author: romingo
 *
 *		Datasheet 		: https://www.analog.com/media/en/technical-documentation/data-sheets/adxl343.pdf
 *
 */

#include "components/ADXL343.h"

#define ADXL_ADRR 		0x53 //ALT ADRESS IS CONNECTED TO THE GROUND
#define ADXL_WRITE_REG 	0xA6
#define ADXL_READ_REG 	0xA7
#define IMU_SAMPLE_0G	10

#define SCALE_FACTOR_OFFSET_REG 15.6 // mg/LSB --> Chatgpt
#define SENSITIVITY_FULL_RES 256     // LSB/g 	-->
#define SCALE_FACTOR_LSB 3.9         // mg/LSB -->

#define ADXL_SENSIBILITY 4 			//mg/LSB
#define MM_S2_SEUIL 40 //mm.s-2
#define I2C_IMU hi2c1

IMURegister_t IMURegister[]={
		{"DEVICE_ID", 	0x00},
		{"THRESH_TAP",  0x01D},
		{"OFSX",  0x1E},
		{"OFSY",  0x1F},
		{"OFSZ",  0x20},
		{"DUR",  0x21},
		{"LATENT",  0x22},
		{"WINDOW",  0x23},
		{"THRESH_ACT",  0x24},
		{"THRESH_INACT",	0x25},
		{"TIME_INACT", 	0x26},
		{"ACT_INACT_CTL",  0x27},
		{"THRESH_FF",  0x28},
		{"TIME_FF",  0x29},
		{"TAP_AXES",  0x2A},
		{"ACT_TAP_STATUS",  0x2B},
		{"BW_RATE",  0x2C},
		{"POWER_CTL",  0x2D},
		{"INT_ENABLE",  0x2E},
		{"INT_MAP",  0x2F},
		{"INT_SOURCE",  0x30},
		{"DATA_FORMAT", 0x31},
		{"DATAX0",  0x32},
		{"DATAX1",  0x33},
		{"DATAY0",  0x34},
		{"DATAY1",  0x35},
		{"DATAZ0",  0x36},
		{"DATAZ1",  0x37},
		{"FIFO_CTL",	0x38},
		{"FIFO_SATUS",	0x39}
};


void ADXL343_init(void){
	debug(INFORMATION,"ADXL343 - INIT");
	uint8_t ret=0;
	ADXL343_ReadRegister(0x00, &ret,1)!=HAL_OK ? debug(D_ERROR,"I2C READ in INIT"):(void)0;
	printf("READ - deviceID: 0x%02X\r\n",ret);

	uint8_t startReg = 0x1D;
	uint8_t endReg = 0x39;
	for (int i = 1; i <= endReg-startReg+1; i++) {
		uint8_t ret=0;
		ADXL343_ReadRegister(IMURegister[i].reg, &ret,1);
		printf("READ - 0x%02X (%s): 0x%02X\r\n", IMURegister[i].reg,IMURegister[i].name, ret);
	}

	/*	0x1D-TRESHS_TAP		|	62.5 mg/LSB
	 *	0xa0= 10g
	 */
	ADXL343_WriteRegister(0x1D, 0x10)!=HAL_OK ? debug(D_ERROR,"I2C TRANSMIT in INIT"):(void)0;
	ADXL343_ReadRegister(0x1D, &ret,1)!=HAL_OK ? debug(D_ERROR,"I2C READ in INIT"):(void)0;
	//printf("WRITE/READ - TRESHS_TAP: 0x%02X\r\n",ret);
	/* 	0x21-DUR	|	625 µs/LSB
	 *	0xa0 : 100ms
	 */
	ADXL343_WriteRegister(0x21, 0xFF)!=HAL_OK ? debug(D_ERROR,"I2C TRANSMIT in INIT"):(void)0;
	ADXL343_ReadRegister(0x21, &ret,1)!=HAL_OK ? debug(D_ERROR,"I2C READ in INIT"):(void)0;
	//printf("WRITE/READ - DUR: 0x%02X\r\n",ret);
	/* 	0x22 - LATENT	|	 1.25 ms/LSB
	 *	0xFF : 378.75ms
	 */
	ADXL343_WriteRegister(0x22, 0xFF)!=HAL_OK ? debug(D_ERROR,"I2C TRANSMIT in INIT"):(void)0;
	ADXL343_ReadRegister(0x22, &ret,1)!=HAL_OK ? debug(D_ERROR,"I2C READ in INIT"):(void)0;
	//printf("WRITE/READ - DUR: 0x%02X\r\n",ret);

	/*	0x2A-TAP_AXES
	 *	D3		|SUPRESS
	 * 	D2		|TAP_X
	 * 	D1		|TAP_Y
	 * 	D0		|TAP_Z
	 */
	ADXL343_WriteRegister(0x2A, 0b110)!=HAL_OK ? debug(D_ERROR,"I2C TRANSMIT in INIT"):(void)0;
	ADXL343_ReadRegister(0x2A, &ret,1)!=HAL_OK ? debug(D_ERROR,"I2C READ in INIT"):(void)0;
	//printf("WRITE/READ - TAP_AXES: 0x%02X\r\n",ret);
	/*	0x2E-INT_ENABLE
	 * 	D6 		| SINGLE_TAP
	 */
	ADXL343_WriteRegister(0x2E, 0b1<<6)!=HAL_OK ? debug(D_ERROR,"I2C TRANSMIT in INIT"):(void)0;
	ADXL343_ReadRegister(0x2E, &ret,1)!=HAL_OK ? debug(D_ERROR,"I2C READ in INIT"):(void)0;
	//printf("WRITE/READ - INT_ENABLE: 0x%02X\r\n",ret);
	/*	0x2F-INT_MAP
	 * 	D6 		| SINGLE_TAP =1 : vers INT2
	 */
	ADXL343_WriteRegister(0x2F, 0b1<<6)!=HAL_OK ? debug(D_ERROR,"I2C TRANSMIT in INIT"):(void)0;
	ADXL343_ReadRegister(0x2F, &ret,1)!=HAL_OK ? debug(D_ERROR,"I2C READ in INIT"):(void)0;
	//printf("WRITE/READ - INT_ENABLE: 0x%02X\r\n",ret);

	/*	0x27—ACT_INACT_CTL
	 * 	D7		| ACT ac/dc
	 * 	D6		| ACT_X enable
	 * 	D5		| ACT_Y enable
	 * 	D4		| ACT_Z enable
	 * 	D3		| INACT ac/dc
	 * 	D2		| INACT_X enable
	 * 	D1		| INACT_Y enable
	 * 	D0		| INACT_Z enable
	 */
	ADXL343_WriteRegister(0x27, 0b11100000)!=HAL_OK ? debug(D_ERROR,"I2C TRANSMIT in INIT"):(void)0;
	ADXL343_ReadRegister(0x27, &ret,1)!=HAL_OK ? debug(D_ERROR,"I2C READ in INIT"):(void)0;
	//printf("WRITE/READ - ACT_INACT_CTL: 0x%02X\r\n",ret);

	/*	0x2D—POWER_CTL
	 * 	D3		| MEASURE = 1
	 */
	ADXL343_WriteRegister(0x2D, 1<<3)!=HAL_OK ? debug(D_ERROR,"I2C TRANSMIT in INIT"):(void)0;
	ADXL343_ReadRegister(0x2D, &ret,1)!=HAL_OK ? debug(D_ERROR,"I2C READ in INIT"):(void)0;
	//printf("WRITE/READ - POWER_CTL: 0x%02X\r\n",ret);
	/*	0x31—DATA_FORMAT
	 * 	D7 		| SELF_TEST = 0 => Permet de tester tousles axes avec un signal imposé  (~0.5g)
	 * 	D6 		| SPI = 1 => SPI 3-WIRE
	 * 	D5 		| INT_INVERT = 0 => Inverse le high and low des INT
	 * 	D3 		| FULL_RES = 1 => Max resolution
	 * 	D2 		| JUSTIFY = 0 => LSB
	 * 	D1-D0 	| RANGE = 00 =>  ±2g
	 */
	ADXL343_WriteRegister(0x31, 0<<7|1<<6|1<<3|0b00)!=HAL_OK ? debug(D_ERROR,"I2C TRANSMIT in INIT"):(void)0;
	ADXL343_ReadRegister(0x31, &ret,1)!=HAL_OK ? debug(D_ERROR,"I2C READ in INIT"):(void)0;
	//printf("WRITE/READ - dataFormat: 0x%02X\r\n",ret);

	/* 	0x38—FIFO_CTL
	 * 	D7-D6	| FIFO_MODE = 10 => STREAM
	 * 	D5		| TRIGGER BIT = 1 => Redirige ExINT vers INT2
	 * 	D4-D0	| SAMPLE BITS = 0b10000(16) => Nécessite 16 samples avant de trigger le INT
	 */
	ADXL343_WriteRegister(0x38, ((0b10<<6)| 0b10000))!=HAL_OK ? debug(D_ERROR,"I2C TRANSMIT in INIT"):(void)0;
	ADXL343_ReadRegister(0x38, &ret,1)!=HAL_OK ? debug(D_ERROR,"I2C READ in INIT"):(void)0;
	//printf("WRITE/READ - FIFO Mode: 0x%02X\r\n",ret);

	/*
	 ************$	ETALONNAGE	$************
	 */
	int32_t sumX = 0, sumY = 0, sumZ = 0;
	for (int i=0;i<IMU_SAMPLE_0G;i++){
		XYZ_t accXYZ_0G = ADXL343_getAcc();
		sumX += accXYZ_0G.X;
		sumY += accXYZ_0G.Y;
		sumZ += accXYZ_0G.Z;
	}
	// Average
	int16_t avgX = sumX / IMU_SAMPLE_0G;
	int16_t avgY = sumY / IMU_SAMPLE_0G;
	int16_t avgZ = (sumZ / IMU_SAMPLE_0G);//-10;

	//!\\  Each LSB of output in full-resolution is 3.9 mg or one-quarter of an LSB of the offset register
	uint8_t offsetX = -round((float)avgX / (SENSITIVITY_FULL_RES / SCALE_FACTOR_OFFSET_REG));
	uint8_t offsetY = -round((float)avgY / (SENSITIVITY_FULL_RES / SCALE_FACTOR_OFFSET_REG));
	uint8_t offsetZ = -round((float)(avgZ - SENSITIVITY_FULL_RES) / (SENSITIVITY_FULL_RES / SCALE_FACTOR_OFFSET_REG));

	ADXL343_WriteRegister(0x1E, offsetX)!=HAL_OK ? debug(D_ERROR,"I2C READ in INIT"):(void)0;
	ADXL343_WriteRegister(0x1F, offsetY)!=HAL_OK ? debug(D_ERROR,"I2C READ in INIT"):(void)0;
	ADXL343_WriteRegister(0x20, offsetZ)!=HAL_OK ? debug(D_ERROR,"I2C READ in INIT"):(void)0;
}
//Resort la valeur en mm/s2
XYZ_t ADXL343_getAcc(void){
	XYZ_t accXYZ;
	int8_t a_x[2]={0,0};
	int8_t a_y[2]={0,0};
	int8_t a_z[2]={0,0};

	ADXL343_ReadRegister(0x32, (uint8_t*)a_x, 2)!=HAL_OK ? debug(D_ERROR,"I2C RECEIVE in ReadREGISTER"):(void)0;
	ADXL343_ReadRegister(0x34, (uint8_t*)a_y, 2)!=HAL_OK ? debug(D_ERROR,"I2C RECEIVE in ReadREGISTER"):(void)0;
	ADXL343_ReadRegister(0x36, (uint8_t*)a_z, 2)!=HAL_OK ? debug(D_ERROR,"I2C RECEIVE in ReadREGISTER"):(void)0;
	accXYZ.X = (int16_t)(	a_x[1] << 8 | a_x[0]	)*ADXL_SENSIBILITY; //g.e-3 ie mm.s-2
	accXYZ.Y = (int16_t)(	a_y[1] << 8 | a_y[0]	)*ADXL_SENSIBILITY;
	accXYZ.Z = (int16_t)(	a_z[1] << 8 | a_z[0]	)*ADXL_SENSIBILITY;

	/** FILTRAGE **/
	accXYZ.X = abs(accXYZ.X)>=MM_S2_SEUIL ? accXYZ.X:0;
	accXYZ.Y = abs(accXYZ.Y)>=MM_S2_SEUIL ? accXYZ.Y:0;
	accXYZ.Z = abs(accXYZ.Z)>=MM_S2_SEUIL ? accXYZ.Z:0;

	return accXYZ;
	/*
	 * Pour obtenir vitesse et position : CallBack dans fichier .IT
	 */
}

HAL_StatusTypeDef ADXL343_WriteRegister(uint8_t reg, uint8_t data){
	uint8_t buffer[2]={reg,data};
	return HAL_I2C_Master_Transmit(&I2C_IMU, (uint16_t)(ADXL_ADRR<<1), buffer, 2 ,HAL_MAX_DELAY);
}

HAL_StatusTypeDef ADXL343_ReadRegister(uint8_t reg, uint8_t* recptData, uint8_t lenData){
	HAL_I2C_Master_Transmit(&I2C_IMU, (uint16_t)(ADXL_ADRR<<1), &reg, 1 ,HAL_MAX_DELAY)!=HAL_OK ? debug(D_ERROR,"I2C TRANSMIT in ReadREGISTER"):(void)0;
	return HAL_I2C_Master_Receive(&I2C_IMU, (uint16_t)(ADXL_ADRR<<1), recptData, lenData ,HAL_MAX_DELAY);
}
