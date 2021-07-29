#include "start_task.h"

uint32_t CPU_high_water;

#define START_TASK_PRIO 1  //任务优先级
#define START_STK_SIZE 256 //任务堆栈
static TaskHandle_t StartTask_Handler; //任务句柄

////底盘任务
#define CHASSIS_TASK_PRIO 18  //任务优先级
#define CHASSIS_STK_SIZE 256 //任务堆栈
static TaskHandle_t chassis_Handler; //任务句柄

//陀螺仪任务
#define GYRO_TASK_PRIO 15  //任务优先级
#define GYRO_STK_SIZE 256 //任务堆栈
static TaskHandle_t Gyro_Handler; //任务句柄

//数据发送任务
#define DATA_DISPOSE_TASK_PRIO 19  //任务优先级
#define DATA_DISPOSE_STK_SIZE 128 //任务堆栈
static TaskHandle_t Data_Dispose_Handler; //任务句柄


#define CPU_TASK_PRIO 5  //任务优先级
#define CPU_STK_SIZE 256 //任务堆栈
static TaskHandle_t CPUTask_Handler; //任务句柄

uint8_t CPU_RunInfo1[200]; //保存任务运行时间信息 分别是：任务名 任务状态 优先级 剩余栈 任务序号
uint8_t CPU_RunInfo2[200]; //保存任务运行时间信息 分别是：任务名 运行计数  使用率


void start_task(void *pvParameters)
{
  taskENTER_CRITICAL();
	
	xTaskCreate((TaskFunction_t)CPU_task,          //任务函数
                (const char *)"CPU_task",          //任务名称
                (uint16_t)CPU_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)CPU_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&CPUTask_Handler); //任务句柄
	
	xTaskCreate((TaskFunction_t)CHASSIC_task,          //任务函数
                (const char *)"CHASSIC_task",          //任务名称
                (uint16_t)CHASSIS_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)CHASSIS_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&chassis_Handler); //任务句柄
		
							
	xTaskCreate((TaskFunction_t)DATA_DISPOSE_task,          //任务函数
					(const char *)"DATA_DISPOSE_task",          //任务名称
					(uint16_t)DATA_DISPOSE_STK_SIZE,            //任务堆栈大小
					(void *)NULL,                        //传递给任务函数的参数
					(UBaseType_t)DATA_DISPOSE_TASK_PRIO,        //任务优先级
					(TaskHandle_t *)&Data_Dispose_Handler); //任务句柄
					
	xTaskCreate((TaskFunction_t)GYRO_task,          //任务函数
					(const char *)"GYRO_task",          //任务名称
					(uint16_t)GYRO_STK_SIZE,            //任务堆栈大小
					(void *)NULL,                        //传递给任务函数的参数
					(UBaseType_t)GYRO_TASK_PRIO,        //任务优先级
					(TaskHandle_t *)&Gyro_Handler); //任务句柄
							
								
								
	vTaskDelete(StartTask_Handler); //删除开始任务
  taskEXIT_CRITICAL();            //退出临界区
}



void CPU_task(void *pvParameters)
{
	

	
   while (1) {
    memset(CPU_RunInfo1,0,200); //信息缓冲区清零
 
    vTaskList((char *)&CPU_RunInfo1); //获取任务运行时间信息
    
    memset(CPU_RunInfo2,0,200); //信息缓冲区清零
 
    vTaskGetRunTimeStats((char *)&CPU_RunInfo2);
    vTaskDelay(10); /* 延时 500 个 tick */
		 
//#if INCLUDE_uxTaskGetStackHighWaterMark
//        CPU_high_water = uxTaskGetStackHighWaterMark(NULL);
//#endif
    }
}
void startTast(void)
{
    xTaskCreate((TaskFunction_t)start_task,          //任务函数
                (const char *)"start_task",          //任务名称
                (uint16_t)START_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)START_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&StartTask_Handler); //任务句柄
}

