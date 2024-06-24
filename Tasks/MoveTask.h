/*  
 *  Nanjing JIYU Technology Co.,Ltd All rights reserved.
 *  Copyright (C) 2022 
 *  ==============================================================
 * 
 *     MoveTask.h
 *    小车运动控制
 *
 *  --------------------------------------------------------------
 *  
 */

#ifndef _MOVETASK_H
#define _MOVETASK_H

#include "main.h"

#define ANGLE_SPEED_PID_KP 15.0f
#define ANGLE_SPEED_PID_KI 0.5f
#define ANGLE_SPEED_PID_KD 100.0f
#define ANGLE_SPEED_PID_MAX_OUT 1000
#define ANGLE_SPEED_PID_MAX_IOUT 100

void chassis_init(void);
	
enum ModeType{
        Stop = 0,
	      OpenLoop,
        Drive,
	      DetectLine,
        TurnAngle,
        ModeTypeMax,
    };

		
#endif




