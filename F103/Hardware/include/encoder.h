#ifndef __ENCODER_H
#define __ENCODER_H

#include "main.h"


void Encoder_Init_TIM3(void);
void Encoder_Init_TIM5(void);
void TIM6_Init(void);
void Get_Motor_Speed(int *leftSpeed,int *rightSpeed);

#endif
