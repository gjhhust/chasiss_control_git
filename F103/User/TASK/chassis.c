#include "chassis.h"

Pid_Typedef Chassis_speed;
Pid_Typedef Chassis_location;

int leftSpeedNow=0;//左右电机当前速度
int rightSpeedNow=0;
int angle=0;//当前车体角度

//extern unsigned char controlFlag;//如果需要控制命令符则可以修改该值


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
	//获取当前速度并发送给上位机
	Get_Motor_Speed(&leftSpeedNow,&rightSpeedNow);
	
	//将需要发送到ROS的数据，从该函数发出，前三个数据范围（-32768 - +32767），第四个数据的范围(0 - 255)
	//usartSendData(leftSpeedNow,rightSpeedNow,angle,controlFlag);
	//elay_ms(13);//必要的13ms延时
	
	
	pid_speed_chose();
	
	TIM_SetCompare1(TIM1, PID_Calc(&Chassis_speed, rightSpeedNow));	
	
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
*函 数 名: pid_speed_chose
*功能说明: 电机PID选择
*形    参: 无
*返 回 值: 无
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


