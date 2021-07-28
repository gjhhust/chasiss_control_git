#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "main.h"




void CHASSIC_task(void *pvParameters);

//根据上位机传来的速度指令进行运动
void Chassis_CurrentPid_Cal(void);

//pid初始化和动态调控
void PID_Param_Init(void);
void pid_speed_chose(void);


#endif

