#include "chassis.h"

Pid_Typedef Chassis_speed;
Pid_Typedef Chassis_location;

Chassis F103RC_chassis={100,200,20,1};//底盘实时数据
extern Ctrl_information chassis_ctrl;//上位机控制指令


u16 led0pwmval=300;  
u16 led0pwmval2=0;  
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
	Chassis_speed.SetPoint = 100*chassis_ctrl.leftSpeedSet;
	
	
	//获取当前速度并发送给上位机
	Get_Motor_Speed(&F103RC_chassis.leftSpeedNow,&F103RC_chassis.rightSpeedNow);
	
	pid_motor_chose();
	
	TIM_SetCompare1(TIM1, PID_Calc(&Chassis_speed, F103RC_chassis.rightSpeedNow));	
	TIM_SetCompare2(TIM1, PID_Calc(&Chassis_speed, F103RC_chassis.leftSpeedNow));	
	
}

/**********************************************************************************************************
*函 数 名: PID_Param_Init
*功能说明: 电机PID初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void PID_Param_Init(void)
{
			//速度环（内环）
		Chassis_speed.P = 0;
		Chassis_speed.I = 0;
		Chassis_speed.D = 0;
		Chassis_speed.ErrorMax = 1000.0f;
		Chassis_speed.IMax = 200;
		Chassis_speed.SetPoint = 0.0f;	
		Chassis_speed.OutMax = 890;	//最大占空比
	
	
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

void pid_motor_chose(void)
{
//	Chassis_speed.P = 0.00003*rightSpeedNow*rightSpeedNow - 0.0122*rightSpeedNow + 10.012;
//	Chassis_speed.I = 0.00005*rightSpeedNow*rightSpeedNow - 0.0197*rightSpeedNow + 2.2863;
	
	//右电机pid
	if(F103RC_chassis.rightSpeedNow<250)
	{
		Chassis_speed.P = 9;
		Chassis_speed.I = 0.5;
	}else if(F103RC_chassis.rightSpeedNow<300)
	{
		Chassis_speed.P = 9;
		Chassis_speed.I = 1;
	}else if(F103RC_chassis.rightSpeedNow<350)
	{
		Chassis_speed.P = 9;
		Chassis_speed.I = 1.3;
	}else if(F103RC_chassis.rightSpeedNow<380)
	{
		Chassis_speed.P = 10;
		Chassis_speed.I = 1.45;
	}else 
	{
		Chassis_speed.P = 11;
		Chassis_speed.I = 0.025*F103RC_chassis.rightSpeedNow - 8.0862;	
	}
	
	//左电机pid
	if(F103RC_chassis.leftSpeedNow<250)
	{
		Chassis_speed.P = 9;
		Chassis_speed.I = 0.5;
	}else if(F103RC_chassis.leftSpeedNow<300)
	{
		Chassis_speed.P = 9;
		Chassis_speed.I = 1;
	}else if(F103RC_chassis.leftSpeedNow<350)
	{
		Chassis_speed.P = 9;
		Chassis_speed.I = 1.3;
	}else if(F103RC_chassis.leftSpeedNow<380)
	{
		Chassis_speed.P = 10;
		Chassis_speed.I = 1.45;
	}else 
	{
		Chassis_speed.P = 11;
		Chassis_speed.I = 0.025*F103RC_chassis.leftSpeedNow - 8.0862;	
	}

	

}
