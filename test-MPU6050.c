#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "MPU6050/inv_mpu.h"
#include "MPU6050/inv_mpu_dmp_motion_driver.h"


//gcc -Wall ./MPU6050/inv_mpu_dmp_motion_driver.c ./MPU6050/inv_mpu.c test-MPU6050.c -o test-MPU6050 -lm -lwiringPi && ./test-MPU6050

int main (void)
{ 
	if (wiringPiSetup()==-1) 
	{
		printf("wiringPiSetup failed\n");
		return -1;
	}
	if(mpu_dmp_init())
	{
		printf("dmp failed\n");
		return 1;
	}
	float pitch,roll,yaw;

	while(1){ 
		while (mpu_dmp_get_data(&pitch, &roll, &yaw) != 0) {}
		printf("pitch:%f\troll:%f\tyaw:%f\n",pitch,roll,yaw);
	}
	return 0 ;
}


