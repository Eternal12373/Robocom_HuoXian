#include "pid.h"

// ��������ֵ��ָ����Χ��
#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }

/**
 * @brief ��ʼ�� PID ������
 * 
 * @param pid PID �������ṹ��ָ��
 * @param mode ����ģʽ�������� PID_POSITION��λ���ͣ��� PID_DELTA�������ͣ�
 * @param PID PID �������飬��������ϵ�� Kp������ϵ�� Ki ��΢��ϵ�� Kd
 * @param max_out ����޷�ֵ
 * @param max_iout ��������޷�ֵ
 */
void PID_Init(PidTypeDef *pid, uint8_t mode, const float PID[3], int max_out, int max_iout)
{
    pid->mode = mode;
    pid->Kp = PID[0];
    pid->Ki = PID[1];
    pid->Kd = PID[2];
    pid->max_out = max_out;
    pid->max_iout = max_iout;
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    pid->error[0] = pid->error[1] = pid->error[2] = pid->Pout = pid->Iout = pid->Dout = pid->out = 0.0f;
}

/**
 * @brief ���� PID ���������
 * 
 * @param pid PID �������ṹ��ָ��
 * @return float PID ���������ֵ
 */
float PID_Calc(PidTypeDef *pid)
{
    // �������ֵ
    pid->error[2] = pid->error[1];
    pid->error[1] = pid->error[0];
    pid->error[0] = pid->set - pid->fdb;

    if (pid->mode == PID_POSITION)    // λ���� PID ����
    {
        // ���������������΢����
        pid->Pout = pid->Kp * pid->error[0];
        pid->Iout += pid->Ki * pid->error[0];
        pid->Dbuf[2] = pid->Dbuf[1];
        pid->Dbuf[1] = pid->Dbuf[0];
        pid->Dbuf[0] = (pid->error[0] - pid->error[1]);
        pid->Dout = pid->Kd * pid->Dbuf[0];

        // ���ƻ�����������ֵ
        LimitMax(pid->Iout, pid->max_iout);
        pid->out = pid->Pout + pid->Iout + pid->Dout;
        LimitMax(pid->out, pid->max_out);
    }
    else if (pid->mode == PID_DELTA)    // ������ PID ����
    {
        // ���������������΢����
        pid->Pout = pid->Kp * (pid->error[0] - pid->error[1]);
        pid->Iout = pid->Ki * pid->error[0];
        pid->Dbuf[2] = pid->Dbuf[1];
        pid->Dbuf[1] = pid->Dbuf[0];
        pid->Dbuf[0] = (pid->error[0] - 2.0f * pid->error[1] + pid->error[2]);
        pid->Dout = pid->Kd * pid->Dbuf[0];

        // �������ֵ
        pid->out += pid->Pout + pid->Iout + pid->Dout;
        LimitMax(pid->out, pid->max_out);
    }
    return pid->out;
}

/**
 * @brief ��� PID ������״̬
 * 
 * @param pid PID �������ṹ��ָ��
 */
void PID_clear(PidTypeDef *pid)
{
    // ����΢�ֻ�������ֵ����
    pid->error[0] = pid->error[1] = pid->error[2] = 0.0f;
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    pid->out = pid->Pout = pid->Iout = pid->Dout = 0.0f;
    pid->fdb = pid->set = 0.0f;
}
