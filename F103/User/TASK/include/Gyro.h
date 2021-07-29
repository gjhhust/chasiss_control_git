#ifndef GYRO_H
#define GYRO_H
#include "main.h"


void GYRO_task(void *pvParameters);
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);

#endif
