/*
 * callBack.c
 *
 *  Created on: Nov 19, 2024
 *      Author: romingo
 */

#include "callBack.h"

SemaphoreHandle_t sem_uart_read = NULL;

extern XYZ_t accXYZ;
extern XYZ_t vitXYZ;
extern XYZ_t posXYZ;
extern int isSeeIMU;
extern int isASSERV;


extern int isASSERV;
extern int isADC_cplt;

uint8_t fifo_status = 0;

/*
 * We must declared here a function CUSTOM_ because main.c already use this callback
 */

void CUSTOM_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if (htim->Instance == TIM15) { //Every 1ms
		if (isSeeIMU) {
			ADXL343_ReadRegister(0x00, &fifo_status, 1) != HAL_OK ?
					debug(D_ERROR, "I2C READ in IT") : (void) 0;
			if (fifo_status >> 7) {
				XYZ_t accPREV = accXYZ;
				XYZ_t vitPREV = vitXYZ;

				accXYZ = ADXL343_getAcc();

				vitXYZ = (XYZ_t ) { accPREV.X - accXYZ.X, accPREV.Y - accXYZ.Y,
					accPREV.Z - accXYZ.Z };
				posXYZ = (XYZ_t ) { vitPREV.X - vitPREV.X, vitPREV.Y - vitPREV.Y,
					vitPREV.Z - vitPREV.Z };

				printf("accX: %-24i|accY: %-24i|accZ: %-24i\r\n", accXYZ.X,
						accXYZ.Y, accXYZ.Z);
				printf("1e3 vitX: %-20i|1e3 vitY: %-20i|1e3 vitZ: %-20i\r\n",
						vitXYZ.X, vitXYZ.Y, vitXYZ.Z);
				printf("1e3 posX: %-20i|1e3 posY: %-20i|1e3 posZ: %-20i\r\n",
						posXYZ.X, posXYZ.Y, posXYZ.Z);
				printf(separator);
				fifo_status = 0;
			}
		}
		if (isASSERV) {

			// Lancer le ADC
		}
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	/** SHELL INTERRUPTIONS **/
	if (huart->Instance == USART1) {
		BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(sem_uart_read, &pxHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
	}
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc->Instance == ADC2) {
		isADC_cplt = 1;
	}
}
