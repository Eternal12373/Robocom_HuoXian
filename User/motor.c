/*  
 *  Nanjing JIYU Technology Co.,Ltd All rights reserved.
 *  Copyright (C) 2022 
 *  ==============================================================
 * 
 *  motor.c 
 *    电机控制功能的实现
 *
 *  --------------------------------------------------------------
 *  
 */

#include "motor.h"

extern TIM_HandleTypeDef htim2;

void MotorInit(void){
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);                            //开启定时器2PWM波输出
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);                            //开启定时器2PWM波输出
}
/*-----------------------------  TB6612,电机方向和端电压(PWM占空比)值  -------------------------------------*/

void MotorLeftSet(int p)                                     //左电机
{
	HAL_GPIO_WritePin(MotorEN_GPIO_Port, MotorEN_Pin, GPIO_PIN_SET);
	if (p < 0)                                                         //控制电机顺时针旋转
	{
		HAL_GPIO_WritePin(MotorLeftDir1_GPIO_Port, MotorLeftDir1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MotorLeftDir2_GPIO_Port, MotorLeftDir2_Pin, GPIO_PIN_RESET);
	}
	else                                                                //控制电机逆时针旋转
	{
		HAL_GPIO_WritePin(MotorLeftDir1_GPIO_Port, MotorLeftDir1_Pin, GPIO_PIN_RESET);              //BIN1
		HAL_GPIO_WritePin(MotorLeftDir2_GPIO_Port, MotorLeftDir2_Pin, GPIO_PIN_SET);            //BIN2
	}
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, abs(MotorLimit(p)));
}


void MotorRightSet(int p)                                     //右电机
{
	HAL_GPIO_WritePin(MotorEN_GPIO_Port, MotorEN_Pin, GPIO_PIN_SET);
	if (p < 0)                                                         //控制电机顺时针旋转
	{
		HAL_GPIO_WritePin(MotorRightDir1_GPIO_Port, MotorRightDir1_Pin, GPIO_PIN_RESET);               //AIN1
		HAL_GPIO_WritePin(MotorRightDir2_GPIO_Port, MotorRightDir2_Pin, GPIO_PIN_SET);             //AIN2
	}
	else                                                                //控制电机逆时针旋转
	{
		HAL_GPIO_WritePin(MotorRightDir1_GPIO_Port, MotorRightDir1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MotorRightDir2_GPIO_Port, MotorRightDir2_Pin, GPIO_PIN_RESET);
	}
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, abs(MotorLimit(p)));
}


int MotorLimit(int l)
{
	if(l>1000) return 1000;
	else if(l<-1000) return -1000;
	else return l;
}


