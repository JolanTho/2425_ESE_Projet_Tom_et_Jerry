/*
 * callBack.c
 *
 *  Created on: Nov 19, 2024
 *      Author: romingo
 */

#include "callBack.h"

SemaphoreHandle_t sem_uart_read = NULL;
extern 	SemaphoreHandle_t semb_cpltCllbck;
extern SemaphoreHandle_t semb_halfCllbck;

extern TaskHandle_t h_task_asserv_I;
extern TaskHandle_t h_task_asserv_XYZ;

extern XYZ_t accXYZ;
extern XYZ_t vitXYZ;
extern XYZ_t posXYZ;
extern int isSeeIMU;
extern int isSpeedInit;
extern MDriver_t MDriver1;
extern MDriver_t MDriver2;

//extern uint8_t fifo_status = 0;

/*
 * We must declared here a function CUSTOM_ because main.c already use this callback
 */

/*********** PREEMPTION PRIORITY 14 ***********/
void CUSTOM_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM15) { //Every 1s
		if (isSeeIMU) {
			uint8_t ret_ADXL;
			uint8_t ret_GPIO;
			ADXL343_ReadRegister(0x2B, &ret_ADXL, 1);
			TCA9555_ReadRegister(0x01, &ret_GPIO, 1);
			printf("READ - ACP_TAP_STATUS :0x%02X\r\n",ret_ADXL);
			printf("READ - OUTPUT Port Registers :0x%02X\r\n",ret_GPIO);


			BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
			xTaskNotifyFromISR(h_task_asserv_XYZ,
					1,
					eSetBits,
					&pxHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);

		}
	}
	if (htim->Instance == TIM16) { //Every 1ms
		if (isSpeedInit) {
			/*** SMOOTH SPEED CHANGE ***/
			uint8_t isSMOOTHspeed = 0;
			isSMOOTHspeed |= MDriver1.FWD->pulseGoal != *(MDriver1.FWD->CCR_Channel) ? 1: 0; //Stocke sur un bit si on doit changer une valeur de pulse
			isSMOOTHspeed |= MDriver1.REV->pulseGoal != *(MDriver1.REV->CCR_Channel) ? 1<<1: 0;
			isSMOOTHspeed |= MDriver2.FWD->pulseGoal != *(MDriver2.FWD->CCR_Channel) ? 1<<2: 0;
			isSMOOTHspeed |= MDriver2.REV->pulseGoal != *(MDriver2.REV->CCR_Channel) ? 1<<3: 0;


			if(isSMOOTHspeed){ // Si un seul 1 est présent aloir la condition est vrai
				(isSMOOTHspeed & 0b1)>> 0 ? // Si on a detecter une erreur
						IT_ZXB5210_speed_UPDATE(&MDriver1, MDriver1.FWD):(void)0;
				(isSMOOTHspeed & 0b10)>> 1 ?
						IT_ZXB5210_speed_UPDATE(&MDriver1, MDriver1.REV):(void)0;
				(isSMOOTHspeed & 0b100)>> 2 ?
						IT_ZXB5210_speed_UPDATE(&MDriver2, MDriver2.FWD):(void)0;
				(isSMOOTHspeed & 0b1000)>> 3 ?
						IT_ZXB5210_speed_UPDATE(&MDriver2, MDriver2.REV):(void)0;
			}
		}
	}
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {

    if (huart->Instance == USART2) {
    	BaseType_t higher_priority_task_woken = pdFALSE;
    	xSemaphoreGiveFromISR(semb_halfCllbck, &higher_priority_task_woken);
    	portYIELD_FROM_ISR(higher_priority_task_woken);
    }
}

/*********** PREEMPTION PRIORITY 5 ***********/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

	/** SHELL INTERRUPTIONS **/
	if (huart->Instance == USART1) {
		BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(sem_uart_read, &pxHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
	}

    if (huart->Instance == USART2){
		BaseType_t higher_priority_task_woken = pdFALSE;
    	xSemaphoreGiveFromISR(semb_cpltCllbck, &higher_priority_task_woken);
    	portYIELD_FROM_ISR(higher_priority_task_woken);
    }
}
/*********** PREEMPTION PRIORITY 5 ***********/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc->Instance == ADC2) {
		BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
		xTaskNotifyFromISR(h_task_asserv_I,
				1,
				eSetBits,
				&pxHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
	}
}

void vApplicationStackOwerflowHook(xTaskHandle xTask, signed char* pcTaskName){
	return;
}
