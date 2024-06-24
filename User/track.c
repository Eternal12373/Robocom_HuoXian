/*  
 *  Nanjing JIYU Technology Co.,Ltd All rights reserved.
 *  Copyright (C) 2023 
 *  ==============================================================
 * 
 *  track.c 
 *    底盘循迹的实现
 *
 *  --------------------------------------------------------------
 *  
 */

#include "track.h"



float es_value=0;



/**
 * @brief 检测交叉口或转弯
 * 
 * 该函数根据当前灰度传感器数组中的值检测交叉口或转弯情况。
 * 
 * @param Cross 检测类型，可以是 Crossing（交叉口）、CrossRight（右转弯）、CrossLeft（左转弯）或 CrossNum（灰度传感器值总和）
 * @return unsigned char 检测结果，如果符合条件则返回1，否则返回0
 */
unsigned char CrossDetect(unsigned char Cross){
    unsigned char gray[12];    // 灰度传感器数组，存储灰度值
    
    get_gray(gray);   // 获取灰度值
    
    // 根据检测类型进行不同的判断
    switch (Cross){
        case Crossing:
            if(gray[0]+gray[1]+gray[2]+gray[3]+gray[4]+gray[5]+gray[6]+gray[7]+gray[8] > 6) return 1;
            break;
        case CrossRight:
            if(gray[5]+gray[6]+gray[7]+gray[8] >= 2) return 1;
            break;
        case CrossLeft:
            if(gray[0]+gray[1]+gray[2]+gray[3] > 2) return 1;
            break;
        case CrossNum:
            return gray[0]+gray[1]+gray[2]+gray[3]+gray[4]+gray[5]+gray[6]+gray[7]+gray[8];
            break;
    }
    return 0;
}




float line_err=0;



/**
 * @brief 计算线路跟踪的误差值
 * 
 * 该函数根据灰度传感器数组中的值计算线路跟踪的误差值。
 * 
 * @return float 线路跟踪的误差值
 */
float LineError(){
    unsigned char su=0;        // 灰度传感器中灰度值为1的数量
    unsigned char gray[12];    // 灰度传感器数组，存储灰度值
	
    get_gray(gray);   // 获取灰度值
	
    // 计算灰度值为1的数量
    su = gray[0] + gray[1] + gray[2] + gray[3] + gray[4] + gray[5] + gray[6] + gray[7] + gray[8];

    // 根据灰度值的分布情况计算线路跟踪的误差值
    if(su == 2){
        if(gray[3] == 1 && gray[4] == 1) line_err = 1;
        else if(gray[4] == 1 && gray[5] == 1) line_err = -1;
        else if(gray[2] == 1 && gray[3] == 1) line_err = 3;
        else if(gray[5] == 1 && gray[6] == 1) line_err = -3;
        else if(gray[1] == 1 && gray[2] == 1) line_err = 5;
        else if(gray[6] == 1 && gray[7] == 1) line_err = -5;
        else if(gray[0] == 1 && gray[1] == 1) line_err = 7;
        else if(gray[7] == 1 && gray[8] == 1) line_err = -7;
    }
    else if(su == 1){
        if(gray[4] == 1) line_err = 0;
        else if(gray[3] == 1) line_err = 2;
        else if(gray[5] == 1) line_err = -2;
        else if(gray[2] == 1) line_err = 4;
        else if(gray[6] == 1) line_err = -4;
        else if(gray[1] == 1) line_err = 6;
        else if(gray[7] == 1) line_err = -6;
        else if(gray[0] == 1) line_err = 8;
        else if(gray[8] == 1) line_err = -8;
    }
    else if(su == 0){
        if(line_err > 6) line_err = 10;
        else if(line_err < -6) line_err = -10;
    }
    else if(su > 3) line_err = 0;
    
    // 对误差值进行滤波处理，使其更加稳定
    line_err = filter(0.1, line_err, &es_value);
    
    // 返回线路跟踪的误差值
    return line_err;
}



/**
 * @brief 对输入值进行指数平滑滤波处理
 * 
 * @param p 平滑因子，控制滤波器对新值的响应速度，取值范围 [0, 1]
 * @param new_value 新的输入值
 * @param last_value 上一次滤波后的值，函数会更新该值
 * @return float 滤波后的值
 */
float filter(float p, float new_value, float* last_value) 
{ 
    // 使用指数平滑滤波公式计算新的滤波值
    *last_value = p * new_value + (1 - p) * (*last_value);
    
    // 返回新的滤波值
    return *last_value;
}

