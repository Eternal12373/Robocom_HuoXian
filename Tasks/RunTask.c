/*
 *  Nanjing JIYU Technology Co.,Ltd All rights reserved.
 *  Copyright (C) 2022
 *  ==============================================================
 *
 *  RunTask.h
 *    整体运行逻辑
 *
 *  --------------------------------------------------------------
 *
 */

#include "MoveTask.h"
#include "RunTask.h"

uint8_t SingleMode = 0;               // 单圈模式标志
uint8_t MoveMode = Stop;              // 移动模式
int SetCarSpeed = 0, SetCarAngle = 0; // 设定小车速度和角度
float CarAngle = 0, LastCarAngle = 0; // 小车当前角度和上一个角度
extern OpenmvTypeDef Openmv;          // OpenMV数据
uint8_t BallColor = 0;                // 球的颜色
unsigned int SingleTim = 0;           // 单圈计时器
uint8_t TurnStraightSign = 0;         // 转直行标志

static void TurnStraightUntil(int16_t SetStraightSpeed, int16_t SetTurnAngle, uint8_t Cross);
static void StraightUntil(int16_t SetStraightSpeed, int16_t SetAngle, uint8_t Cross);
static uint8_t SingleCircle(void);

int test = 0;
int turn_flag = 0;
uint8_t RunMode = 0;

unsigned char test_gray[12];
int i;
int j;
/**
 * @brief RunTask任务函数
 *
 * @param argument 任务参数
 */
void RunTask_Function(void const *argument)
{

    unsigned int tim = 0;
    portTickType tick = 0;
    while (1)
    {
        tick = xTaskGetTickCount();
        // SingleCircle();
        // TurnStraightUntil(LowSpeed,90,Crossing);
        // 		  MoveMode=DetectLine;        // 进入巡线模式（低速，车方向角度0），检测到右侧交叉路口停车
        //    SetCarSpeed=LowSpeed;
//				get_gray(test_gray);
//			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, i);
//			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, j);
// printf("%d, %d, %d, %d\r\n", Openmv.x, Openmv.y, Openmv.h, Openmv.m);
#if 0
        switch (RunMode)
        {
        case 0:
            if (ServoUp()) // 夹爪抬升
            {
                tim = 0;
                RunMode++;
            }
            break;
        case 1:
            if (tim < 900) // 夹爪开合
            {
                __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1700); // 1600 loose
            }
            else
            {
                __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 2500); // 2500 catch
                tim = 0;
                RunMode++;
            }
            break;
        case 2:
            MoveMode = DetectLine; // 进入巡线模式（低速，车方向角度0），检测到右侧交叉路口停车
            SetCarSpeed = LowSpeed;
            CarAngle = 0; // tim>200
            if (CrossDetect(CrossRight) && tim > 200)
            {
                tim = 0;
                MoveMode = Stop;
                RunMode++;
            }
            break;
        case 3: // 进入singlecircle函数，跑单圈
            if (SingleCircle())
            {
                tim = 0;
                MoveMode = Stop;
                RunMode++;
            }
            break;
        case 4: // 舵机抬升
            if (ServoUp())
            {
                tim = 0;
                RunMode++;
            }
            break;
        case 5:
            if (tim < 900)
            {
                __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1200);
            }
            else
            {
                __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1500);
                tim = 0;
                RunMode++;
            }
            break;
        case 6: // 巡线（高速，航向角180，检测到左侧路口停车）
            MoveMode = DetectLine;
            SetCarSpeed = HighSpeed;
            CarAngle = 180;
            if (CrossDetect(CrossLeft) && tim > 200)
            {
                tim = 0;
                MoveMode = Stop;
                RunMode++;
                SingleTim = 0;
            }
            break;
        case 7: // 进入singlecircle函数，跑单圈
            if (SingleCircle())
            {
                tim = 0;
                RunMode++;
            }
            break;
        case 8: // 舵机抬升
            if (ServoUp())
            {
                tim = 0;
                RunMode++;
            }
            break;
        case 9:
            if (tim < 900)
            {
                __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1200);
            }
            else
            {
                __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1500);
                tim = 0;
                RunMode++;
                SingleTim = 0;
            }
            break;
        case 10: // 进入singlecircle函数，跑单圈
            if (SingleCircle())
            {
                tim = 0;
                RunMode++;
            }
            break;
        case 11:
            MoveMode = DetectLine;
            SetCarSpeed = HighSpeed;
            CarAngle = 180;
            if (CrossDetect(Crossing) && tim > 200)
            {
                tim = 0;
                MoveMode = Stop;
                RunMode++;
            }
            break;
        case 12:
            MoveMode = DetectLine;
            SetCarSpeed = HighSpeed;
            CarAngle = 180;
            if (CrossDetect(Crossing) && tim > 200)
            {
                tim = 0;
                MoveMode = Stop;
                RunMode++;
            }
            break;
        default:
            MoveMode = Stop;
        }
        tim++;

#endif

        vTaskDelayUntil(&tick, 1);
    }
}

/**
 * @brief 单圈运行函数
 *
 * @return uint8_t 返回1表示单圈结束，返回0表示继续
 */
static uint8_t SingleCircle(void)
{
    int NullTime = 0;
    switch (SingleMode)
    {
    case 0:
        TurnStraightUntil(LowSpeed, 90, Crossing); // 转弯直行，低速，航向角90度
        break;
    case 1:

        if (SingleTim < 500)
            ServoGrab();          // 抓取
        else if (SingleTim < 700) // 中速后退，航向角 -90度
        {
            MoveMode = Drive;
            SetCarSpeed = -MediumSpeed;
            CarAngle = -90;
        }
        else
        {
            SingleTim = 0;
            MoveMode = Stop;
            SingleMode++;
        }
        break;
    case 2:
        if (ServoUp()) // 舵机抬升
        {
            SingleTim = 0;
            SingleMode++;
        }
        break;
    case 3:
        TurnStraightUntil(-LowSpeed, 90, Crossing); // 转弯直行，低速倒车，航向角90度，十字路口停车
        break;
    case 4:
        TurnStraightUntil(HighSpeed, 0, CrossLeft); // 转弯直行，高速 ，航向角0度，检测到左侧路口停车
        break;
    case 5:
        TurnStraightUntil(MediumSpeed, -90, Crossing); // 转弯直行，中速 ，航向角-90度，检测到十字路口停车
        break;
    case 6:
        TurnStraightUntil(HighSpeed, -180, CrossRight); // 转弯直行，高速， 航向角-180度， 检测到右侧路口停车
        break;
    case 7:
        TurnStraightUntil(HighSpeed, -90, CrossRight); // 转弯直行，高速， 航向角-90度， 检测到右侧路口停车
        break;
    case 8:
        TurnStraightUntil(HighSpeed, 0, CrossLeft); // 转弯直行，高速， 航向角0度， 检测到左侧路口停车
        break;
    case 9:
        TurnStraightUntil(HighSpeed, -90, CrossRight); // 转弯直行，高速， 航向角-90度， 检测到右侧路口停车
        break;
    case 10:
        TurnStraightUntil(HighSpeed, 0, CrossLeft); // 转弯直行，高速， 航向角0度， 检测到左侧路口停车
        break;
    case 11:
        TurnStraightUntil(200, -90, Crossing); // 转弯直行，速度200， 航向角-90度， 检测到十字路口停车
        break;
    case 12: // 将沙包丢到传送带上
        if (SingleTim < 500)
        {
            MoveMode = Stop;
        }
        else if (SingleTim < 800)
        {
            __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1200);
        }
        else if (SingleTim < 1100)
        {
            __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1500);
            ServoRelease();
        }
        else if (SingleTim < 1500)
        {
            MoveMode = Drive;
            SetCarSpeed = -MediumSpeed;
            CarAngle = -90;
        }
        else
        {
            SingleTim = 0;
            MoveMode = Stop;
            SingleMode++;
        }
        break;
    case 13:
        StraightUntil(-MediumSpeed, -90, CrossLeft); // 后退 （中速， 航向角-90度，检测到左侧路口停车）
        break;
    case 14:
        TurnStraightUntil(HighSpeed, -180, CrossLeft); // 转弯直行 （高速， 航向角-180度，检测到左侧路口停车）
        break;
    case 15:
        TurnStraightUntil(MediumSpeed, 90, Crossing); // 转弯直行 （中速， 航向角90度，检测到十字路口停车）
        break;
    case 16:
        if (ServoUp()) // 夹爪抬升，释放
        {
            ServoRelease();
            SingleTim = 0;
            SingleMode++;
            LastCarAngle = 0;
            CarAngle = 0;
        }
        break;
    case 17: // 根据openmv提供的位置信息，去夹取，刚刚被抛投出去的沙包
        MoveMode = Drive;
        SetCarSpeed = 150;
        if (SingleTim > 3000)
            CarAngle = LastCarAngle + Openmv.x / 400.0;
        if (CarAngle < -45)
            CarAngle = -45;
        if (CarAngle > 45)
            CarAngle = 45;
        SetCarAngle = 90 + CarAngle;
        LastCarAngle = CarAngle;
        if (Openmv.h > 950 && Openmv.y < -45 && abs(Openmv.x) < 4)
        {
            BallColor = Openmv.m;
            SingleTim = 0;
            MoveMode = Stop;
            SingleMode++;
        }
        break;
    case 18:
        if (SingleTim < 1600) // 夹取
        {
            __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 850);
        }
        else
        {
            __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1500);
            SingleTim = 0;
            SingleMode++;
        }
        break;
    case 19: // 航向角0 速度600
        MoveMode = OpenLoop;
        SetCarSpeed = 600;
        SetCarAngle = 0;
        if (SingleTim > 600)
        {
            ServoGrab();
            SingleTim = 0;
            MoveMode = Stop;
            SingleMode++;
        }
        break;
    case 20:
        if (ServoUp())
        {
            SingleTim = 0;
            SingleMode++;
        }
        break;
    case 21:
        MoveMode = TurnAngle;
        SetCarAngle = 0;
        if (SingleTim > 300)
        {
            SingleTim = 0;
            MoveMode = Stop;
            SingleMode++;
        }
        break;
    case 22:
        MoveMode = Drive;
        SetCarSpeed = MediumSpeed;
        CarAngle = 0;
        if (CrossDetect(Crossing) && SingleTim > 800)
        {
            SingleTim = 0;
            MoveMode = Stop;
            SingleMode++;
        }
        else if (CrossDetect(CrossNum) > 1 && SingleTim < 1200)
        {
            if (SingleTim > 600)
                SingleTim = 600;
        }
        break;
    case 23:
        MoveMode = Drive;
        SetCarSpeed = -MediumSpeed;
        CarAngle = 0;
        if (SingleTim > 200)
        {
            SingleTim = 0;
            MoveMode = Stop;
            SingleMode++;
        }
        break;
    case 24:
        TurnStraightUntil(HighSpeed, 90, Crossing);
        break;
    case 25:
        TurnStraightUntil(HighSpeed, 180, CrossLeft);
        break;
    case 26:
        TurnStraightUntil(MediumSpeed, 90, Crossing);
        break;
    case 27:
        TurnStraightUntil(MediumSpeed, 0, CrossRight);
        break;
    case 28: // 根据目标的颜色，来决定跳转到哪个case分支
        if (BallColor == 4)
            SingleMode = SingleMode + 3;
        else if (BallColor == 2)
            SingleMode = SingleMode + 2;
        else if (BallColor == 1)
            SingleMode = SingleMode + 1;
        break;
    case 29:
        StraightUntil(MediumSpeed, 0, CrossRight);
        break;
    case 30:
        StraightUntil(MediumSpeed, 0, CrossRight);
        break;
    case 31:
        TurnStraightUntil(LowSpeed, 90, Crossing);
        break;
    case 32:
        if (SingleTim < 1350)
        {
            __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1200);
        }
        else if (SingleTim < 1500)
        {
            __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1500);
            ServoRelease();
        }
        else if (SingleTim < 1700)
        {
            MoveMode = Drive;
            SetCarSpeed = -MediumSpeed;
            CarAngle = 90;
        }
        break;
    case 33:
        StraightUntil(-LowSpeed, 90, Crossing);
        break;
    case 34:
        TurnStraightUntil(HighSpeed, 180, CrossRight);
        break;
    case 35:
        StraightUntil(HighSpeed, 180, CrossLeft);
        break;
    case 36:
        SingleMode = 0;
        return 1;
    }
    SingleTim++;
    return 0;
}

/**
 * @brief 转直行直至条件满足函数
 *
 * @param SetStraightSpeed 设定直行速度
 * @param SetTurnAngle 设定转弯角度
 * @param Cross 交叉口类型
 */
static void TurnStraightUntil(int16_t SetStraightSpeed, int16_t SetTurnAngle, uint8_t Cross)
{
    // if (SingleTim < 200 && turn_flag == 0)
    // {
    //     MoveMode = OpenLoop;
    //     SetCarSpeed = LowSpeed;
    //     SetCarAngle = 0;
    // }
    // else
    // {
    //     SingleTim = 0;
    //     turn_flag = 1;
    // }
    // if (turn_flag)
    // {
    if (TurnStraightSign == 0) // 车航向角设定  （转向模式）
    {
        if (SingleTim < 200)
        {
            MoveMode = OpenLoop;
            SetCarSpeed = LowSpeed;
            SetCarAngle = 0;
        }
        else
        {
            MoveMode = TurnAngle;
            SetCarAngle = SetTurnAngle;
            if (SingleTim > 620)
            {
                SingleTim = 0;
                MoveMode = Stop;
                TurnStraightSign = 1;
            }
        }
    }
    else if (TurnStraightSign == 1)
    {
        MoveMode = DetectLine;
        SetCarSpeed = SetStraightSpeed; // 设定速度
        SetCarAngle = SetTurnAngle;     // 设定转向角度
        if (CrossDetect(Cross) && SingleTim > 200)
        { // 检测到十字路口停车
            SingleTim = 0;
            MoveMode = Stop;
            if (SetStraightSpeed < 0)
                TurnStraightSign = 2; // 设定速度小于0， 进入TurnStraightSign==2分支
            else if (SetStraightSpeed > 400)
                TurnStraightSign = 3; // 设定速度大于0， 进入TurnStraightSign==3分支
            else
            {
                TurnStraightSign = 0;
                SingleMode++;
                turn_flag = 0;
            }
        }
    }
    else if (TurnStraightSign == 2)
    {
        MoveMode = Drive;
        SetCarSpeed = 400;
        SetCarAngle = SetTurnAngle;
        if (SingleTim > 150)
        {
            SingleTim = 0;
            MoveMode = Stop;
            TurnStraightSign = 0;
            SingleMode++;
            turn_flag = 0;
        }
    }
    else if (TurnStraightSign == 3)
    {
        MoveMode = Drive;
        SetCarSpeed = -400;
        SetCarAngle = SetTurnAngle;
        if (SingleTim > 40)
        {
            SingleTim = 0;
            MoveMode = Stop;
            TurnStraightSign = 0;
            SingleMode++;
            turn_flag = 0;
        }
    }
    // }
}

/**
 * @brief 直行直至条件满足函数
 *
 * @param SetStraightSpeed 设定直行速度
 * @param SetAngle 设定角度
 * @param Cross 交叉口类型
 */
static void StraightUntil(int16_t SetStraightSpeed, int16_t SetAngle, uint8_t Cross)
{

    if (TurnStraightSign == 0)
    {
        MoveMode = DetectLine;
        SetCarSpeed = SetStraightSpeed;
        SetCarAngle = SetAngle;
        if (CrossDetect(Cross) && SingleTim > 150)
        {
            SingleTim = 0;
            MoveMode = Stop;
            if (SetStraightSpeed < 0)
                TurnStraightSign = 1;
            else if (SetStraightSpeed > 0)
                TurnStraightSign = 2;
            else
            {
                TurnStraightSign = 0;
                SingleMode++;
            }
        }
    }
    else if (TurnStraightSign == 1)
    {
        MoveMode = Drive;
        SetCarSpeed = 400;
        SetCarAngle = SetAngle;
        if (SingleTim > 150)
        {
            SingleTim = 0;
            MoveMode = Stop;
            TurnStraightSign = 0;
            SingleMode++;
        }
    }
    else if (TurnStraightSign == 2)
    {
        MoveMode = Drive;
        SetCarSpeed = -SetStraightSpeed * 0.6;
        SetCarAngle = SetAngle;
        if (SingleTim > 10)
        {
            SingleTim = 0;
            MoveMode = Stop;
            TurnStraightSign = 0;
            SingleMode++;
        }
    }
}
