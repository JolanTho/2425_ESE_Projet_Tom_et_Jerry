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

extern uint16_t adc2_asserv_VAL[2];

int16_t I_cons_1 = 0;
int16_t I_cons_2 = 0;

void asserv_init(void){

}
/************ ASSERV COURANT ************/
void asserv_courant_run(void*){

	for(;;){

		ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
		// --> Varaiation DE 250mV de 0% a 100%duty cycle  ET  450mV de -100% a 0% duty cycle
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

			vitXYZ = (XYZ_t ) {(vitPREV.X - accPREV.X*10)/1000, accPREV.Y - accXYZ.Y,
				accPREV.Z - accXYZ.Z }; //   mm/s

			posXYZ = (XYZ_t ) {(posPREV.X - vitXYZ.X*10)/1000, posPREV.Y - vitPREV.Y,
				posPREV.Z - vitPREV.Z }; // m&m




			printf("accX (mm.s-2): %-24li| accY: %-24li| accZ: %-24li\r\n", accXYZ.X,
					accXYZ.Y, accXYZ.Z);
			printf("vitX (mm.s-1): %-20li| vitY: %-20li| vitZ: %-20li\r\n",
					vitXYZ.X, vitXYZ.Y, vitXYZ.Z);
			printf("posX (mm): %-20li| posY: %-20li| posZ: %-20li\r\n",
					posXYZ.X, posXYZ.Y, posXYZ.Z);
			printf(separator);


			fifo_status = 0;
		}
	}
}


