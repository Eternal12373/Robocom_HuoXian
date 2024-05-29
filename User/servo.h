/*  
 *  Nanjing JIYU Technology Co.,Ltd All rights reserved.
 *  Copyright (C) 2023 
 *  ==============================================================
 * 
 * servo.c 
 *    舵机控制功能的实现
 *
 *  --------------------------------------------------------------
 *  
 */

#ifndef _SERVO_H
#define _SERVO_H

#include "main.h"

void ServoInit(void);
unsigned char ServoUp(void);
void ServoDown(void);
void ServoGrab(void);
void ServoRelease(void);

#endif
