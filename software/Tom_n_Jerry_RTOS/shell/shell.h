/*
 * shellv2.h
 *
 *  Created on: Nov 14, 2024
 *      Author: romingo
 */

#ifndef SHELL_H_
#define SHELL_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"
#include "gpio.h"
#include "tim.h"
#include "adc.h"
#include "cmsis_os.h"
#include "string.h"
#include "components/LP5812.h"
#include "components/ADXL343.h"
#include "components/TCA9555.h"
#include "components/ZXB5210.h"

#define UART_DEVICE huart1

#define MAX_ARGS 8
#define CMD_BUFFER_SIZE 64
#define UART_RX_BUFFER_SIZE 1
#define UART_TX_BUFFER_SIZE 64

void shell_init();
void shell_run(void*);
void task_shell(void *unused);

void subfunct_help(char **argv);
void subfunct_speed(char **argv);
void subfunct_start(char **argv);
void subfunct_stop(char **argv);
void subfunct_setLed(char **argv);
void subfunct_seeIMU(char **argv);
void subfunct_IMU_SelfTest(char **argv);
void subfunct_IMU_Update(char **argv);
void subfunct_IMU_GET(char **argv);
void subfunct_Iasserv(char **argv);
void subfunct_MIAOU(char **argv);
void subfunct_clear(char **argv);

void debug(char *debugType, char *message);

typedef struct {
	char *name;
	char *resume;
	char *params;
	void (*funct)(char**);
} MAPPER;

#define ASCII_LF 0x0A			// LF = line feed, saut de ligne
#define ASCII_CR 0x0D			// CR = carriage return, retour chariot
#define ASCII_BACK 0x08			// BACK = Backspace
#define ASCII_BACK_BIS 0x7F
#define ASCII_ESC_1 0x1B
#define ASCII_ESC_2 0x5B
#define ASCII_ARROW_UP 0x41
#define ASCII_ARROW_DOWN 0x42

/************************************************************************************************
 * 										DEBUG
 *************************************************************************************************/
#define separator "--------------------------------------------------------------------------\r\n"
#define INFORMATION 	"\033[35mINFORMATION"   // Code couleur Magenta
#define WARNING    		"\033[33mWARNING"    // Code couleur Jaune
#define START 			"\033[32mSTART"
#define OK 				"\033[32mOK"
#define STOP   			"\033[31mSTOP"    // Code couleur Rouge
#define D_ERROR   		"\033[31mERROR"    // Code couleur Rouge
#define GRAS 			"\033[1m"
#define FUNCTION 		"\033[35m"
#define D_RESET 		"\033[0m"         // Réinitialise la couleur

#endif /* SHELL_H_ */

