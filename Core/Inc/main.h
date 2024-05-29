/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "freertos.h"
#include "usart.h"
#include "spi.h"
#include "dma.h"
#include "tim.h"

#include "motor.h"
#include "servo.h"
#include "track.h"
#include "pid.h"
#include "gray.h"
#include "openmv.h"
#include "mpu6500.h"

#include "MoveTask.h"
#include "ImuTask.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define	u8  unsigned char
#define u16 unsigned short int
#define	u32 unsigned int
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MotorLeftDir1_Pin GPIO_PIN_0
#define MotorLeftDir1_GPIO_Port GPIOF
#define MotorLeftDir2_Pin GPIO_PIN_2
#define MotorLeftDir2_GPIO_Port GPIOF
#define MotorEN_Pin GPIO_PIN_4
#define MotorEN_GPIO_Port GPIOF
#define MotorRightDir1_Pin GPIO_PIN_1
#define MotorRightDir1_GPIO_Port GPIOC
#define MotorRightDir2_Pin GPIO_PIN_3
#define MotorRightDir2_GPIO_Port GPIOC
#define SpacingUp_Pin GPIO_PIN_5
#define SpacingUp_GPIO_Port GPIOA
#define GRAY_B_0_Pin GPIO_PIN_4
#define GRAY_B_0_GPIO_Port GPIOC
#define GRAY_B_1_Pin GPIO_PIN_5
#define GRAY_B_1_GPIO_Port GPIOC
#define GRAY_B_2_Pin GPIO_PIN_0
#define GRAY_B_2_GPIO_Port GPIOB
#define GRAY_F_0_Pin GPIO_PIN_11
#define GRAY_F_0_GPIO_Port GPIOF
#define GRAY_F_1_Pin GPIO_PIN_12
#define GRAY_F_1_GPIO_Port GPIOF
#define GRAY_F_2_Pin GPIO_PIN_13
#define GRAY_F_2_GPIO_Port GPIOF
#define GRAY_F_3_Pin GPIO_PIN_14
#define GRAY_F_3_GPIO_Port GPIOF
#define GRAY_F_4_Pin GPIO_PIN_15
#define GRAY_F_4_GPIO_Port GPIOF
#define GRAY_F_5_Pin GPIO_PIN_0
#define GRAY_F_5_GPIO_Port GPIOG
#define GRAY_F_6_Pin GPIO_PIN_1
#define GRAY_F_6_GPIO_Port GPIOG
#define GRAY_F_7_Pin GPIO_PIN_7
#define GRAY_F_7_GPIO_Port GPIOE
#define GRAY_F_8_Pin GPIO_PIN_8
#define GRAY_F_8_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
