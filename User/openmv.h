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

#ifndef __OPENMV_H
#define __OPENMV_H

#include "main.h"


/* 向上位机发送信息 */
typedef struct
{
    int x;
    int y;
	  int h;
	  int m;
}OpenmvTypeDef;

void OpenmvInit(void);

void HAL_UART3_RxCpltCallback(UART_HandleTypeDef *huart);

void USER_UART3_IRQHandler(UART_HandleTypeDef *huart);

int ByteToInt(char a);


#endif



