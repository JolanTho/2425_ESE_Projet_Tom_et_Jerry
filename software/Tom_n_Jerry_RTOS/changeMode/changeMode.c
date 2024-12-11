/*
 * changeMode.c
 *
 *  Created on: Nov 27, 2024
 *      Author: romingo
 */


#include "changeMode.h"
#include "song.h"
#include "components/TCA9555.h"
#include "components/LP5812.h"
#include "components/ADXL343.h"

static int  workMode =0;

void changement_mode_init(void){

}
void changeMode_run(void*){

	for(;;){
		ulTaskNotifyTake(pdTRUE,portMAX_DELAY);

		// Lire dans les registre du Ge
		uint8_t ret=0;
		TCA9555_ReadRegister(0x03,&ret,1);
		uint8_t INT_IMU2 =  (ret >> 7) & 0b1;
		// Prendre decision
		if (INT_IMU2==1){
			workMode = workMode ==0 ? 1 : 0;
		}
		if (workMode){
			LP5812_WriteRegister(0x044,125);
			LP5812_WriteRegister(0x049,0);

		}
		else{
			LP5812_WriteRegister(0x049,125);
			LP5812_WriteRegister(0x044,0);
		}
		uint8_t retADXL=0;
		ADXL343_ReadRegister(0x30,&retADXL,1); //Permet de reinitialiser le registre de l'adxl
	}
}

void play_song(void){
	//Ici on joue le son miaou
}
