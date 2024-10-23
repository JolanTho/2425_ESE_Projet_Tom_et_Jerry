/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : app_freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "shell.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void shell(void * pvParameters);
void edgedetection(void * pvParameters);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* definition and creation of defaultTask */
	osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
	defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

	/* USER CODE BEGIN RTOS_THREADS */
#define STACK_SIZE 1000
#define DELAY_1 4
#define DELAY_2 4
	BaseType_t xReturned;
	TaskHandle_t xShellHandle = NULL;
	TaskHandle_t xEdgeDetectionHandle = NULL;


	xReturned = xTaskCreate(
			shell, /* Function that implements the task. */
			"Shell_Task", /* Text name for the task. */
			STACK_SIZE, /* Stack size in words, not bytes. */
			(void *) DELAY_1, /* Parameter passed into the task. */
			1,/* Priority at which the task is created. */
			&xShellHandle ); /* Used to pass out the created task's handle. */
	xReturned = xTaskCreate(
			edgedetection, /* Function that implements the task. */
			"EdgeDetection_Task", /* Text name for the task. */
			STACK_SIZE, /* Stack size in words, not bytes. */
			(void *) DELAY_2, /* Parameter passed into the task. */
			1,/* Priority at which the task is created. */
			&xEdgeDetectionHandle ); /* Used to pass out the created task's handle. */
	if (xReturned){
		Error_Handler();
	}
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
	/* USER CODE BEGIN StartDefaultTask */
	/* Infinite loop */

	/* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/*Si erreur de compilation ici, il faut include le path du driver dans les propiété du projet
* Project->Properties->C/C++ Build->Settings->MCU/MPU GCC Compiler->Include paths
* -> dans la section "include paths" ajouter "../shell"
*/
void shell(void * pvParameters){
	shell_init();
	shell_add('f', fonction, "Une fonction inutile");
	shell_add('a', addition, "Une fonction sommeuse");
	for(;;){
		shell_run();
	}
}
void edgedetection(void * pvParameters){
}
/* USER CODE END Application */

