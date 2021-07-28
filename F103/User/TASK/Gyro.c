#include "Gyro.h"



float pitch,roll,yaw; 		//ŷ����
short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;	//������ԭʼ����
short temp;					//�¶�	
/**********************************************************************************************************
*�� �� ��: Gyro_task
*����˵��: ��������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
uint32_t GYRO_high_water;
void GYRO_task(void *pvParameters){
	portTickType xLastWakeTime;
	const portTickType xFrequency = 1;	
	
	while(1)
	{
 
		
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)!=0)
		{
			temp=MPU_Get_Temperature();	//�õ��¶�ֵ
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
		}
		
		
		vTaskDelay(20); /* ��ʱ 500 �� tick */

	}

}
