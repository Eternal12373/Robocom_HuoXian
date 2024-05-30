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

#include "MoveTask.h"


PidTypeDef ANGLE_Z_PID;

extern imu_t imu;

extern uint8_t MoveMode;
extern int SetCarSpeed,SetCarAngle;


void MoveTask_Function(void const * argument)
{
	float Move_A;
	int Move_S;
	float Yaw,YawError;
	float Error=0;

	static float ANGLE_speed_pid[3] = {ANGLE_SPEED_PID_KP, ANGLE_SPEED_PID_KI, ANGLE_SPEED_PID_KD};	
	portTickType tick = 0;
	PID_Init(&ANGLE_Z_PID, PID_POSITION, ANGLE_speed_pid, ANGLE_SPEED_PID_MAX_OUT, ANGLE_SPEED_PID_MAX_IOUT);  
	
	while(1)
	{
		tick = xTaskGetTickCount();
		Yaw = imu.yaw+tick/7000; //yaw after suppress zero drift
		Move_A = SetCarAngle;  //global angle
		Move_S = SetCarSpeed;  //run speed
		YawError=Yaw-Move_A;   //angle err
		if(YawError<-180)YawError+=360; //limit
		else if(YawError>180)YawError-=360; //limit
		Error = LineError();   //follow line
		//printf("Yaw=%f,YawError=%f\r\n",Yaw,YawError);
		//printf("(%d,%f)\r\n",SetCarAngle,imu.yaw);
		switch (MoveMode){
			case Stop:
				MotorRightSet(0);
				MotorLeftSet(0);
				PID_clear(&ANGLE_Z_PID);
				break;
			case OpenLoop:
				MotorRightSet(Move_S + Move_A);
				MotorLeftSet(Move_S - Move_A);
				break;
			case Drive:
				ANGLE_Z_PID.set=0;
				ANGLE_Z_PID.fdb = YawError;
				PID_Calc(&ANGLE_Z_PID);

				if(Move_S>0)
				{
					if(ANGLE_Z_PID.out>Move_S)ANGLE_Z_PID.out=Move_S;
					if(ANGLE_Z_PID.out<-Move_S)ANGLE_Z_PID.out=-Move_S;
				}
				else
				{
					if(ANGLE_Z_PID.out<Move_S)ANGLE_Z_PID.out=Move_S;
					if(ANGLE_Z_PID.out>-Move_S)ANGLE_Z_PID.out=-Move_S;
				}
				MotorRightSet(Move_S + ANGLE_Z_PID.out);
				MotorLeftSet(Move_S - ANGLE_Z_PID.out);
				break;
			case DetectLine:
				Error = -LineError();   //follow line
				if(Move_S>0)ANGLE_Z_PID.set =Error*3;  //need to run straight,more err correct
				else ANGLE_Z_PID.set = -Error*2;  //less err correct

				// ANGLE_Z_PID.fdb = YawError;  //imu err
				ANGLE_Z_PID.fdb=0;

				PID_Calc(&ANGLE_Z_PID);
				if(Move_S>0)
				{
					if(ANGLE_Z_PID.out>Move_S)ANGLE_Z_PID.out=Move_S;
					if(ANGLE_Z_PID.out<-Move_S)ANGLE_Z_PID.out=-Move_S;
				}
				else
				{
					if(ANGLE_Z_PID.out<Move_S)ANGLE_Z_PID.out=Move_S;
					if(ANGLE_Z_PID.out>-Move_S)ANGLE_Z_PID.out=-Move_S;
				}
				//printf("YawError=%f,Error=%f,z.out=%f,\r\n",Yaw,Error,ANGLE_Z_PID.out);
				MotorRightSet(Move_S + ANGLE_Z_PID.out);
				MotorLeftSet(Move_S - ANGLE_Z_PID.out);
				break;
			case TurnAngle:
				ANGLE_Z_PID.set=0;
				ANGLE_Z_PID.fdb = YawError;
				PID_Calc(&ANGLE_Z_PID);
				
				MotorRightSet(-ANGLE_Z_PID.out);
				MotorLeftSet(ANGLE_Z_PID.out);
				break;
		}
		//printf("ANGLE_Z_PID.out=%f\r\n",ANGLE_Z_PID.out);
		vTaskDelayUntil(&tick,10);
	}
}



