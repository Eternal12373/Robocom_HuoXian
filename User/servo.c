/*  
 *  Nanjing JIYU Technology Co.,Ltd All rights reserved.
 *  Copyright (C) 2022 
 *  ==============================================================
 * 
 * servo.c 
 *    舵机控制功能的实现
 *
 *  --------------------------------------------------------------
 *  
 */

#include "servo.h"

/**
 * @brief 初始化舵机控制
 * 
 * 该函数用于初始化舵机控制，包括启动定时器4的PWM波输出，并设置初始的PWM占空比。
 */
void ServoInit(void)                                     
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);    // 开启定时器4的PWM波输出
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);    // 开启定时器4的PWM波输出
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);    // 开启定时器4的PWM波输出
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);    // 开启定时器4的PWM波输出
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1700);    // 设置初始的PWM占空比为1500
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 1500);    // 设置初始的PWM占空比为1000
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 1500);    // 设置初始的PWM占空比为1500
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 1500);    // 设置初始的PWM占空比为1500
}

/**
 * @brief 控制舵机抓取
 * 
 * 该函数用于控制舵机进行抓取动作，设置舵机2的PWM占空比为1700。
 */
void ServoGrab(void)                                     
{
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 2500);    // 设置舵机2的PWM占空比为1700
}

/**
 * @brief 控制舵机释放
 * 
 * 该函数用于控制舵机进行释放动作，设置舵机2的PWM占空比为1000。
 */
void ServoRelease(void)                                     
{
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 1500);    // 设置舵机2的PWM占空比为1000
}

/**
 * @brief 控制舵机上升
 * 
 * 该函数用于控制舵机上升，根据上方的接近开关状态确定舵机1的PWM占空比。
 * 如果接近开关未检测到障碍物，则将舵机1的PWM占空比设置为1900，表示上升；
 * 如果接近开关检测到障碍物，则将舵机1的PWM占空比设置为1500，表示停止上升。
 * 
 * @return unsigned char 如果上方有障碍物返回0，否则返回1。
 */
unsigned char ServoUp(void)                                     
{
    if(HAL_GPIO_ReadPin(SpacingUp_GPIO_Port, SpacingUp_Pin)){    // 如果上方有障碍物
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1900);    // 设置舵机1的PWM占空比为1900，表示上升
        return 0;    // 返回0表示上方有障碍物
    }
    else {
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1500);    // 设置舵机1的PWM占空比为1500，表示停止上升
        return 1;    // 返回1表示上方没有障碍物
    }
}

/**
 * @brief 控制舵机下降
 * 
 * 该函数用于控制舵机下降，将舵机1的PWM占空比设置为1200。
 */
void ServoDown(void)                                     
{
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1200);    // 设置舵机1的PWM占空比为1200，表示下降
}
