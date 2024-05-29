/*  
 *  Nanjing JIYU Technology Co.,Ltd All rights reserved.
 *  Copyright (C) 2023 
 *  ==============================================================
 * 
 *  track.h 
 *    底盘循迹的实现
 *
 *  --------------------------------------------------------------
 *  
 */

#ifndef _TRACK_H
#define _TRACK_H

#include "main.h"

enum CrossingType{
        Crossing = 0,
        CrossRight,
	      CrossLeft,
	      CrossNum,
    };

float LineError(void);
		
unsigned char CrossDetect(unsigned char Cross);
	
float filter(float p ,float new_value, float* last_value);

#endif




