#include "chassis.h"

Pid_Typedef Chassis_speed;
Pid_Typedef Chassis_location;

int leftSpeedNow=0;//���ҵ����ǰ�ٶ�
int rightSpeedNow=0;
int angle=0;//��ǰ����Ƕ�

//extern unsigned char controlFlag;//�����Ҫ���������������޸ĸ�ֵ


u16 led0pwmval=300;  
u16 led0pwmval2=0;  
int i=0;
int flag=1;


/**********************************************************************************************************
*�� �� ��: Chassis_task
*����˵��: ��������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
uint32_t Chassis_high_water;
void CHASSIC_task(void *pvParameters){
	portTickType xLastWakeTime;
	const portTickType xFrequency = 1;	

	PID_Param_Init();


	
	while(1)
	{
 
		Chassis_CurrentPid_Cal();
		
		vTaskDelay(1); /* ��ʱ 500 �� tick */
	
		xLastWakeTime = xTaskGetTickCount();
				
		vTaskDelayUntil(&xLastWakeTime,xFrequency); 
	
//	#if INCLUDE_uxTaskGetStackHighWaterMark
//					Chassis_high_water = uxTaskGetStackHighWaterMark(NULL);
//	#endif
	}
}


/**********************************************************************************************************
*�� �� ��: Chassis_CurrentPid_Cal
*����˵��: ���̲���
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Chassis_CurrentPid_Cal(void)
{
	//��ȡ��ǰ�ٶȲ����͸���λ��
	Get_Motor_Speed(&leftSpeedNow,&rightSpeedNow);
	
	//����Ҫ���͵�ROS�����ݣ��Ӹú���������ǰ�������ݷ�Χ��-32768 - +32767�������ĸ����ݵķ�Χ(0 - 255)
	//usartSendData(leftSpeedNow,rightSpeedNow,angle,controlFlag);
	//elay_ms(13);//��Ҫ��13ms��ʱ
	
	
	pid_speed_chose();
	
	TIM_SetCompare1(TIM1, PID_Calc(&Chassis_speed, rightSpeedNow));	
	
}

/**********************************************************************************************************
*�� �� ��: PID_Param_Init
*����˵��: ���PID��ʼ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void PID_Param_Init(void)
{
			//�ٶȻ����ڻ���
		Chassis_speed.P = 0;
		Chassis_speed.I = 0;
		Chassis_speed.D = 0;
		Chassis_speed.ErrorMax = 1000.0f;
		Chassis_speed.IMax = 200;
		Chassis_speed.SetPoint = 0.0f;	
		Chassis_speed.OutMax = 890;	//���ռ�ձ�
	
	
//		//λ�û�
//		Chassis_location.P = 5.0f;
//		Chassis_location.I = 0;
//		Chassis_location.D = 0;
//		Chassis_location.ErrorMax = 1500.0f;
//		Chassis_location.IMax = 4000.0f;
//		Chassis_location.SetPoint = 0.0f;	
//		Chassis_location.OutMax = 0.87f;//���ת��	
}

/**********************************************************************************************************
*�� �� ��: pid_speed_chose
*����˵��: ���PIDѡ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/

void pid_speed_chose(void)
{
//	Chassis_speed.P = 0.00003*rightSpeedNow*rightSpeedNow - 0.0122*rightSpeedNow + 10.012;
//	Chassis_speed.I = 0.00005*rightSpeedNow*rightSpeedNow - 0.0197*rightSpeedNow + 2.2863;
	
	if(rightSpeedNow<250)
	{
		Chassis_speed.P = 9;
		Chassis_speed.I = 0.5;
	}else if(rightSpeedNow<300)
	{
		Chassis_speed.P = 9;
		Chassis_speed.I = 1;
	}else if(rightSpeedNow<350)
	{
		Chassis_speed.P = 9;
		Chassis_speed.I = 1.3;
	}else if(rightSpeedNow<380)
	{
		Chassis_speed.P = 10;
		Chassis_speed.I = 1.45;
	}else 
	{
		Chassis_speed.P = 11;
		Chassis_speed.I = 0.025*rightSpeedNow - 8.0862;	
	}

}


