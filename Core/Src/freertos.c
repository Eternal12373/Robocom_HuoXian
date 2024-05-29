/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
/* Definitions for OpenmvTask */
osThreadId_t OpenmvTaskHandle;
const osThreadAttr_t OpenmvTask_attributes = {
  .name = "OpenmvTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MoveTask */
osThreadId_t MoveTaskHandle;
const osThreadAttr_t MoveTask_attributes = {
  .name = "MoveTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for ImuTask */
osThreadId_t ImuTaskHandle;
const osThreadAttr_t ImuTask_attributes = {
  .name = "ImuTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for RunTask */
osThreadId_t RunTaskHandle;
const osThreadAttr_t RunTask_attributes = {
  .name = "RunTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for ImuIntSem */
osSemaphoreId_t ImuIntSemHandle;
const osSemaphoreAttr_t ImuIntSem_attributes = {
  .name = "ImuIntSem"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void OpenmvTask_Function(void *argument);
void MoveTask_Function(void *argument);
void ImuTask_Function(void *argument);
void RunTask_Function(void *argument);

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

  /* Create the semaphores(s) */
  /* creation of ImuIntSem */
  ImuIntSemHandle = osSemaphoreNew(1, 1, &ImuIntSem_attributes);

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
  /* creation of OpenmvTask */
  OpenmvTaskHandle = osThreadNew(OpenmvTask_Function, NULL, &OpenmvTask_attributes);

  /* creation of MoveTask */
  MoveTaskHandle = osThreadNew(MoveTask_Function, NULL, &MoveTask_attributes);

  /* creation of ImuTask */
  ImuTaskHandle = osThreadNew(ImuTask_Function, NULL, &ImuTask_attributes);

  /* creation of RunTask */
  RunTaskHandle = osThreadNew(RunTask_Function, NULL, &RunTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_OpenmvTask_Function */
/**
  * @brief  Function implementing the OpenmvTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_OpenmvTask_Function */
__weak void OpenmvTask_Function(void *argument)
{
  /* USER CODE BEGIN OpenmvTask_Function */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END OpenmvTask_Function */
}

/* USER CODE BEGIN Header_MoveTask_Function */
/**
* @brief Function implementing the MoveTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MoveTask_Function */
__weak void MoveTask_Function(void *argument)
{
  /* USER CODE BEGIN MoveTask_Function */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END MoveTask_Function */
}

/* USER CODE BEGIN Header_ImuTask_Function */
/**
* @brief Function implementing the ImuTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ImuTask_Function */
__weak void ImuTask_Function(void *argument)
{
  /* USER CODE BEGIN ImuTask_Function */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ImuTask_Function */
}

/* USER CODE BEGIN Header_RunTask_Function */
/**
* @brief Function implementing the RunTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_RunTask_Function */
__weak void RunTask_Function(void *argument)
{
  /* USER CODE BEGIN RunTask_Function */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END RunTask_Function */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

