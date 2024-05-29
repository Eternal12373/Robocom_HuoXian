/*  
 *  Nanjing JIYU Technology Co.,Ltd All rights reserved.
 *  Copyright (C) 2023 
 *  ==============================================================
 * 
 *    openmv.c 
 *    OPENMV通信和指令处理
 *
 *  --------------------------------------------------------------
 *  
 */

#include "openmv.h"

uint8_t receive_buff3[18];  // 接收缓冲区
extern DMA_HandleTypeDef hdma_usart3_rx;  // 串口3的DMA句柄
extern UART_HandleTypeDef huart3;  // 串口3句柄
extern UART_HandleTypeDef huart4;  // 串口4句柄

OpenmvTypeDef Openmv={0,0,0,0};  // 定义OPENMV结构体实例，用于存储OPENMV相关数据

/**
 * @brief OPENMV模块初始化
 * 
 * 该函数用于初始化OPENMV模块，包括启动串口接收DMA并使能空闲中断。
 */
void OpenmvInit(void)
{	
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);  // 使能串口3空闲中断
	HAL_UART_Receive_DMA(&huart3, (uint8_t*)receive_buff3, 18);  // 启动串口3 DMA接收
}

/**
 * @brief 串口中断处理函数
 * 
 * @param huart 串口句柄
 */
void USER_UART3_IRQHandler(UART_HandleTypeDef *huart)
{
    if(USART3 == huart3.Instance)  // 判断是否是串口3
    {
        if(RESET != __HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE))  // 判断是否是空闲中断
        {
            __HAL_UART_CLEAR_IDLEFLAG(&huart3);  // 清除空闲中断标志
            HAL_UART3_RxCpltCallback(huart);  // 调用中断处理函数
        }
    }
}

/**
 * @brief 串口接收完成回调函数
 * 
 * @param huart 串口句柄
 */
void HAL_UART3_RxCpltCallback(UART_HandleTypeDef *huart)
{
    HAL_UART_DMAStop(&huart3);  // 停止DMA传输
    uint8_t data_length  = 18 - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);  // 计算接收到的数据长度
    if(receive_buff3[0]==0xA5 && receive_buff3[17]==0x5A){  // 检查数据帧头尾
			memcpy(&Openmv, &receive_buff3[1], 16);  // 将接收到的数据存入OPENMV结构体
		}
    memset(receive_buff3,0,data_length);  // 清空接收缓冲区
    data_length = 0;
    HAL_UART_Receive_DMA(&huart3, (uint8_t*)receive_buff3, 18);  // 重启DMA接收
}

/**
 * @brief 将补码形式的字符转换为整型
 * 
 * @param a 补码形式的字符
 * @return int 转换后的整型数据
 */
int ByteToInt(char a)
{
	int data;
	if ((a & 0x7F) == a) return a;  // 判断是否为正数
	else
	{
		data = a - 0x01;  // 减去补码
		data = data ^ 0xFF;  // 反码得到负数的绝对值
		return data * -1;  // 返回负数
	}
}
