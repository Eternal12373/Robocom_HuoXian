/*  
 *  Nanjing JIYU Technology Co.,Ltd All rights reserved.
 *  Copyright (C) 2022 
 *  ==============================================================
 * 
 *  ImuTask.h
 *    Imu姿态解算任务
 *
 *  --------------------------------------------------------------
 *  
 */
 
#include "ImuTask.h"

/**
 * @brief Imu任务函数
 * 
 * @param argument 任务参数
 */
void ImuTask_Function(void const * argument)
{
	portTickType tick;
    while(1)
    {
		tick = xTaskGetTickCount();  // 获取当前系统时间
        mpu_get_data();        // 获取陀螺仪数据
		imu_ahrs_update();     // 更新航姿参考
	    imu_attitude_update(); // 姿态解算
		vTaskDelayUntil(&tick,1);  // 延时1个系统时钟周期
    }

}
