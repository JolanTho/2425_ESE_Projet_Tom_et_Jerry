/*
 * asserv.c
 *
 *  Created on: Nov 20, 2024
 *      Author: romingo
 */
#include "asserv.h"
#include "shell.h"
#include "components/ADXL343.h"

#define R_SHUNT  10 //mOhm
#define GAIN_NCS199A2R 100
#define Te 0.001
extern uint16_t adc2_asserv_VAL[2];

int16_t I_cons_1 = 0;
int16_t I_cons_2 = 0;

void asserv_init(void){

}
/************ ASSERV COURANT ************/
void asserv_courant_run(void*){
	for(;;){

		ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
		// --> Variation DE 250mV de 0% a 100%duty cycle  ET  450mV de -100% a 0% duty cycle
		printf("\r\nADC CHANNEL 1:%i\t\tADC CHANNEL 2:%i\r\n",(int16_t)adc2_asserv_VAL[0],(int16_t)adc2_asserv_VAL[1]);
		int16_t I_1_mA = (int16_t)adc2_asserv_VAL[0] /(R_SHUNT * GAIN_NCS199A2R); //RSHUNT EN mOhm --> I en mA
		int16_t I_2_mA = (int16_t)adc2_asserv_VAL[1] /(R_SHUNT * GAIN_NCS199A2R); //RSHUNT EN mOhm --> I en mA
		printf("Courant Driver1: %i (mA)\t\tCourant Driver2: %i (mA)\r\n",I_1_mA,I_2_mA);
		//** Fonction de Transfert **//
		int32_t erreur_1= I_cons_1 - I_1_mA;
		int32_t erreur_2= I_cons_1 - I_2_mA;
		/* .... */
		/**	Transmission des consignes aux moteurs	**/
		/**											**/
	}
}


/************ ASSERV VITESSE ROTATION ************/
void asserv_vitesse_run(void*){
	// Consigne via le Lidar (dpd du mode aussi mais plus tard)
	// Utilisation d'un observatueur de Luenberg pour reconstruire l'état de vitesse de rotation
	return;
}

extern XYZ_t accXYZ;
extern XYZ_t vitXYZ;
extern XYZ_t posXYZ;
/************ ASSERV POSITION ************/
void asserv_position_run(void*){
	uint8_t fifo_status = 0;
	for(;;){

		ulTaskNotifyTake(pdTRUE,portMAX_DELAY);

		ADXL343_ReadRegister(0x00, &fifo_status, 1) != HAL_OK ?
				debug(D_ERROR, "I2C READ in IT") : (void) 0;
		if (fifo_status >> 7) {
			//GET POSITION VIA DERIVATION
			XYZ_t accPREV = accXYZ;
			XYZ_t vitPREV = vitXYZ;
			XYZ_t posPREV = posXYZ;

			accXYZ = ADXL343_getAcc();
			vitXYZ = (XYZ_t ) {vitPREV.X + (accXYZ.X + accPREV.X)*Te/2, vitPREV.Y + (accXYZ.Y + accPREV.Y)*Te/2,
				vitPREV.Z + (accXYZ.Z + accPREV.Z)*Te/2}; //   mm/s
			posXYZ = (XYZ_t ) {posPREV.X + (vitXYZ.X + vitPREV.X)*Te/2,posPREV.X + (vitXYZ.Y + vitPREV.Y)*Te/2,
				posPREV.Z + (vitXYZ.Z + vitPREV.Z)*Te/2}; // m&m



			printf("accX (mm.s-2): %0.2f| accY: %0.2f| accZ: %0.2f\r\n",
					accXYZ.X, accXYZ.Y, accXYZ.Z);
			printf("vitX (mm.s-1): %0.2f| vitY: %0.2f| vitZ: %0.2f\r\n",
					vitXYZ.X, vitXYZ.Y, vitXYZ.Z);
			printf("posX (mm): %0.2f| posY: %0.2f| posZ: %0.2f\r\n",
					posXYZ.X, posXYZ.Y, posXYZ.Z);
			printf(separator);


			fifo_status = 0;
		}
	}
}


