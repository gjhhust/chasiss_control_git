#include "Gyro.h"



float pitch,roll,yaw; 		//欧拉角
short aacx,aacy,aacz;		//加速度传感器原始数据
short gyrox,gyroy,gyroz;	//陀螺仪原始数据
short temp;					//温度	
/**********************************************************************************************************
*函 数 名: Gyro_task
*功能说明: 底盘任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
uint32_t GYRO_high_water;
void GYRO_task(void *pvParameters){
	portTickType xLastWakeTime;
	const portTickType xFrequency = 1;	
	
	while(1)
	{
 
		
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)!=0)
		{
			temp=MPU_Get_Temperature();	//得到温度值
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
		}
		
		
		vTaskDelay(20); /* 延时 500 个 tick */

	}

}
