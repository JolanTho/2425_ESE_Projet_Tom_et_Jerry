/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "drv_LIDAR.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define STACK_SIZE 256


#define DUREE 5000
#define DUREEP 30000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
h_LIDAR_t lidar;
SemaphoreHandle_t semb_halfCllbck;
SemaphoreHandle_t semb_cpltCllbck;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}

// Définir les fonctions UART pour le mode polling
int uart_transmit(uint8_t *p_data, uint16_t size) {
    return HAL_UART_Transmit(&huart4, p_data, size, HAL_MAX_DELAY);
}

int uart_poll_receive(uint8_t *p_data, uint16_t size) {
    return HAL_UART_Receive(&huart4, p_data, size, 100);
}

int uart_it_receive(uint8_t *p_data, uint16_t size){
	HAL_UART_Receive_IT(&huart4, p_data, size);
	return 0;
}

int uart_it_transmit(uint8_t *p_data, uint16_t size){
	HAL_UART_Transmit_IT(&huart4, p_data, size);
	return 0;
}

int uart_dma_receive(uint8_t *p_data, uint16_t size){
	HAL_UART_Receive_DMA(&huart4, p_data, size);
	return 0;
}

int uart_dma_transmit(uint8_t *p_data, uint16_t size){
	HAL_UART_Transmit_DMA(&huart4, p_data, size);
	return 0;
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void counter (void * pvParameters){
	char* s = pcTaskGetName(xTaskGetCurrentTaskHandle());

//
//	vTaskDelay(500);
//
//	  // Récupération des informations de l'appareil
//	  if (LIDAR_get_info(&lidar) == 0) {
//		  printf("LIDAR device information retrieved successfully.\r\n");
//	  } else {
//		  printf("Failed to retrieve LIDAR device information.\r\n");
//	  }
//
//	vTaskDelay(500);

//	  // Récupération de l'état de santé
//	  if (LIDAR_get_health_stat(&lidar) == 0) {
//		  printf("LIDAR health status retrieved successfully.\r\n");
//	  } else {
//		  printf("Failed to retrieve LIDAR health status.\r\n");
//	  }

//	vTaskDelay(500);

	while (1) {

		printf("Je suis la tache %s et je m'endors pour %d periodes\r\n", s, DUREEP);

		vTaskDelay(DUREEP);

		printf("LIDAR Scan Results (Angle: Distance in mm):\r\n");
	    for (int i = 0; i < 360; i++) {
	    		if (lidar.processing.point_buff[i] >= 0) { // Afficher uniquement les valeurs valides
	    			printf("Angle %3d: %4d mm\r\n", i, lidar.processing.point_buff[i]);
	    		} else {
	    			printf("Angle %3d: --- mm (No Data)\r\n", i);
	    		}
	    }

	    printf("\n");


		}
	}

void lidarTake (void * pvParameters){
	char* s = pcTaskGetName(xTaskGetCurrentTaskHandle());
	uint8_t* buff;

	  if (LIDAR_start_scan_dma(&lidar) == 0) {
	      printf("LIDAR scanning started successfully.\r\n");
	  } else {
	      printf("Failed to start LIDAR scanning.\r\n");
	  }

	for(;;){


		// Attente du Semahore de half buffer
		xSemaphoreTake(semb_halfCllbck, portMAX_DELAY);

		// Pointeur sur le début des données
		buff = lidar.processing.receive_buff;
		LIDAR_process_frame(&lidar, buff);


		// Attente du semaphore du complete buffer
		xSemaphoreTake(semb_cpltCllbck, portMAX_DELAY);

		// Pointeur sur la moitié des données
		buff = &lidar.processing.receive_buff[FRAME_BUFF_SIZE];
		LIDAR_process_frame(&lidar, buff);


		}
}

void lidarprocess (void * pvParameters){
	char* s = pcTaskGetName(xTaskGetCurrentTaskHandle());

	for(;;){
		printf("Je suis la tache %s et je m'endors pour %d periodes\r\n", s, DUREE);

	    medianFilter(&lidar);

	    find_clusters(&lidar);

	    printf("Clusters Maison trouvees : %d \r\n", lidar.processing.cluster_cnt);
//
//	    kMeansClustering(&lidar);
//
//	    printf("Clusters K-Means trouvees : %d \r\n", lidar.processing.cluster_cnt);

	    vTaskDelay(DUREE);
	}

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	BaseType_t xReturned;
	TaskHandle_t xHandle1 = NULL;
	TaskHandle_t xHandle2 = NULL;
	TaskHandle_t xHandle3 = NULL;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */
  printf("=================== DRIVER LIDAR =================== \r\n");

  /*
   * Allumage du LIDAR et mise en place de la rotation
   *
   */

  /*
   * Création des vérifications du lidar.
   *
   */

  semb_cpltCllbck = xSemaphoreCreateBinary();
  semb_halfCllbck = xSemaphoreCreateBinary();

	  // Déclaration et configuration de la structure h_LIDAR

	  lidar.serial_drv.transmit = uart_transmit;
	  lidar.serial_drv.poll_receive = uart_poll_receive;

	  lidar.serial_drv.it_receive=uart_it_receive;
	  lidar.serial_drv.it_transmit=uart_it_transmit;

	  lidar.serial_drv.dma_receive=uart_dma_receive;
	  lidar.serial_drv.dma_transmit=uart_dma_transmit;

	xReturned = xTaskCreate(
			lidarTake, // Function that implements the task.
			"lidarTake", // Text name for the task.
			STACK_SIZE, // Stack size in words, not bytes.
			(void *) NULL, // Parameter passed into the task.
			2,// Priority at which the task is created.
			&xHandle1 ); // Used to pass out the created task's handle.

	if (xReturned != pdPASS)
	{
		printf("Error creating Task\r\n");
		Error_Handler();
	}

	printf("Tasks created\r\n");

	xReturned = xTaskCreate(
			counter, // Function that implements the task.
			"counter", // Text name for the task.
			STACK_SIZE, // Stack size in words, not bytes.
			(void *) NULL, // Parameter passed into the task.
			tskIDLE_PRIORITY,// Priority at which the task is created.
			&xHandle2 ); // Used to pass out the created task's handle.

	if (xReturned != pdPASS)
	{
		printf("Error creating Task\r\n");
		Error_Handler();
	}

	printf("Tasks created\r\n");

	xReturned = xTaskCreate(
			lidarprocess, // Function that implements the task.
			"LidarProcess", // Text name for the task.
			STACK_SIZE, // Stack size in words, not bytes.
			(void *) NULL, // Parameter passed into the task.
			tskIDLE_PRIORITY,// Priority at which the task is created.
			&xHandle3 ); // Used to pass out the created task's handle.

	if (xReturned != pdPASS)
	{
		printf("Error creating Task\r\n");
		Error_Handler();
	}

	printf("Tasks created\r\n");

	vTaskStartScheduler();
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in cmsis_os2.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {

    if (huart->Instance == UART4) {


    	BaseType_t higher_priority_task_woken = pdFALSE;
    	xSemaphoreGiveFromISR(semb_halfCllbck, &higher_priority_task_woken);

    	portYIELD_FROM_ISR(higher_priority_task_woken);

    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART4){

		BaseType_t higher_priority_task_woken = pdFALSE;
    	xSemaphoreGiveFromISR(semb_cpltCllbck, &higher_priority_task_woken);

    	portYIELD_FROM_ISR(higher_priority_task_woken);

    }
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
