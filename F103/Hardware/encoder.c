#include "encoder.h"

extern int leftSpeedNow;//左右电机当前速度
extern int rightSpeedNow;
extern Chassis F103RC_chassis;
extern Pid_Typedef Chassis_speed_L;
extern Pid_Typedef Chassis_speed_R;
/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式 右轮
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM3(void)
{
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
        TIM_ICInitTypeDef TIM_ICInitStructure;  
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    //使能定时器2的时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //使能PA端口时钟

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;        //端口配置
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入
        GPIO_Init(GPIOA, &GPIO_InitStructure);                                        //根据设定参数初始化GPIOA

        TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
        TIM_TimeBaseStructure.TIM_Prescaler = 0x0;              //预分频器 
        TIM_TimeBaseStructure.TIM_Period = 65535;  //设定计数器自动重装值
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //选择时钟分频：不分频
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数  
        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
        
        TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
        
        TIM_ICStructInit(&TIM_ICInitStructure);
        TIM_ICInitStructure.TIM_ICFilter = 10;
        TIM_ICInit(TIM3, &TIM_ICInitStructure);
        TIM_ClearFlag(TIM3, TIM_FLAG_Update);                   //清除TIM的更新标志位
        TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
        //Reset counter
        TIM_SetCounter(TIM3,0);
        //===============================================
        TIM3->CNT = 0x7fff;
        //===============================================
        TIM_Cmd(TIM3, ENABLE); 
}
/**************************************************************************
函数功能：把TIM5初始化为编码器接口模式 左轮
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM5(void)
{
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
        TIM_ICInitTypeDef TIM_ICInitStructure;  
        GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);    //使能定时器4的时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //使能PB端口时钟

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;        //端口配置
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入
        GPIO_Init(GPIOA, &GPIO_InitStructure);                                        //根据设定参数初始化GPIOA

        TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
        TIM_TimeBaseStructure.TIM_Prescaler = 0x0;              // 预分频器 
        TIM_TimeBaseStructure.TIM_Period = 65535;  //设定计数器自动重装值
        TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //选择时钟分频：不分频
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数  
        TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
        TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	
        TIM_ICStructInit(&TIM_ICInitStructure);
        TIM_ICInitStructure.TIM_ICFilter = 10;
        TIM_ICInit(TIM5, &TIM_ICInitStructure);
        TIM_ClearFlag(TIM5, TIM_FLAG_Update);                   //清除TIM的更新标志位
        TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
        //Reset counter
        TIM_SetCounter(TIM5,0);
        //===============================================
        TIM5->CNT = 0x7fff;
        //===============================================
        TIM_Cmd(TIM5, ENABLE); 
}

/**************************************************************************
函数功能：读取编码器脉冲差值
入口参数：TIM_TypeDef * TIMx
返回  值：无
**************************************************************************/
s16 getTIMx_DetaCnt(TIM_TypeDef * TIMx)
{
        s16 cnt;
        cnt = TIMx->CNT-0x7fff;     //这一点默认认为，单位时间内编码器不会出现0x7fff的脉冲变化，事实上也是这样
        TIMx->CNT = 0x7fff;
        return cnt;
}

/**************************************************************************
函数功能：计算左右轮速
入口参数：int *leftSpeed,int *rightSpeed
返回  值：无
                //计算左右车轮线速度，正向速度为正值 ，反向速度为负值
                //一定时间内的编码器变化值*转化率（转化为直线上的距离m）*200s（5ms计算一次） 得到 m/s *1000转化为int数据
								
							 左 3300 = 11 4 75
							右 1980 = 11 * 4 * 45
                一圈的脉冲数：
                        左：  11
                        右：  11
                轮子周长：13cm
                一个脉冲的距离：
                        左： 0.13/17
                        右： 0.13/17
**************************************************************************/

void Get_Motor_Speed(int *leftSpeed,int *rightSpeed)
{
        static int leftWheelEncoderNow    = 0;
        static int rightWheelEncoderNow   = 0;
        static int leftWheelEncoderLast   = 0;
        static int rightWheelEncoderLast  = 0;        
        
        //记录本次左右编码器数据
        leftWheelEncoderNow += getTIMx_DetaCnt(TIM5);
        rightWheelEncoderNow+= getTIMx_DetaCnt(TIM3);
                
        //5ms测速            
				//变大测速会使得该轮子跑慢
        *leftSpeed   =  0.75  * 13 * (leftWheelEncoderNow - leftWheelEncoderLast) * 200 /  3300;  //速度为cm/s
        *rightSpeed  =  0.88  * 13 * (rightWheelEncoderNow - rightWheelEncoderLast)* 200 / 1980;

        //记录上次编码器数据
        leftWheelEncoderLast  = leftWheelEncoderNow;                    
        rightWheelEncoderLast = rightWheelEncoderNow;                   
}


/**************************************************************************
函数功能：开启TIM6用以定时测速
入口参数：无
返回  值：无
**************************************************************************/
void TIM6_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); 

    TIM_TimeBaseInitStructure.TIM_Period = 50-1; 	
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200-1;  
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);
		TIM_ARRPreloadConfig(TIM6, ENABLE); //使能TIMx在ARR上的预装载寄存器

    

    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
		TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM6, ENABLE); 		
}



