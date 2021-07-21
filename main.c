#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "MPU6050/inv_mpu.h"
#include "MPU6050/inv_mpu_dmp_motion_driver.h"

#include "OLED/oled.h"

#include "Transform/transform.h"

//gcc -Wall ./MPU6050/inv_mpu_dmp_motion_driver.c ./MPU6050/inv_mpu.c ./OLED/oled.c ./Transform/transform.c main.c -o main -lm -lwiringPi && ./main

int main(void)
{
    oled_Init();
    mpu6050_init();


    float pitch, roll, yaw;

    while (1)
    {
        while (mpu_dmp_get_data(&pitch, &roll, &yaw) != 0) {}

        trans_Update3DLines(roll, pitch, -yaw);

        oled_PrintString(65, 2, (uchar *)"P:");
        oled_PrintString(65, 10, (uchar *)"R:");
        oled_PrintString(65, 18, (uchar *)"Y:");

        oled_PrintFloat(80, 2, pitch);
        oled_PrintFloat(80, 10, roll);
        oled_PrintFloat(80, 18, yaw);

        oled_PrintString(68, 29, (uchar *)"-OLED 3D-");

        oled_ShowBorder();
        oled_ShowDate();

        oled_Update();
    }

    return 0;
}
