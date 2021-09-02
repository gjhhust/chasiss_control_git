#include "main.h" 

/*************************************************
包含接线说明
1.电机右 TIM1_pwm:PA10 逻辑控制PA10 PC10  编码器:TIM3 :PA7  PA6
2.电机左 TIM1_pwm:PA9 逻辑控制PC7 PC9	 编码器:TIM5 :PA0  PA 1
3.MPU6050(Gyro) IIC: 	SCL_PB10
											SDA_PB11
  
4.串口通信 UART2 	TX  PA2
									RX  PA3


******************************************************/

void BSP_init(void);

int main(void)
{


	 BSP_init();
   delay_ms(500);
	
	
	 startTast();
   vTaskStartScheduler();
   while (1){}
		 
}


void BSP_init(void)
{
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	TIM2_Init();
	TIM4_Init();
	
	
		/**************************电机**************************/
	Encoder_Init_TIM3();				//TIM3编码器模式初始化，A6、A7分别作为A相和B相的脉冲输入
	TIM1_PWM_Init(899,0);//不分频。PWM频率=72000/(899+1)=80Khz 
	Encoder_Init_TIM5();				//TIM5编码器模式初始化，A1、A2分别作为A相和B相的脉冲输入
	TIM6_Init();//作为计算转速定时器使用
	
	mpu_dmp_init();
	
	UART2_DMA_Init();
	
	
	//ESP8266_Init ();                                                        //初始化WiFi模块使用的接口和外设
	//ESP8266_StaTcpClient_Unvarnish_ConfigTest();       //模式设置
}

