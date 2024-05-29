/*  
 *  Nanjing JIYU Technology Co.,Ltd All rights reserved.
 *  Copyright (C) 2023 
 *  ==============================================================
 * 
 *  gray.c 
 *    灰度传感器功能的实现
 *
 *  --------------------------------------------------------------
 *  
 */

#include "gray.h"

/**
 * @brief 获取灰度传感器数据
 * 
 * @param g 用于存储灰度传感器数据的数组
 */
void get_gray(unsigned char *g)
{
	g[0]=HAL_GPIO_ReadPin(GRAY_F_0_GPIO_Port,GRAY_F_0_Pin);	// 读取前向传感器0的值
	g[1]=HAL_GPIO_ReadPin(GRAY_F_1_GPIO_Port,GRAY_F_1_Pin);	// 读取前向传感器1的值
	g[2]=HAL_GPIO_ReadPin(GRAY_F_2_GPIO_Port,GRAY_F_2_Pin);	// 读取前向传感器2的值
	g[3]=HAL_GPIO_ReadPin(GRAY_F_3_GPIO_Port,GRAY_F_3_Pin);	// 读取前向传感器3的值
	g[4]=HAL_GPIO_ReadPin(GRAY_F_4_GPIO_Port,GRAY_F_4_Pin);	// 读取前向传感器4的值
	g[5]=HAL_GPIO_ReadPin(GRAY_F_5_GPIO_Port,GRAY_F_5_Pin);	// 读取前向传感器5的值
	g[6]=HAL_GPIO_ReadPin(GRAY_F_6_GPIO_Port,GRAY_F_6_Pin);	// 读取前向传感器6的值
	g[7]=HAL_GPIO_ReadPin(GRAY_F_7_GPIO_Port,GRAY_F_7_Pin);	// 读取前向传感器7的值
	g[8]=HAL_GPIO_ReadPin(GRAY_F_8_GPIO_Port,GRAY_F_8_Pin);	// 读取前向传感器8的值
	g[9]=HAL_GPIO_ReadPin(GRAY_B_0_GPIO_Port,GRAY_B_0_Pin);	// 读取后向传感器0的值
    g[10]=HAL_GPIO_ReadPin(GRAY_B_1_GPIO_Port,GRAY_B_1_Pin);	// 读取后向传感器1的值
	g[11]=HAL_GPIO_ReadPin(GRAY_B_2_GPIO_Port,GRAY_B_2_Pin);	// 读取后向传感器2的值
	//printf("x1=%d x2=%d x3=%d x4=%d x5=%d x6=%d x7=%d x8=%d x9=%d x10=%d x11=%d x12=%d\r\n",g[0],g[1],g[2],g[3],g[4],g[5],g[6],g[7],g[8],g[9],g[10],g[11]);   
}
