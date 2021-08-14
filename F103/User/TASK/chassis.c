#include "chassis.h"

Pid_Typedef Chassis_speed_L;
Pid_Typedef Chassis_speed_R;
Pid_Typedef Chassis_position;
Chassis F103RC_chassis={100,200,20,1,0};//底盘实时数据


extern Ctrl_information chassis_ctrl;//上位机控制指令
extern float Input[2];
extern float Output[2];

int led0pwmval=20;  
int led0pwmval2=450;  
int i=0;
int flag=1;
double positionNow=0;

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
	Chassis_speed_L.SetPoint = LIMIT_MAX_MIN(chassis_ctrl.leftSpeedSet,70,-70);
	Chassis_speed_R.SetPoint = LIMIT_MAX_MIN(chassis_ctrl.rightSpeedSet,70,-70);
	//Chassis_speed_L.SetPoint = led0pwmval;
	//Chassis_speed_R.SetPoint = led0pwmval;
	
	//直线标定
	//goto_1m();
	
	//上位机控制位
	control();
	
	//获得航向角
	GYRO();

	//选择pid
	pid_motor_chose(&Chassis_speed_L,Chassis_speed_L.SetPoint);
	pid_motor_chose(&Chassis_speed_R,Chassis_speed_R.SetPoint);
	
	//速度过大出错复位
	if( F103RC_chassis.speed_error >500) SpeedReset(), F103RC_chassis.speed_error=0;		
}

/**********************************************************************************************************
*函 数 名: goto_1m
*功能说明: 直线走一米
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
static int set_des_flag=0;
void goto_1m(void){
	if(set_des_flag == 0){
		Chassis_position.SetPoint = 100;//1m
		set_des_flag = 1;
	}	
	Chassis_speed_L.SetPoint = LIMIT_MAX_MIN(position_PID_Calc(&Chassis_position, positionNow),30,-30);
	Chassis_speed_R.SetPoint = LIMIT_MAX_MIN(position_PID_Calc(&Chassis_position, positionNow),30,-30);
	
}
/**********************************************************************************************************
*函 数 名: PID_Param_Init
*功能说明: 电机方向选择
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void motor_direction(void)
{
	if(Chassis_speed_L.SetPoint<0)
	{
		motor_L_back();
	}else
	{
		motor_L_move();
	}
	
	
	if(Chassis_speed_R.SetPoint<0)
	{
		motor_R_back();
	}else
	{
		motor_R_move();
	}
	
	if(Chassis_speed_L.SetPoint == 0)motor_L_stop();
	if(Chassis_speed_R.SetPoint == 0)motor_R_stop();
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
		Chassis_speed_L.P = 28;
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
	
		//位置环
		Chassis_position.P = 2.0f;
		Chassis_position.I = 0;
		Chassis_position.D = 2;
		Chassis_position.ErrorMax = 1000.0f;
		Chassis_position.IMax = 1000.0f;
		Chassis_position.SetPoint = 0.0f;	
		Chassis_position.OutMax = 70;	
}

/**********************************************************************************************************
*函 数 名: pid_motor_chose
*功能说明: 电机PID选择
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/

void pid_motor_chose(Pid_Typedef *P, int speed)
{

	//电机pid
	if(speed<71)
	P->I = 0.0005* speed * speed - 0.0199*speed + 2.23;

	if(P == &Chassis_speed_L) P->I = 0.0005* speed * speed - 0.0199*speed + 2.52;
	
}

/**********************************************************************************************************
*函 数 名: control
*功能说明: 执行控制位
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
char temp=0;
void control(void)
{
	if(chassis_ctrl.crtlFlag>>7){
		SoftReset();
	}
	
	temp = chassis_ctrl.crtlFlag << 1;
	if(temp>>7){
		chassis_ctrl.left_move = 1;
	}else{
		chassis_ctrl.left_move = 0;
	}
	
	temp = chassis_ctrl.crtlFlag << 2;
	if(temp>>7){
		chassis_ctrl.right_move = 1;
	}else{
		chassis_ctrl.right_move = 0;
	}
}

/**********************************************************************************************************
*函 数 名: SoftReset
*功能说明: 软件复位
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void SoftReset(void)
{
    __set_FAULTMASK(1); // 关闭所有中断
    NVIC_SystemReset(); // 复位
}

/**********************************************************************************************************
*函 数 名: SpeedReset
*功能说明: 软件复位
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void SpeedReset(void)
{
  Chassis_speed_L.SetPoint = 0;
	Chassis_speed_L.I = 0;
	Chassis_speed_L.SetPointLast = 0;
	Chassis_speed_L.SumError = 0;
	Chassis_speed_L.LastError = 0;
	Chassis_speed_L.PreError = 0;
	motor_R_move();
	
	Chassis_speed_R.SetPoint = 0;
	Chassis_speed_R.I = 0;
	Chassis_speed_R.SetPointLast = 0;
	Chassis_speed_R.SumError = 0;
	Chassis_speed_R.LastError = 0;
	Chassis_speed_R.PreError = 0;
	motor_R_move();
}



/**********************************************************************************************************
*函 数 名: TIM6_IRQHandler
*功能说明: 车辆控制周期 
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
//5ms进一次中断采样
extern int led0pwmval;  
extern int led0pwmval2; 
static short pid_flag=0; 
static short data_send_flag=0;
void TIM6_IRQHandler(void)   //TIM3中断
{
		if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
		{
			
			pid_flag++;
			data_send_flag++;
			
			
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update);   //清除TIMx的中断待处理位:TIM 中断源
					 
			Get_Motor_Speed(&F103RC_chassis.leftSpeedNow,&F103RC_chassis.rightSpeedNow);
			
	
			F103RC_chassis.rightSpeedNow = IIR_TICK_d_R(F103RC_chassis.rightSpeedNow);
			F103RC_chassis.leftSpeedNow = IIR_TICK_d_L(F103RC_chassis.leftSpeedNow);
			
			
			//电机控制
			if(pid_flag == 2){
					//选择电机方向
				motor_direction();
				TIM_SetCompare1(TIM1,speed_PID_Calc(&Chassis_speed_R, F103RC_chassis.rightSpeedNow));	//led0pwmval);//
				TIM_SetCompare2(TIM1,speed_PID_Calc(&Chassis_speed_L, F103RC_chassis.leftSpeedNow));	
				pid_flag = 0;
			}
			
			
			//将需要发送到ROS的数据，从该函数发出，前三个数据范围（-32768 - +32767），第四个数据的范围(0 - 255)
			if(data_send_flag == 4){
				usartSendData(F103RC_chassis.leftSpeedNow,F103RC_chassis.rightSpeedNow,F103RC_chassis.angle,F103RC_chassis.controlFlag);
				data_send_flag = 0;
			}
			
			
			//标定使用
			positionNow += (F103RC_chassis.rightSpeedNow*0.005 + F103RC_chassis.leftSpeedNow*0.005 ) / 2;
			
			
				//速度失控
			if(F103RC_chassis.rightSpeedNow>71 || F103RC_chassis.rightSpeedNow > 71) F103RC_chassis.speed_error++;
		}
}
