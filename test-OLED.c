#include <wiringPi.h>
#include "./OLED/oled.h"

//gcc -Wall test-OLED.c ./OLED/oled.c -o test-OLED -lwiringPi && ./test-OLED

int main(void)
{
	oled_Init();

	oled_PrintString(0, 0, (uchar *)"123");

	oled_Update();

	return 0;
}