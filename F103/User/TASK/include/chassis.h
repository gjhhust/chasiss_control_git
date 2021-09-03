#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "main.h"

struct chassis
{
	int leftSpeedNow;//左右电机当前速度
	int rightSpeedNow;
	int angle;//当前车体角度,100倍发送
	int controlFlag;//控制指令
	int speed_error;//失控累计
};
typedef struct chassis Chassis;
enum motorID{
right,left
};
void CHASSIC_task(void *pvParameters);

//根据上位机传来的速度指令进行运动
void Chassis_CurrentPid_Cal(void);

//pid初始化和动态调控
void PID_Param_Init(void);
//电机pid参数动态选择
void pid_motor_chose(Pid_Typedef *P, int speed);

//选择电机转向
int motor_direction(int ID,int Value);//返回占空比


//直线走1m标定
void goto_1m(void);

//控制操作
void control(void);
void SoftReset(void);
void SpeedReset(void);

#endif

