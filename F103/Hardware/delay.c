#include "main.h"
int delayms, delayus;

/**
  * @brief  配置Tim3定时器，作微秒延时（近似2us）
  * @param  None
  * @retval None
  */
//void TIM3_Init(void)
//{
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;

//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

//    TIM_TimeBaseInitStructure.TIM_Period = 1; 	
//    TIM_TimeBaseInitStructure.TIM_Prescaler = 71;  
//    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 

//    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

//    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 
//    TIM_Cmd(TIM3, DISABLE); 

//    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//}

///**
//  * @brief  TIM3定时中断执行事务
//  * @param  None
//  * @retval None
//  */
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
//	{
//        delayus--;
//	}
//	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
//}


void delay_us(int tim) 
{
//    delayus = tim;
//    TIM_Cmd(TIM3, ENABLE);
//    while(delayus > 0);
//    TIM_Cmd(TIM3, DISABLE);
	tim *=11.935318;
	while(tim!=0){
		tim--;
	}
}


void TIM2_Init(void)
{
   TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 

    TIM_TimeBaseInitStructure.TIM_Period = 9; 	
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7199;  
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 
    TIM_Cmd(TIM2, DISABLE); 

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
	 
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        delayms--;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

void delay_ms(int tim)			
{
	  delayms = tim;
    TIM_Cmd(TIM2, ENABLE);
    while(delayms != 0);
    TIM_Cmd(TIM2, DISABLE);
}


