#ifndef __PWM_H
#define __PWM_H
#include "main.h"

#define IN1  GPIO_Pin_10  //PA10
#define IN2  GPIO_Pin_12  //PA12

#define IN3  GPIO_Pin_11  //PA11
#define IN4  GPIO_Pin_12  //PA15

void TIM1_PWM_Init(u16 arr,u16 psc);


#endif
