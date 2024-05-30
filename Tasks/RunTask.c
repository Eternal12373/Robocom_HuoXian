/*
 *  Nanjing JIYU Technology Co.,Ltd All rights reserved.
 *  Copyright (C) 2022
 *  ==============================================================
 *
 *  RunTask.h
 *    ���������߼�
 *
 *  --------------------------------------------------------------
 *
 */

#include "MoveTask.h"
#include "RunTask.h"

uint8_t SingleMode = 0;               // ��Ȧģʽ��־
uint8_t MoveMode = Stop;              // �ƶ�ģʽ
int SetCarSpeed = 0, SetCarAngle = 0; // �趨С���ٶȺͽǶ�
float CarAngle = 0, LastCarAngle = 0; // С����ǰ�ǶȺ���һ���Ƕ�
extern OpenmvTypeDef Openmv;          // OpenMV����
uint8_t BallColor = 0;                // �����ɫ
unsigned int SingleTim = 0;           // ��Ȧ��ʱ��
uint8_t TurnStraightSign = 0;         // תֱ�б�־

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
 * @brief RunTask������
 *
 * @param argument �������
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
        // 		  MoveMode=DetectLine;        // ����Ѳ��ģʽ�����٣�������Ƕ�0������⵽�Ҳཻ��·��ͣ��
        //    SetCarSpeed=LowSpeed;
//				get_gray(test_gray);
//			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, i);
//			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, j);
// printf("%d, %d, %d, %d\r\n", Openmv.x, Openmv.y, Openmv.h, Openmv.m);
#if 0
        switch (RunMode)
        {
        case 0:
            if (ServoUp()) // ��צ̧��
            {
                tim = 0;
                RunMode++;
            }
            break;
        case 1:
            if (tim < 900) // ��צ����
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
            MoveMode = DetectLine; // ����Ѳ��ģʽ�����٣�������Ƕ�0������⵽�Ҳཻ��·��ͣ��
            SetCarSpeed = LowSpeed;
            CarAngle = 0; // tim>200
            if (CrossDetect(CrossRight) && tim > 200)
            {
                tim = 0;
                MoveMode = Stop;
                RunMode++;
            }
            break;
        case 3: // ����singlecircle�������ܵ�Ȧ
            if (SingleCircle())
            {
                tim = 0;
                MoveMode = Stop;
                RunMode++;
            }
            break;
        case 4: // ���̧��
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
        case 6: // Ѳ�ߣ����٣������180����⵽���·��ͣ����
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
        case 7: // ����singlecircle�������ܵ�Ȧ
            if (SingleCircle())
            {
                tim = 0;
                RunMode++;
            }
            break;
        case 8: // ���̧��
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
        case 10: // ����singlecircle�������ܵ�Ȧ
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
 * @brief ��Ȧ���к���
 *
 * @return uint8_t ����1��ʾ��Ȧ����������0��ʾ����
 */
static uint8_t SingleCircle(void)
{
    int NullTime = 0;
    switch (SingleMode)
    {
    case 0:
        TurnStraightUntil(LowSpeed, 90, Crossing); // ת��ֱ�У����٣������90��
        break;
    case 1:

        if (SingleTim < 500)
            ServoGrab();          // ץȡ
        else if (SingleTim < 700) // ���ٺ��ˣ������ -90��
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
        if (ServoUp()) // ���̧��
        {
            SingleTim = 0;
            SingleMode++;
        }
        break;
    case 3:
        TurnStraightUntil(-LowSpeed, 90, Crossing); // ת��ֱ�У����ٵ����������90�ȣ�ʮ��·��ͣ��
        break;
    case 4:
        TurnStraightUntil(HighSpeed, 0, CrossLeft); // ת��ֱ�У����� �������0�ȣ���⵽���·��ͣ��
        break;
    case 5:
        TurnStraightUntil(MediumSpeed, -90, Crossing); // ת��ֱ�У����� �������-90�ȣ���⵽ʮ��·��ͣ��
        break;
    case 6:
        TurnStraightUntil(HighSpeed, -180, CrossRight); // ת��ֱ�У����٣� �����-180�ȣ� ��⵽�Ҳ�·��ͣ��
        break;
    case 7:
        TurnStraightUntil(HighSpeed, -90, CrossRight); // ת��ֱ�У����٣� �����-90�ȣ� ��⵽�Ҳ�·��ͣ��
        break;
    case 8:
        TurnStraightUntil(HighSpeed, 0, CrossLeft); // ת��ֱ�У����٣� �����0�ȣ� ��⵽���·��ͣ��
        break;
    case 9:
        TurnStraightUntil(HighSpeed, -90, CrossRight); // ת��ֱ�У����٣� �����-90�ȣ� ��⵽�Ҳ�·��ͣ��
        break;
    case 10:
        TurnStraightUntil(HighSpeed, 0, CrossLeft); // ת��ֱ�У����٣� �����0�ȣ� ��⵽���·��ͣ��
        break;
    case 11:
        TurnStraightUntil(200, -90, Crossing); // ת��ֱ�У��ٶ�200�� �����-90�ȣ� ��⵽ʮ��·��ͣ��
        break;
    case 12: // ��ɳ���������ʹ���
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
        StraightUntil(-MediumSpeed, -90, CrossLeft); // ���� �����٣� �����-90�ȣ���⵽���·��ͣ����
        break;
    case 14:
        TurnStraightUntil(HighSpeed, -180, CrossLeft); // ת��ֱ�� �����٣� �����-180�ȣ���⵽���·��ͣ����
        break;
    case 15:
        TurnStraightUntil(MediumSpeed, 90, Crossing); // ת��ֱ�� �����٣� �����90�ȣ���⵽ʮ��·��ͣ����
        break;
    case 16:
        if (ServoUp()) // ��צ̧�����ͷ�
        {
            ServoRelease();
            SingleTim = 0;
            SingleMode++;
            LastCarAngle = 0;
            CarAngle = 0;
        }
        break;
    case 17: // ����openmv�ṩ��λ����Ϣ��ȥ��ȡ���ոձ���Ͷ��ȥ��ɳ��
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
        if (SingleTim < 1600) // ��ȡ
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
    case 19: // �����0 �ٶ�600
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
    case 28: // ����Ŀ�����ɫ����������ת���ĸ�case��֧
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
 * @brief תֱ��ֱ���������㺯��
 *
 * @param SetStraightSpeed �趨ֱ���ٶ�
 * @param SetTurnAngle �趨ת��Ƕ�
 * @param Cross ���������
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
    if (TurnStraightSign == 0) // ��������趨  ��ת��ģʽ��
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
        SetCarSpeed = SetStraightSpeed; // �趨�ٶ�
        SetCarAngle = SetTurnAngle;     // �趨ת��Ƕ�
        if (CrossDetect(Cross) && SingleTim > 200)
        { // ��⵽ʮ��·��ͣ��
            SingleTim = 0;
            MoveMode = Stop;
            if (SetStraightSpeed < 0)
                TurnStraightSign = 2; // �趨�ٶ�С��0�� ����TurnStraightSign==2��֧
            else if (SetStraightSpeed > 400)
                TurnStraightSign = 3; // �趨�ٶȴ���0�� ����TurnStraightSign==3��֧
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
 * @brief ֱ��ֱ���������㺯��
 *
 * @param SetStraightSpeed �趨ֱ���ٶ�
 * @param SetAngle �趨�Ƕ�
 * @param Cross ���������
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
