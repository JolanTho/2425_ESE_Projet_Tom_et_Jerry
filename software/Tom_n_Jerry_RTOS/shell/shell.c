/*
 * shellv2.c
 *
 *  Created on: Nov 14, 2024
 *      Author: romingo
 */

#include <shell.h>
#include "changeMode.h"

extern SemaphoreHandle_t sem_uart_read;

XYZ_t accXYZ;
XYZ_t vitXYZ;
XYZ_t posXYZ;
extern IMURegister_t IMURegister[];
extern MDriver_t MDriver1;
extern MDriver_t MDriver2;

#define NUM_CHANNEL_ADC2 2
extern int isSpeedInit;

extern h_LIDAR_t lidar;

MAPPER mapping[] = {
		{ "help", 	"Print every function available", 	"None",subfunct_help },
		{ "clear",	"Clear screen", 					"None", subfunct_clear },
		{ "start", 	"Start the robot", 					"None", subfunct_start },
		{ "stop", 	"Stop the robot", 					"None", subfunct_stop },
		{ "speed", 	"Change speed motor", 				"int:MotorID, int:speed",subfunct_speed },
		{ "angle",	"Change angle of robot",			"int:angle",			subfunct_angle},
		{ "cs", 	"Coeff qui relie le pulse a la W",	"motorId , FWD/REV offset",subfunct_modify_calc_speed},
		{ "led", 	"Change params of the LEDs", 		"int: LedId	int: %PWM(0-255)", subfunct_setLed },
		{ "imu",	"None", 							"None", subfunct_seeIMU },
		{ "imusf",	"Self test of the IMU",				"None", subfunct_IMU_SelfTest },
		{ "imuu", 	"Read register", 					"None", subfunct_IMU_Update },
		{ "imug", 	"Get Acc Value", 					"None", subfunct_IMU_GET },
		{ "adc",	"Start asservissement courant",		"None",subfunct_Iasserv},
		{"lidar","Fonctions pour lidar","None: start|-h : health_status|-r:restart",subfunct_lidar},
		{ "miaou",	"Play song", 						"None", subfunct_MIAOU },
		{ "reset",	"None", 							"None", reset },

};
uint8_t started[] =
		"\r\n*-----------------------------*"
		"\r\n|     Welcome on Tom&Jerry    |"
		"\r\n*-----------------------------*"
		"\r\n";
uint8_t newline[] = "\r\n";
uint8_t backspace[] = "\b \b";
uint8_t cmdNotFound[] = "Command not found\r\n";
uint8_t prompt[] = "shell@Tom&Jerry>>";
uint8_t clear[] = "\x1b[2J\x1b[H";
uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE];
uint8_t uartTxBuffer[UART_TX_BUFFER_SIZE];

uint16_t adc2_asserv_VAL[2]={0,0};

char cmdBuffer[CMD_BUFFER_SIZE];
int idx_cmd;
char *argv[MAX_ARGS];
int argc = 0;
char *token;
int newCmdReady = 0;
int isSeeIMU = 0;
int isSeeIMUFORCE = 0;
int isFind = 0;
int isStarted = 0;
int isADC_cplt =0;
int lidarDebugShell=0;
uint8_t PWMLed = 255;


void shell_init(void) {
	memset(argv, 0, MAX_ARGS * sizeof(char*));
	memset(cmdBuffer, 0, CMD_BUFFER_SIZE * sizeof(char));
	memset(uartRxBuffer, 0, UART_RX_BUFFER_SIZE * sizeof(char));
	memset(uartTxBuffer, 0, UART_TX_BUFFER_SIZE * sizeof(char));
	sem_uart_read = xSemaphoreCreateBinary();
	sem_uart_read == NULL ? Error_Handler() : (void) 0;

	HAL_UART_Transmit(&UART_DEVICE, started, strlen((char*) started),
			HAL_MAX_DELAY);
	HAL_UART_Transmit(&UART_DEVICE, prompt, strlen((char*) prompt),
			HAL_MAX_DELAY);
	printf("\r\n");
	subfunct_start(0);
}

void shell_run(void*) {
	int reading = 0;
	for (;;) {
		reading = 1;
		idx_cmd = 0;
		while (reading) {
			HAL_UART_Receive_IT(&UART_DEVICE, uartRxBuffer, UART_RX_BUFFER_SIZE);
			xSemaphoreTake(sem_uart_read, portMAX_DELAY)!=pdPASS ? Error_Handler():(void)0;

			switch (uartRxBuffer[0]) {
			case ASCII_CR: // Nouvelle ligne, instruction à traiter
				HAL_UART_Transmit(&UART_DEVICE, newline, sizeof(newline),
						HAL_MAX_DELAY);
				//cmdBuffer[idx_cmd] = '\0';
				//On copie la commande dans l'historique
				//strcpy(cmdHistory[cmdHistory_index_save % CMD_HISTORY_SIZE], cmdBuffer);
				//cmdHistory_index_save++;
				argc = 0;
				token = strtok(cmdBuffer, " ");
				while (token != NULL) {
					argv[argc++] = token;
					token = strtok(NULL, " ");
				}
				idx_cmd = 0;
				reading = 0;        //exit read loop
				newCmdReady = 1;
				break;
			case ASCII_BACK:
			case ASCII_BACK_BIS: // Suppression du dernier caractère
				if (idx_cmd == 0) {
					break;
				}
				cmdBuffer[idx_cmd--] = '\0';
				HAL_UART_Transmit(&UART_DEVICE, backspace, sizeof(backspace),
						HAL_MAX_DELAY);
				break;
			default:
				cmdBuffer[idx_cmd++] = uartRxBuffer[0];
				HAL_UART_Transmit(&UART_DEVICE, uartRxBuffer,
						UART_RX_BUFFER_SIZE, HAL_MAX_DELAY);
			}
		}
		if (newCmdReady) {
			for (int i = 0; i < sizeof(mapping) / sizeof(mapping[0]); i++) {
				if (strcmp(argv[0], mapping[i].name) == 0) {
					isFind = 1;
					mapping[i].funct(argv);
				}
			}
			if (!isFind) {
				HAL_UART_Transmit(&UART_DEVICE, cmdNotFound,
						sizeof(cmdNotFound), HAL_MAX_DELAY);
			}

			HAL_UART_Transmit(&UART_DEVICE, prompt, sizeof(prompt),
					HAL_MAX_DELAY);
			newCmdReady = 0;
			isFind = 0;
			memset(cmdBuffer, 0, MAX_ARGS * sizeof(char*));

		}
	}
}

/*
 * ***********************************************************************************************
 * 										SUBFUNCTIONS
 *************************************************************************************************/
void subfunct_help(char **argv) {
	printf("%s%-15s | %-30s | %-50s\r\n%s%s", GRAS, "Fonctions", "Params",
			"Description", D_RESET, separator);
	for (int i = 0; i < sizeof(mapping) / sizeof(mapping[0]); i++) {
		printf("%s%-15s%s | %-30s | %-50s\r\n", FUNCTION, mapping[i].name,
				D_RESET, mapping[i].params, mapping[i].resume);
	}
	printf(separator);
}
void subfunct_clear(char **argv) {
	HAL_UART_Transmit(&UART_DEVICE, clear, sizeof(clear), HAL_MAX_DELAY);
}
void subfunct_start(char **argv) {

	HAL_TIM_Base_Start_IT(&htim15) == HAL_OK ?
			debug(START, "TIMER 15 - CALCUL IMU") : (void) 0;
	HAL_TIM_Base_Start_IT(&htim16) == HAL_OK ?
			debug(START, "TIMER 16 - SMOOTH SPEED") : (void) 0;
	HAL_TIM_Base_Start(&htim2) == HAL_OK ?
			debug(START, "TIMER 2 for PWM" ) : debug(D_ERROR, "TIMER 2 for PWM");
	HAL_TIM_Base_Start(&htim3) == HAL_OK ?
			debug(START, "TIMER 3 for PWM") : debug(D_ERROR, "TIMER 3 for PWM");
	HAL_ADCEx_Calibration_Start(&hadc2,ADC_SINGLE_ENDED) == HAL_OK ?
			debug(START, "ADC2 CALIBRATION") : debug(D_ERROR, "ADC2 CALIBRATION");
	HAL_TIM_Base_Start(&htim6)!= HAL_OK ?
			debug(START,"TIM6 - MIAOU") : debug(D_ERROR,"TIM6 - MIAOU");

	LP5812_WriteRegister(0x049,0);//LED HAUTE VERTE
	LP5812_WriteRegister(0x048,0);
	LP5812_WriteRegister(0x047,0);
	LP5812_WriteRegister(0x046,0);
	LP5812_WriteRegister(0x045,0);
	LP5812_WriteRegister(0x044,0);
	//To-Do : Chenillard des LEDs
	TCA9555_init();
	LP5812_Init();
	ADXL343_init();
	ZXB5210_init();

	HAL_UART_Transmit(&UART_DEVICE, prompt, sizeof(prompt),
			HAL_MAX_DELAY);
	return;
}
void subfunct_stop(char **argv) {
	HAL_TIM_Base_Stop(&htim15) == HAL_OK ?
			debug(STOP, "TIMER 15 - CALCUL IMU") : (void) 0;
	ZXB5210_speed_FWD(&MDriver1,0);
	ZXB5210_speed_FWD(&MDriver2,0);

	//	HAL_TIM_Base_Stop(&htim2) == HAL_OK ?
	//			debug(STOP, "TIMER 2") : debug(D_ERROR, "TIMER 2");
	//	HAL_TIM_Base_Stop(&htim3) == HAL_OK ?
	//			debug(STOP, "TIMER 3") : debug(D_ERROR, "TIMER 3");
	HAL_ADC_Stop_DMA(&hadc2) == HAL_OK ?
			debug(STOP, "ADC DMA") : debug(D_ERROR, "ADC2 DMA");

	ZXB5210_deinit();
	return;
}
void subfunct_speed(char **argv) {
	/*
	 * int:MotorID 	int:speed
	 * ex:	speed 1 90
	 */
	MDriver_t* MDriver;
	if (argv[1] == NULL || argv[2] == NULL) {
		debug(INFORMATION,"SPEED - ARGUMENTS NEEDED");
		return;
	}
	uint8_t driver_id = (uint8_t) strtol(argv[1], NULL, 10); // Base 10
	int8_t s_alpha = (int8_t) strtol(argv[2], NULL, 10); //Prends des valeurs entre -128 et 127
	if (isSpeedInit==0){isSpeedInit=1;}
	MDriver = driver_id==1 ? &MDriver1 : &MDriver2;
	s_alpha >= 0 ? ZXB5210_speed_FWD(MDriver, (uint8_t)s_alpha) : ZXB5210_speed_REV(MDriver, (uint8_t)-s_alpha);

	return;
}
void subfunct_angle(char **argv) {
	/*
	 * int:MotorID 	int:speed
	 * ex:	speed 1 90
	 */
	if (argv[1] == NULL) {
		debug(INFORMATION,"ANGLE - ARGUMENTS NEEDED");
		return;
	}
	int angle = (int) strtol(argv[1], NULL, 10); // Base 10
	if (isSpeedInit==0){isSpeedInit=1;}
	ZXB5210_angle(angle);
	printf("FWD1 | pulseGoal: %lu | pulse: %lu\r\n", MDriver1.FWD->pulseGoal, *(MDriver1.FWD->CCR_Channel));
	printf("REV1 | pulseGoal: %lu | pulse: %lu\r\n", MDriver1.REV->pulseGoal, *(MDriver1.REV->CCR_Channel));
	printf("FWD2 | pulseGoal: %lu | pulse: %lu\r\n", MDriver2.FWD->pulseGoal, *(MDriver2.FWD->CCR_Channel));
	printf("REV2 | pulseGoal: %lu | pulse: %lu\r\n", MDriver2.REV->pulseGoal, *(MDriver2.REV->CCR_Channel));

	return;
}
void subfunct_Iasserv(char **argv) {
	/** THIS DMA REQUEST IS SYNC WITH THE TIM15 EVENT **/

	HAL_ADC_Start_DMA(&hadc2,(uint32_t*)adc2_asserv_VAL,NUM_CHANNEL_ADC2) == HAL_OK ?
			debug(START, "ADC2 DMA") : debug(D_ERROR, "ADC2 DMA");
	return;
}
void subfunct_setLed(char **argv) {
	/*
	 * int: LedId	int: %PWM(0-255)
	 */

	//uint8_t unused = argv[0];
	//uint8_t LedID = (uint8_t) strtol(argv[1], NULL, 10); // Base 10
	uint8_t PWM = (uint8_t) strtol(argv[2], NULL, 10); // Base 10
	/* Set chip_en = 1 to enable the device*/
	LP5812_WriteRegister(0x000, 0x01) == HAL_OK ?
			printf("I2C Ok\r\n") : printf("I2C Error\r\n");
	/* Set led_mode = 4h to configure the LED drive mode as direct drive mode*/
	LP5812_WriteRegister(0x002, 0x40) == HAL_OK ?
			printf("I2C Ok\r\n") : printf("I2C Error\r\n");
	/* Send update command to complete configuration settings*/
	LP5812_WriteRegister(0x010, 0x55) == HAL_OK ?
			printf("I2C Ok\r\n") : printf("I2C Error\r\n");

	/* Set duty cycle for LEDs*/
	PWMLed = PWM < 255 ? 255 : 0;
	LP5812_WriteRegister(0x044, PWMLed) == HAL_OK ?
			printf("I2C Ok\r\n") : printf("I2C Error\r\n");
	LP5812_WriteRegister(0x045, PWMLed) == HAL_OK ?
			printf("I2C Ok\r\n") : printf("I2C Error\r\n");
	LP5812_WriteRegister(0x046, PWMLed) == HAL_OK ?
			printf("I2C Ok\r\n") : printf("I2C Error\r\n");
	LP5812_WriteRegister(0x047, PWMLed) == HAL_OK ?
			printf("I2C Ok\r\n") : printf("I2C Error\r\n");
	LP5812_WriteRegister(0x048, PWMLed) == HAL_OK ?
			printf("I2C Ok\r\n") : printf("I2C Error\r\n");
	LP5812_WriteRegister(0x049, PWMLed) == HAL_OK ?
			printf("I2C Ok\r\n") : printf("I2C Error\r\n");
	LP5812_WriteRegister(0x04A, PWMLed) == HAL_OK ?
			printf("I2C Ok\r\n") : printf("I2C Error\r\n");
	LP5812_WriteRegister(0x04B, PWMLed) == HAL_OK ?
			printf("I2C Ok\r\n") : printf("I2C Error\r\n");
	LP5812_WriteRegister(0x04C, PWMLed) == HAL_OK ?
			printf("I2C Ok\r\n") : printf("I2C Error\r\n");
	LP5812_WriteRegister(0x04D, PWMLed) == HAL_OK ?
			printf("I2C Ok\r\n") : printf("I2C Error\r\n");
	LP5812_WriteRegister(0x04E, PWMLed) == HAL_OK ?
			printf("I2C Ok\r\n") : printf("I2C Error\r\n");
	LP5812_WriteRegister(0x04F, PWMLed) == HAL_OK ?
			printf("I2C Ok\r\n") : printf("I2C Error\r\n");
}
void subfunct_seeIMU(char **argv) {
	/*
	 * argv None
	 */
	isSeeIMU = isSeeIMU == 1 ? 0 : 1;

	/* in main.c : TIM16 permet de faire calculer la vitesse et la position   */
}
void subfunct_IMU_Update(char **argv) {
	/*
	 * argv None
	 */
	uint8_t startReg = 0x1D;
	uint8_t endReg = 0x39;
	for (int i = 1; i <= endReg - startReg + 1; i++) {
		uint8_t ret = 0;
		ADXL343_ReadRegister(IMURegister[i].reg, &ret, 1);
		printf("READ - 0x%02X (%s): 0x%02X\r\n", IMURegister[i].reg,
				IMURegister[i].name, ret);
	}
	/* in main.c : TIM16 permet de faire calculer la vitesse et la position   */
}
void subfunct_IMU_SelfTest(char **argv) {
	/*
	 * argv None
	 */
	uint8_t ret;

	isSeeIMUFORCE = isSeeIMUFORCE == 1 ? 0 : 1;
	char *debugType = isSeeIMUFORCE == 1 ? START : STOP;

	ADXL343_ReadRegister(0x31, &ret, 1) != HAL_OK ?
			debug(D_ERROR, "I2C READ in IMUFORCE") : (void) 0;
	ADXL343_WriteRegister(0x31, ((isSeeIMUFORCE << 7) | (ret & 0x7F)))
	!= HAL_OK ? debug(D_ERROR, "I2C WRITE in IMUFORCE") : (void) 0;
	debug(debugType, "IMU FORCE - Self Test");
	/* in main.c : TIM16 permet de calculer la vitesse et la position   */
}
void subfunct_IMU_GET(char **argv) {
	/*
	 * argv None
	 */
	XYZ_t accPREV = accXYZ;
	XYZ_t vitPREV = vitXYZ;

	accXYZ = ADXL343_getAcc();

	vitXYZ = (XYZ_t ) { accPREV.X - accXYZ.X, accPREV.Y - accXYZ.Y, accPREV.Z
		- accXYZ.Z };
	posXYZ = (XYZ_t ) { vitPREV.X - vitPREV.X, vitPREV.Y - vitPREV.Y, vitPREV.Z
		- vitPREV.Z };
}
void subfunct_MIAOU(char **argv) {
	debug(INFORMATION,"MIAOU");
	play_song();
	return;
}
void subfunct_modify_calc_speed(char**argv){
	MDriver_t* MDriver;
	MDriver_Config_t* MDriver_Config;

	uint8_t driver_id = (uint8_t) strtol(argv[1], NULL, 10); // Base 10
	char* sens_motor = argv[2];
	int32_t offset_user = (int32_t) strtol(argv[3], NULL, 10); //Prends des valeurs entre -128 et 127
	MDriver = driver_id==1 ? &MDriver1 : &MDriver2;
	MDriver_Config = strcmp(sens_motor, "FWD") == 0 ? MDriver->FWD : MDriver->REV;
	MDriver_Config->offset=offset_user;

}

void subfunct_lidar(char**argv){
	if(argv[1]==NULL){
		LIDAR_start_scan_dma(&lidar) == 0 ? debug(START,"LIDAR") : debug(D_ERROR,"LIDAR");
		lidarDebugShell = 1;
	}
	else{
		strcmp(argv[1], "-h") ==0 ? LIDAR_get_health_stat(&lidar):(void)0;
		strcmp(argv[1], "-r") ==0 ? LIDAR_restart(&lidar):(void)0;
		strcmp(argv[1], "-i") ==0 ? LIDAR_get_info(&lidar):(void)0;
		strcmp(argv[1], "-s") ==0 ? HAL_GPIO_TogglePin(GPIOA,LIDAR_M_CTR_Pin):(void)0;

		lidarDebugShell = strcmp(argv[1], "-debug") == 0 ? 1-lidarDebugShell:lidarDebugShell;

	}
}
void reset(char **argv){
    __disable_irq(); // Désactive les interruptions globales
    NVIC_SystemReset(); // Demande un reset système via le NVIC
}
/************************************************************************************************
 * 										DEBUG
 *************************************************************************************************/

void debug(char *debugType, char *message) {

	printf("%-15s%s| %s\r\n", debugType, D_RESET, message);

}
