# Pose-visualization

Retrieve accelerometer data from MPU6050 and visualize the graphical pos via oled by simulating a simple 3D plane model on the Raspberry Pi.

## Compile & Run
    gcc -Wall ./MPU6050/inv_mpu_dmp_motion_driver.c ./MPU6050/inv_mpu.c ./OLED/oled.c ./Transform/transform.c main.c -o main -lm -lwiringPi && ./main
<img src="Animation.gif" width="200" height="200" />