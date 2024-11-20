/*
 * shellv2.c
 *
 *  Created on: Nov 14, 2024
 *      Author: romingo
 */

#include "shellv2.h"

extern SemaphoreHandle_t sem_uart_read;


extern XYZ_t accXYZ;
extern XYZ_t vitXYZ;
extern XYZ_t posXYZ;
extern IMURegister_t IMURegister[];
extern GPIOExpanderRegister_t GPIOExpRegister[];

#define NUM_CHANNEL_ADC2 2


MAPPER mapping[] = { { "help", "Print every function available", "None",
		subfunct_help }, { "start", "Start the robot", "None", subfunct_start },
		{ "stop", "Stop the robot", "None", subfunct_stop },
		{ "speed", "Change speed motor", "int:MotorID, int:speed",subfunct_speed },
		{ "led", "Change params of the LEDs", "int: LedId	int: %PWM(0-255)", subfunct_setLed },
		{ "IMU","None", "None", subfunct_seeIMU },
		{ "IMUsf","Self test of the IMU", "None", subfunct_IMU_SelfTest },
		{"IMUu", "Read register", "None", subfunct_IMU_Update },
		{"IMUg", "Get Acc Value", "None", subfunct_IMU_GET },
		{"Iasserv","Start asservissement courant","None",subfunct_Iasserv},
		{ "miaou","Play song", "None", subfunct_MIAOU },
		{ "clear","Clear screen", "None", subfunct_clear },
};
uint8_t started[] = "\r\n*-----------------------------*"
		"\r\n|     Welcome on Tom&Jerry    |"
		"\r\n*-----------------------------*"
		"\r\n";
uint8_t newline[] = "\r\n";
uint8_t backspace[] = "\b \b";
uint8_t cmdNotFound[] = "Command not found\r\n";
uint8_t prompt[] = "user@Tom&Jerry>>";
uint8_t clear[] = "\x1b[2J\x1b[H";
uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE];
uint8_t uartTxBuffer[UART_TX_BUFFER_SIZE];

uint32_t adc2_values_VAL[2]={0,0};

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

}

void shell_run(void*) {
	int reading = 0;
	for (;;) {
		reading = 1;
		idx_cmd = 0;
		while (reading) {
			HAL_UART_Receive_IT(&UART_DEVICE, uartRxBuffer,
					UART_RX_BUFFER_SIZE);
			xSemaphoreTake(sem_uart_read, portMAX_DELAY);

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
void subfunct_start(char **argv) {
	HAL_TIM_Base_Start_IT(&htim15) == HAL_OK ?
			debug(START, "TIMER 15 - CALCUL IMU") : (void) 0;
	HAL_TIM_Base_Start(&htim2) == HAL_OK ?
			debug(START, "TIMER 2") : debug(D_ERROR, "TIMER 2");
	HAL_TIM_Base_Start(&htim3) == HAL_OK ?
			debug(START, "TIMER 3") : debug(D_ERROR, "TIMER 3");
	HAL_ADCEx_Calibration_Start(&hadc2,ADC_SINGLE_ENDED) == HAL_OK ?
			debug(START, "ADC2 CALIBRATION") : debug(D_ERROR, "ADC2 CALIBRATION");

	//	LP5812_Init();
	ADXL343_init();
	ZXB5210_init();
	return;
}
void subfunct_stop(char **argv) {
	HAL_TIM_Base_Stop(&htim15) == HAL_OK ?
			debug(STOP, "TIMER 15 - CALCUL IMU") : (void) 0;
	HAL_TIM_Base_Stop(&htim2) == HAL_OK ?
			debug(STOP, "TIMER 2") : debug(D_ERROR, "TIMER 2");
	HAL_TIM_Base_Stop(&htim3) == HAL_OK ?
			debug(STOP, "TIMER 3") : debug(D_ERROR, "TIMER 3");
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
	uint8_t alpha = (uint8_t) strtol(argv[1], NULL, 10); // Base 10
	ZXB5210_speed_REV(&htim3, alpha);
	ZXB5210_speed_FWD(&htim2, alpha);

	return;
}
void subfunct_Iasserv(char **argv) {
	HAL_ADC_Start_DMA(&hadc2,(uint32_t*)adc2_values_VAL,NUM_CHANNEL_ADC2) == HAL_OK ?
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

	printf("accX: %-24i|accY: %-24i|accZ: %-24i\r\n", accXYZ.X, accXYZ.Y,
			accXYZ.Z);
	printf("1e3 vitX: %-20i|1e3 vitY: %-20i|1e3 vitZ: %-20i\r\n", vitXYZ.X,
			vitXYZ.Y, vitXYZ.Z);
	printf("1e3 posX: %-20i|1e3 posY: %-20i|1e3 posZ: %-20i\r\n", posXYZ.X,
			posXYZ.Y, posXYZ.Z);
	printf(separator);
}
void subfunct_MIAOU(char **argv) {
	return;
}
void subfunct_clear(char **argv) {
	HAL_UART_Transmit(&UART_DEVICE, clear, sizeof(clear), HAL_MAX_DELAY);
}

/************************************************************************************************
 * 										DEBUG
 *************************************************************************************************/

void debug(char *debugType, char *message) {

	printf("%-15s%s| %s\r\n", debugType, D_RESET, message);

}
