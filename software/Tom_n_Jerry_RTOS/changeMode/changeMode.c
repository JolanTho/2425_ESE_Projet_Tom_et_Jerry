/*
 * changeMode.c
 *
 *  Created on: Nov 27, 2024
 *      Author: romingo
 */


#include "changeMode.h"
#include "components/TCA9555.h"
#include "components/LP5812.h"
#include "components/ADXL343.h"
#include "dac.h"
#include "miaou.h"

extern LED_t LED;

static int  workMode =0;
#define DELAI_NOTIFICATION_MS 500

void changement_mode_init(void){

}
void changeMode_run(void*){

	TickType_t lastCall_Ticks = 0;
	LED_update(LED.L1.R,255);
	//LED_update(LED.L2.R,255);
	//LED_update(LED.L3.R,255);
	//LED_update(LED.L4.R,255);
	for(;;){
		ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
		// Obtenir le temps actuel en ticks
		TickType_t tempsActuelTicks = xTaskGetTickCount();

		// Calculer la durée écoulée depuis la dernière notification
		TickType_t dureeDepuisDernierAppel = tempsActuelTicks - lastCall_Ticks;

		// Vérifier si la durée dépasse le délai défini
		if (dureeDepuisDernierAppel > pdMS_TO_TICKS(DELAI_NOTIFICATION_MS)) {
			// Mettre à jour l'horodatage du dernier appel
			lastCall_Ticks = tempsActuelTicks;
			// Lire dans les registre du Ge
			uint8_t ret=0;
			TCA9555_ReadRegister(0x03,&ret,1);
			uint8_t INT_IMU2 =  (ret >> 7) & 0b1;
			// Prendre decision
			if (INT_IMU2==1){
				workMode = workMode ==0 ? 1 : 0;
			}
			if (workMode){
				//LP5812_WriteRegister(0x044,125);
				//LP5812_WriteRegister(0x049,0);
				play_song();

			}
			else{
				//LP5812_WriteRegister(0x049,125);
				//LP5812_WriteRegister(0x044,0);
			}
			//LP5812_WriteRegister(0x046,0); //Led Cooldown

		}
		else{
			//LP5812_WriteRegister(0x046,124); //Led Cooldown

		}
		uint8_t retADXL=0;
		ADXL343_ReadRegister(0x30,&retADXL,1); //Permet de reinitialiser le registre de l'adxl
	}
}

void play_song(void){
	//Ici on joue le son miaou
	HAL_DAC_Start_DMA(&hdac1, DAC1_CHANNEL_1, (uint32_t*)audio_table, AUDIO_LENGTH,DAC_ALIGN_8B_R) !=HAL_OK ? debug(START,"DMA - MIAOU") : debug(D_ERROR,"DMA - MIAOU");


}
