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
#include "components/ZXB5210.h"
#include "dac.h"
//#include "miaou.h"


#define AUDIO_LENGTH 200      // Nombre de points
#define SAMPLING_FREQ 8000    // Fréquence d'échantillonnage (8 kHz)
#define DAC_RESOLUTION 255    // Plage du DAC pour 8 bits (0-255)
#define SIN_FREQUENCY 8000   // Fréquence du sinus (8 kHz)

uint8_t audio_table[AUDIO_LENGTH];  // Tableau contenant les valeurs du signal


extern LED_t LED;

int  workMode =1; // On commence en CHAT
#define DELAI_NOTIFICATION_MS 500

void changement_mode_init(void){
 //Le DAC est SYNCHRO AVEC TIM6
//	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);

}
void changeMode_run(void*){
	TickType_t lastCall_Ticks = 0;
	for(;;){
		ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
		// Obtenir le temps actuel en ticks
		TickType_t tempsActuelTicks = xTaskGetTickCount();
		uint8_t ret=0;

		// Calculer la durée écoulée depuis la dernière notification
		TickType_t dureeDepuisDernierAppel = tempsActuelTicks - lastCall_Ticks;

		// Vérifier si la durée dépasse le délai défini
		if (dureeDepuisDernierAppel > pdMS_TO_TICKS(DELAI_NOTIFICATION_MS)) {
			// Mettre à jour l'horodatage du dernier appel
			lastCall_Ticks = tempsActuelTicks;
			// Lire dans les registre du Ge
			TCA9555_ReadRegister(0x03,&ret,1);
			uint8_t INT_IMU2 =  (ret >> 7) & 0b1;
			// Prendre decision
			if (INT_IMU2==1){
				workMode = workMode == 0 ? 1 : 0;
			}
			if (workMode){
				LED_update((uint16_t )LED.L1.G,124);
				LED_update((uint16_t )LED.L1.B,0);
				//play_song();
			}
			else{
				LED_update((uint16_t )LED.L1.G,0);
				LED_update((uint16_t )LED.L1.B,124);
			}
			LED_update((uint16_t )LED.L2.R,0); //Led Cooldown
		}
		else{
			LED_update((uint16_t )LED.L2.R,124); //Led Cooldown
		}
		uint8_t retADXL=0;
		ADXL343_ReadRegister(0x30,&retADXL,1); //Permet de reinitialiser le registre de l'adxl
		TCA9555_ReadRegister(0x03,&ret,1);

	}
}

void play_song(void){
	//Ici on joue le son miaou
	//HAL_DACEx_TriangleWaveGenerate(&hdac1, DAC1_CHANNEL_1, DAC_TRIANGLEAMPLITUDE_4095);
	generate_sine_wave();
	HAL_DAC_Start_DMA(&hdac1, DAC1_CHANNEL_1, (uint32_t*)audio_table, AUDIO_LENGTH,DAC_ALIGN_8B_R) ==HAL_OK ? debug(START,"DMA - MIAOU") : debug(D_ERROR,"DMA - MIAOU");
}


void generate_sine_wave(void) {
    float amplitude = DAC_RESOLUTION / 2;  // Plage moyenne du DAC (0 à 255)
    float offset = amplitude;              // Décalage pour faire osciller autour de 0

    for (int i = 0; i < AUDIO_LENGTH; i++) {
        // Calculer l'échantillon sinusoidal
        float t = (float)i / SAMPLING_FREQ;  // Temps pour l'échantillon i
        float sine_value = sin(2 * M_PI * SIN_FREQUENCY * t);  // Signal sinusoïdal

        // Adapter la valeur du sinus à la plage 0-255 (DAC 8 bits)
        int32_t dac_value = (int32_t)(amplitude * sine_value + offset);

        // Clipper la valeur dans la plage [0, 255]
        if (dac_value < 0) {
            dac_value = 0;
        } else if (dac_value > DAC_RESOLUTION) {
            dac_value = DAC_RESOLUTION;
        }

        // Assigner la valeur au tableau audio (8 bits)
        audio_table[i] = (uint8_t)dac_value;
    }
}
