#include "main.h"
/**
  * @brief  PID反馈算法
  * @param  PID_Struct *P  PID参数结构体
  *         ActualValue    PID计算反馈量（当前真实检测值）
  * @retval PID反馈计算输出值
  */
	
	//只输出正数
float speed_PID_Calc(Pid_Typedef *P, float ActualValue)
{
		P->PreError = P->SetPoint - ActualValue;
		P->dError = P->PreError - P->LastError;
	
	  P->SetPointLast = P->SetPoint;
	
		if(P->PreError > -P->ErrorMax && P->PreError < P->ErrorMax)
		{
			P->SumError += P->PreError; 
		}
		
		P->LastError = P->PreError;
		
		if(P->SumError >= P->IMax)
			P->SumError = P->IMax;
		else if(P->SumError <= -P->IMax)
			P->SumError = -P->IMax;
		
		P->POut = P->P * P->PreError;
		P->IOut = P->I * P->SumError;
		P->DOut = P->D * P->dError;
		
		
		return ABS(LIMIT_MAX_MIN(P->POut+P->IOut+P->DOut,P->OutMax,-P->OutMax)); 
}
	//只输出正负都会
float position_PID_Calc(Pid_Typedef *P, float ActualValue)
{
		P->PreError = P->SetPoint - ActualValue;
		P->dError = P->PreError - P->LastError;
	
	  P->SetPointLast = P->SetPoint;
	
		if(P->PreError > -P->ErrorMax && P->PreError < P->ErrorMax)
		{
			P->SumError += P->PreError; 
		}
		
		P->LastError = P->PreError;
		
		if(P->SumError >= P->IMax)
			P->SumError = P->IMax;
		else if(P->SumError <= -P->IMax)
			P->SumError = -P->IMax;
		
		P->POut = P->P * P->PreError;
		P->IOut = P->I * P->SumError;
		P->DOut = P->D * P->dError;
		
		
		return LIMIT_MAX_MIN(P->POut+P->IOut+P->DOut,P->OutMax,-P->OutMax);
}


float PID_realize(pid * P,float actual_val) {
	/* 计算目标值与实际值的误差 */
	P->err=P->target_val-actual_val; /*PID 算法实现 */
	P->actual_val += P->Kp*(P->err -P->err_next) 
									+ P->Ki*P->err
									+ P->Kd*(P->err - 2 * P->err_next +P->err_last);
	
	
	/* 传递误差 */
	P->err_last = P->err_next; P->err_next = P->err; /* 返回当前实际值 */
	return LIMIT_MAX_MIN(P->actual_val,P->out_Max,-P->out_Max);;
}

