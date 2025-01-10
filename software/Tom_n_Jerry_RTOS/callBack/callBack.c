/*
 * callBack.c
 *
 *  Created on: Nov 19, 2024
 *      Author: romingo
 */

#include "callBack.h"
#include "changeMode.h"

#define SECONDS_BLOCKED 2

static int counterSecondesPreempt = -1;
int isSpeedActualisationIsBlocked = 0;
SemaphoreHandle_t sem_uart_read = NULL;
extern 	SemaphoreHandle_t semb_cpltCllbck;
extern SemaphoreHandle_t semb_halfCllbck;

extern TaskHandle_t h_task_asserv_I;
//extern TaskHandle_t h_task_asserv_XYZ;

extern int isSeeIMU;
extern int isSpeedInit;
extern MDriver_t MDriver1;
extern MDriver_t MDriver2;
extern TaskHandle_t h_task_changemenMode;
extern int workMode;

extern LED_t LED;
extern int ANGLE_GLOBAL;
extern int verbosePulse;

int isFalling1=0;
int isFalling3=0;

/*
 * We must declared here a function CUSTOM_ because main.c already use this callback
 */

/*********** PREEMPTION PRIORITY 14 ***********/
void CUSTOM_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM15) { //Every 1s
		if (verbosePulse){
		printf("\r\nANGLE:%i\tWorkMode:%s\t%s\r\n",ANGLE_GLOBAL,(workMode == 1) ? "CHAT" : "SOURIS",(isSpeedActualisationIsBlocked==1) ? "BLOCKED" : "");
		printf("FWD1 | pulseGoal: %lu | pulse: %lu\r\n", MDriver1.FWD->pulseGoal, *(MDriver1.FWD->CCR_Channel));
		printf("REV1 | pulseGoal: %lu | pulse: %lu\r\n", MDriver1.REV->pulseGoal, *(MDriver1.REV->CCR_Channel));
		printf("FWD2 | pulseGoal: %lu | pulse: %lu\r\n", MDriver2.FWD->pulseGoal, *(MDriver2.FWD->CCR_Channel));
		printf("REV2 | pulseGoal: %lu | pulse: %lu\r\n", MDriver2.REV->pulseGoal, *(MDriver2.REV->CCR_Channel));
		}
		if (isSeeIMU) {
			uint8_t ret_ADXL;
			uint8_t ret_GPIO;
			ADXL343_ReadRegister(0x2B, &ret_ADXL, 1);
			TCA9555_ReadRegister(0x01, &ret_GPIO, 1);
			printf("READ - ACP_TAP_STATUS :0x%02X\r\n",ret_ADXL);
			printf("READ - OUTPUT Port Registers :0x%02X\r\n",ret_GPIO);

		}
		if(counterSecondesPreempt>0){
 			counterSecondesPreempt--;
		}
		else{
			isSpeedActualisationIsBlocked = 0; // On debloque
		}
	}
	if (htim->Instance == TIM16) { //Every 250us
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
				((isSMOOTHspeed & 0b10)>> 1) ?
						IT_ZXB5210_speed_UPDATE(&MDriver1, MDriver1.REV):(void)0;
				((isSMOOTHspeed & 0b100)>> 2) ?
						IT_ZXB5210_speed_UPDATE(&MDriver2, MDriver2.FWD):(void)0;
				((isSMOOTHspeed & 0b1000)>> 3) ?
						IT_ZXB5210_speed_UPDATE(&MDriver2, MDriver2.REV):(void)0;
			}
		}

		/* CHECK DETECTION BORD */
		if (HAL_GPIO_ReadPin(MOUSTACHE_1_GPIO_Port,MOUSTACHE_1_Pin) != GPIO_PIN_RESET)//Pour le debug on met RESET MAIS C'est BIEN SET
		{
			LED_update(LED.L4.G,124);
			//ANGLE MOTEUR --> CHANGEMENT DE DIRECTION ±180
			if(ANGLE_GLOBAL >=0 && !isFalling1){
				//ANGLE_GLOBAL = ((ANGLE_GLOBAL + 180) % 360 + 360) % 360;
				ZXB5210_angle(0);
				isFalling1 = 1;
				//Fenetre temporelle ou la consigne de demi-tour préemte sur celle du Lidar
				counterSecondesPreempt = SECONDS_BLOCKED;
				isSpeedActualisationIsBlocked = 1;
			}
		}
		else{
			LED_update(LED.L4.G,0);
			isFalling1 = 0;
		}

		if (HAL_GPIO_ReadPin(MOUSTACHE_3_GPIO_Port,MOUSTACHE_3_Pin) != GPIO_PIN_RESET)
		{
			LED_update(LED.L4.B,124);
			//ANGLE MOTEUR --> CHANGEMENT DE DIRECTION ±180
			if(ANGLE_GLOBAL >= 0 && !isFalling3){
				//ANGLE_GLOBAL = ((ANGLE_GLOBAL + 180) % 360 + 360) % 360;
				ZXB5210_angle(180);
				isFalling3 = 1;
				//Fenetre temporelle ou la consigne de demi-tour préemte sur celle du Lidar
				counterSecondesPreempt = SECONDS_BLOCKED;
				isSpeedActualisationIsBlocked =1;
			}
		}
		else{
			LED_update(LED.L4.B,0);
			isFalling3 = 0;
		}
	}
}
void CUSTOM_EXTI_IRQHandler(void){
	if (__HAL_GPIO_EXTI_GET_IT(GE_EXTI_Pin) != RESET)
	{
		BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
		xTaskNotifyFromISR(h_task_changemenMode,
				1,
				eSetBits,
				&pxHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
	}

	if (__HAL_GPIO_EXTI_GET_IT(START_Pin) != RESET)
	{
		isSpeedInit = 1-isSpeedInit;
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

void HAL_DACEx_ConvCpltCallbackCh2(DAC_HandleTypeDef *hdac){
	if (hdac->Instance == DAC1) {
		printf("FIN DE DU DAC\r\n");}
}





void vApplicationStackOwerflowHook(xTaskHandle xTask, signed char* pcTaskName){
	return;
}
