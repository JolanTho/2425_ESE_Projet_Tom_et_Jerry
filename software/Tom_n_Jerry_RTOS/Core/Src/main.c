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
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "ucpd.h"
#include "usart.h"
#include "usbpd.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "callBack.h"
#include "asserv.h"
#include "changeMode.h"
#include "lidar.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TASK_STACK_DEPTH_SHELL 2048
#define TASK_PRIORITY_SHELL 5

#define TASK_STACK_DEPTH_ASSERV_I 64
#define TASK_PRIORITY_ASSERV_I 2

#define TASK_STACK_DEPTH_ASSERV_XYZ 64
#define TASK_PRIORITY_ASSERV_XYZ 3

#define TASK_STACK_DEPTH_CHANGEMODE 64
#define TASK_PRIORITY_CHANGEMODE 1

#define TASK_STACK_DEPTH_LIDAR 512
#define TASK_PRIORITY_LIDAR 4
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
HeapStats_t heapStats;

TaskHandle_t h_task_shell = NULL;
TaskHandle_t h_task_asserv_I = NULL;
TaskHandle_t h_task_asserv_XYZ = NULL;
TaskHandle_t h_task_changemenMode = NULL;
TaskHandle_t h_task_LIDAR_Take = NULL;
TaskHandle_t h_task_LIDAR_Process = NULL;


SemaphoreHandle_t semb_halfCllbck;
SemaphoreHandle_t semb_cpltCllbck;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_ADC2_Init();
  MX_DAC1_Init();
  MX_I2C1_Init();
  MX_I2C3_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_UCPD1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM15_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */

	semb_cpltCllbck = xSemaphoreCreateBinary();
	semb_halfCllbck = xSemaphoreCreateBinary();

	asserv_init();
	shell_init();
	lidar_init();
	changement_mode_init();

	vPortGetHeapStats(&heapStats);
	xTaskCreate(changeMode_run,			"Changement MODE", TASK_STACK_DEPTH_CHANGEMODE, NULL, TASK_PRIORITY_CHANGEMODE, &h_task_changemenMode) != pdPASS ? Error_Handler():(void)0;
	vPortGetHeapStats(&heapStats);
	//xTaskCreate(asserv_courant_run, 	"Asserv Courant", TASK_STACK_DEPTH_ASSERV_I, NULL, TASK_PRIORITY_ASSERV_I, &h_task_asserv_I) != pdPASS ? Error_Handler():(void)0;
	//vPortGetHeapStats(&heapStats);
	//xTaskCreate(asserv_position_run, 	"Asserv Position", TASK_STACK_DEPTH_ASSERV_XYZ, NULL, TASK_PRIORITY_ASSERV_XYZ, &h_task_asserv_XYZ) != pdPASS ? Error_Handler():(void)0;
	vPortGetHeapStats(&heapStats);
	xTaskCreate(shell_run,				"Shell", TASK_STACK_DEPTH_SHELL, NULL, TASK_PRIORITY_SHELL, &h_task_shell) != pdPASS ? Error_Handler():(void)0;
	vPortGetHeapStats(&heapStats);
	xTaskCreate(lidarTake,				"LIDAR_Take", TASK_STACK_DEPTH_LIDAR, NULL, TASK_PRIORITY_LIDAR, &h_task_LIDAR_Take) != pdPASS ? Error_Handler():(void)0;
	vPortGetHeapStats(&heapStats);
	xTaskCreate(lidarprocess,			"LIDAR_Process", TASK_STACK_DEPTH_LIDAR, NULL, TASK_PRIORITY_LIDAR, &h_task_LIDAR_Process) != pdPASS ? Error_Handler():(void)0;
	vPortGetHeapStats(&heapStats);

	vTaskStartScheduler();
  /* USER CODE END 2 */

  /* USBPD initialisation ---------------------------------*/
  MX_USBPD_Init();

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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM17 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
	CUSTOM_TIM_PeriodElapsedCallback(htim);
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM17) {
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
