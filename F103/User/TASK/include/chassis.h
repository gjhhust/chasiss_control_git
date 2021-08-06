#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "main.h"


typedef struct
{
	int leftSpeedNow;//左右电机当前速度
	int rightSpeedNow;
	int angle;//当前车体角度
	int controlFlag;//控制指令
}Chassis;

void CHASSIC_task(void *pvParameters);

//根据上位机传来的速度指令进行运动
void Chassis_CurrentPid_Cal(void);

//pid初始化和动态调控
void PID_Param_Init(void);

void pid_motor_chose(Pid_Typedef *P, int speed);//电机pid参数动态选择



#endif

