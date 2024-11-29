/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

#include "stm32g4xx_ll_ucpd.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_cortex.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_system.h"
#include "stm32g4xx_ll_utils.h"
#include "stm32g4xx_ll_pwr.h"
#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_dma.h"

#include "stm32g4xx_ll_exti.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BUTTON_INITIAL_STATE_Pin GPIO_PIN_13
#define BUTTON_INITIAL_STATE_GPIO_Port GPIOC
#define BUTTON_INITIAL_STATE_EXTI_IRQn EXTI15_10_IRQn
#define BUTTON_START_ROBOT_Pin GPIO_PIN_14
#define BUTTON_START_ROBOT_GPIO_Port GPIOC
#define BUTTON_START_ROBOT_EXTI_IRQn EXTI15_10_IRQn
#define LIDAR_RXnTX_STM_Pin GPIO_PIN_2
#define LIDAR_RXnTX_STM_GPIO_Port GPIOA
#define LIDAR_TXnRX_STM_Pin GPIO_PIN_3
#define LIDAR_TXnRX_STM_GPIO_Port GPIOA
#define DAC__Pin GPIO_PIN_4
#define DAC__GPIO_Port GPIOA
#define ASSERV_I1_Pin GPIO_PIN_6
#define ASSERV_I1_GPIO_Port GPIOA
#define ASSERV_I2_Pin GPIO_PIN_7
#define ASSERV_I2_GPIO_Port GPIOA
#define VCP_TX_Pin GPIO_PIN_4
#define VCP_TX_GPIO_Port GPIOC
#define DRIVER_FWD2_Pin GPIO_PIN_0
#define DRIVER_FWD2_GPIO_Port GPIOB
#define DRIVER_REV2_Pin GPIO_PIN_1
#define DRIVER_REV2_GPIO_Port GPIOB
#define DRIVER_REV1_Pin GPIO_PIN_10
#define DRIVER_REV1_GPIO_Port GPIOB
#define DRIVER_FWD1_Pin GPIO_PIN_11
#define DRIVER_FWD1_GPIO_Port GPIOB
#define MOUSTACHE_1_Pin GPIO_PIN_15
#define MOUSTACHE_1_GPIO_Port GPIOB
#define MOUSTACHE_1_EXTI_IRQn EXTI15_10_IRQn
#define I2C_MAIN_SCL_Pin GPIO_PIN_8
#define I2C_MAIN_SCL_GPIO_Port GPIOA
#define LIDAR_M_CTR_Pin GPIO_PIN_11
#define LIDAR_M_CTR_GPIO_Port GPIOA
#define GPIO_EXP_EXTI_Pin GPIO_PIN_12
#define GPIO_EXP_EXTI_GPIO_Port GPIOA
#define GPIO_EXP_EXTI_EXTI_IRQn EXTI15_10_IRQn
#define I2C_IMU_SCL_Pin GPIO_PIN_15
#define I2C_IMU_SCL_GPIO_Port GPIOA
#define I2C_MAIN_SDA_Pin GPIO_PIN_11
#define I2C_MAIN_SDA_GPIO_Port GPIOC
#define VCP_RX_Pin GPIO_PIN_7
#define VCP_RX_GPIO_Port GPIOB
#define I2C_IMU_SDA_Pin GPIO_PIN_9
#define I2C_IMU_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
