#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "main.h"


typedef struct
{
	int leftSpeedNow;//���ҵ����ǰ�ٶ�
	int rightSpeedNow;
	int angle;//��ǰ����Ƕ�
	int controlFlag;//����ָ��
}Chassis;

void CHASSIC_task(void *pvParameters);

//������λ���������ٶ�ָ������˶�
void Chassis_CurrentPid_Cal(void);

//pid��ʼ���Ͷ�̬����
void PID_Param_Init(void);

void pid_motor_chose(Pid_Typedef *P, int speed);//���pid������̬ѡ��



#endif

