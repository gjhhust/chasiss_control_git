#ifndef DATA_H
#define DATA_H
#include "main.h"


/*--------------------------------发送协议-----------------------------------
//----------------55 aa size 00 00 00 00 00 crc8 0d 0a----------------------
//数据头55aa + 数据字节数size + 数据（利用共用体） + 校验crc8 + 数据尾0d0a
//注意：这里数据中预留了一个字节的控制位，其他的可以自行扩展，更改size和数据
--------------------------------------------------------------------------*/

/*--------------------------------接收协议-----------------------------------
//----------------55 aa size 00 00 00 00 00 crc8 0d 0a----------------------
//数据头55aa + 数据字节数size + 数据（利用共用体） + 校验crc8 + 数据尾0d0a
//注意：这里数据中预留了一个字节的控制位，其他的可以自行扩展，更改size和数据
--------------------------------------------------------------------------*/


#define START   0X11
#define reBiggestSize 13
#define SendBiggestSize  13

//发送数据（左轮速、右轮速、角度）共用体（-32767 - +32768）
typedef union 
{
	short d;
	unsigned char data[2];
}sendData;

//左右轮速控制速度共用体
typedef union
{
	short d;
	unsigned char data[2];
}receiveData;


//控制车体结构体
typedef struct{
	int leftSpeedSet;
	int rightSpeedSet;
	unsigned char crtlFlag;
}Ctrl_information;


void DATA_DISPOSE_task(void *pvParameters);
	
//串口接受中断中接收函数
void usartReceiveData(void);

//从linux接收并解析数据到参数地址中
extern int usartReceiveOneData(int *p_leftSpeedSet,int *p_rightSpeedSet,unsigned char *p_crtlFlag);   


//封装数据，调用USART1_Send_String将数据发送给linux
extern void usartSendData(short leftVel, short rightVel,short angle,unsigned char ctrlFlag); 


//发送指定字符数组的函数
void USART_Send_String(unsigned char *p,unsigned short sendSize); 


//计算八位循环冗余校验，得到校验值，一定程度上验证数据的正确性
unsigned char getCrc8(unsigned char *ptr, unsigned short len); 


void JudgeBuffReceive(unsigned char ReceiveBuffer[]);
#endif
