#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "main.h"




void CHASSIC_task(void *pvParameters);

//������λ���������ٶ�ָ������˶�
void Chassis_CurrentPid_Cal(void);

//pid��ʼ���Ͷ�̬����
void PID_Param_Init(void);
void pid_speed_chose(void);


#endif

