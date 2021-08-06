#include "chassis.h"

Pid_Typedef Chassis_speed_L;
Pid_Typedef Chassis_speed_R;
Chassis F103RC_chassis={100,200,20,1};//底盘实时数据


extern Ctrl_information chassis_ctrl;//上位机控制指令
extern float Input[2];
extern float Output[2];

u16 led0pwmval=500;  
u16 led0pwmval2=500;  
int i=0;
int flag=1;


/**********************************************************************************************************
*函 数 名: Chassis_task
*功能说明: 底盘任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
uint32_t Chassis_high_water;
void CHASSIC_task(void *pvParameters){
	portTickType xLastWakeTime;
	const portTickType xFrequency = 1;	

	PID_Param_Init();


	
	while(1)
	{
 
		Chassis_CurrentPid_Cal();
		
		vTaskDelay(1); /* 延时 500 个 tick */
	
		xLastWakeTime = xTaskGetTickCount();
				
		vTaskDelayUntil(&xLastWakeTime,xFrequency); 
	
//	#if INCLUDE_uxTaskGetStackHighWaterMark
//					Chassis_high_water = uxTaskGetStackHighWaterMark(NULL);
//	#endif
	}
}


/**********************************************************************************************************
*函 数 名: Chassis_CurrentPid_Cal
*功能说明: 底盘操作
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Chassis_CurrentPid_Cal(void)
{
	//速度赋值 
	Chassis_speed_L.SetPoint = 100*chassis_ctrl.leftSpeedSet;
	Chassis_speed_R.SetPoint = 100*chassis_ctrl.rightSpeedSet;
	
	
	//选择pid
	pid_motor_chose(&Chassis_speed_L,Chassis_speed_L.SetPoint);
	pid_motor_chose(&Chassis_speed_R,Chassis_speed_R.SetPoint);

			
}
/**********************************************************************************************************
*函 数 名: PID_Param_Init
*功能说明: 电机PID初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void PID_Param_Init(void)
{
			//速度环左电机
		Chassis_speed_L.P = 25;
		Chassis_speed_L.I = 2;
		Chassis_speed_L.D = 0;
		Chassis_speed_L.ErrorMax = 1000.0f;
		Chassis_speed_L.IMax = 200;
		Chassis_speed_L.SetPoint = 0.0f;	
		Chassis_speed_L.OutMax = 750;	//最大占空 速度为370-590
	
		//速度环右电机
		Chassis_speed_R.P = 25;
		Chassis_speed_R.I = 2;
		Chassis_speed_R.D = 0;
		Chassis_speed_R.ErrorMax = 1000.0f;
		Chassis_speed_R.IMax = 200;
		Chassis_speed_R.SetPoint = 0.0f;	
		Chassis_speed_R.OutMax = 750;	//最大占空 速度为370-590
	
//		//位置环
//		Chassis_location.P = 5.0f;
//		Chassis_location.I = 0;
//		Chassis_location.D = 0;
//		Chassis_location.ErrorMax = 1500.0f;
//		Chassis_location.IMax = 4000.0f;
//		Chassis_location.SetPoint = 0.0f;	
//		Chassis_location.OutMax = 0.87f;//最大转速	
}

/**********************************************************************************************************
*函 数 名: pid_motor_chose
*功能说明: 右电机PID选择
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/

void pid_motor_chose(Pid_Typedef *P, int speed)
{

	//电机pid

	P->I = 0.0005* speed * speed - 0.0199*speed + 2.23;
		

}
