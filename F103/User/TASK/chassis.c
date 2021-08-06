#include "chassis.h"

Pid_Typedef Chassis_speed_L;
Pid_Typedef Chassis_speed_R;
Chassis F103RC_chassis={100,200,20,1};//����ʵʱ����


extern Ctrl_information chassis_ctrl;//��λ������ָ��
extern float Input[2];
extern float Output[2];

u16 led0pwmval=500;  
u16 led0pwmval2=500;  
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
	//�ٶȸ�ֵ 
	Chassis_speed_L.SetPoint = 100*chassis_ctrl.leftSpeedSet;
	Chassis_speed_R.SetPoint = 100*chassis_ctrl.rightSpeedSet;
	
	
	//ѡ��pid
	pid_motor_chose(&Chassis_speed_L,Chassis_speed_L.SetPoint);
	pid_motor_chose(&Chassis_speed_R,Chassis_speed_R.SetPoint);

			
}
/**********************************************************************************************************
*�� �� ��: PID_Param_Init
*����˵��: ���PID��ʼ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void PID_Param_Init(void)
{
			//�ٶȻ�����
		Chassis_speed_L.P = 25;
		Chassis_speed_L.I = 2;
		Chassis_speed_L.D = 0;
		Chassis_speed_L.ErrorMax = 1000.0f;
		Chassis_speed_L.IMax = 200;
		Chassis_speed_L.SetPoint = 0.0f;	
		Chassis_speed_L.OutMax = 750;	//���ռ�� �ٶ�Ϊ370-590
	
		//�ٶȻ��ҵ��
		Chassis_speed_R.P = 25;
		Chassis_speed_R.I = 2;
		Chassis_speed_R.D = 0;
		Chassis_speed_R.ErrorMax = 1000.0f;
		Chassis_speed_R.IMax = 200;
		Chassis_speed_R.SetPoint = 0.0f;	
		Chassis_speed_R.OutMax = 750;	//���ռ�� �ٶ�Ϊ370-590
	
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
*�� �� ��: pid_motor_chose
*����˵��: �ҵ��PIDѡ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/

void pid_motor_chose(Pid_Typedef *P, int speed)
{

	//���pid

	P->I = 0.0005* speed * speed - 0.0199*speed + 2.23;
		

}
