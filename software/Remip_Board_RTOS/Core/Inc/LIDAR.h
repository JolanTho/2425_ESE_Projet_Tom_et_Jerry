/*
 * LIDAR.h
 *
 *  Created on: Oct 16, 2024
 *      Author: romingo
 */

#ifndef INC_LIDAR_H_
#define INC_LIDAR_H_

uint8_t CMD_GET_SCAN[2] = {0xA5,0x60};
uint8_t CMD_STOP[2] = {0xA5,0x65};
uint8_t CMD_GET_INF[2] = {0xA5,0x90};
uint8_t CMD_GET_HEALTH[2] = {0xA5,0x91};
uint8_t CMD_SOFT_RESTART[2] = {0xA5,0x80};

typedef struct {
	uint8_t LENGTH[4];
	uint8_t MODE[1];
	uint8_t CODE[1];
	void* CONTENT; //Le but est d'associer a "content" une struct
}LIDAR_MSG;

typedef struct{
	uint8_t MODEL[2];
	uint8_t FIRM_V[2];
	uint8_t HARD_V[2];
	uint8_t SERIAL_NUM[16];
}GET_HEALTH_T;


typedef union{
	uint8_t header[7];
	struct{
		uint8_t useless : 8*5;
		uint8_t mode : 1;
	};
}LIDAR_HEADER;
#endif /* INC_LIDAR_H_ */
