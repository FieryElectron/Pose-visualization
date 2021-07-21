#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <time.h>
#include <stdio.h>

#define uchar unsigned char

#define PIN_SCL 14 //D0
#define PIN_SDA 12 //D1
#define PIN_RST 2  //RST
#define PIN_DC 3   //DC
//PIN CS GPIO_24 WiringPin 10

#define Brightness 0x7F

time_t rawtime;
struct tm *timeinfo;

const uchar F6x8[][6] = {
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // sp
	{0x00, 0x00, 0x00, 0x2f, 0x00, 0x00}, // !
	{0x00, 0x00, 0x07, 0x00, 0x07, 0x00}, // "
	{0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14}, // #
	{0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12}, // $
	{0x00, 0x62, 0x64, 0x08, 0x13, 0x23}, // %
	{0x00, 0x36, 0x49, 0x55, 0x22, 0x50}, // &
	{0x00, 0x00, 0x05, 0x03, 0x00, 0x00}, // '
	{0x00, 0x00, 0x1c, 0x22, 0x41, 0x00}, // (
	{0x00, 0x00, 0x41, 0x22, 0x1c, 0x00}, // )
	{0x00, 0x14, 0x08, 0x3E, 0x08, 0x14}, // *
	{0x00, 0x08, 0x08, 0x3E, 0x08, 0x08}, // +
	{0x00, 0x00, 0x00, 0xA0, 0x60, 0x00}, // ,
	{0x00, 0x08, 0x08, 0x08, 0x08, 0x08}, // -
	{0x00, 0x00, 0x60, 0x60, 0x00, 0x00}, // .
	{0x00, 0x20, 0x10, 0x08, 0x04, 0x02}, // /
	{0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
	{0x00, 0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
	{0x00, 0x42, 0x61, 0x51, 0x49, 0x46}, // 2
	{0x00, 0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
	{0x00, 0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
	{0x00, 0x27, 0x45, 0x45, 0x45, 0x39}, // 5
	{0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
	{0x00, 0x01, 0x71, 0x09, 0x05, 0x03}, // 7
	{0x00, 0x36, 0x49, 0x49, 0x49, 0x36}, // 8
	{0x00, 0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
	{0x00, 0x00, 0x36, 0x36, 0x00, 0x00}, // :
	{0x00, 0x00, 0x56, 0x36, 0x00, 0x00}, // ;
	{0x00, 0x08, 0x14, 0x22, 0x41, 0x00}, // <
	{0x00, 0x14, 0x14, 0x14, 0x14, 0x14}, // =
	{0x00, 0x00, 0x41, 0x22, 0x14, 0x08}, // >
	{0x00, 0x02, 0x01, 0x51, 0x09, 0x06}, // ?
	{0x00, 0x32, 0x49, 0x59, 0x51, 0x3E}, // @
	{0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C}, // A
	{0x00, 0x7F, 0x49, 0x49, 0x49, 0x36}, // B
	{0x00, 0x3E, 0x41, 0x41, 0x41, 0x22}, // C
	{0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
	{0x00, 0x7F, 0x49, 0x49, 0x49, 0x41}, // E
	{0x00, 0x7F, 0x09, 0x09, 0x09, 0x01}, // F
	{0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
	{0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
	{0x00, 0x00, 0x41, 0x7F, 0x41, 0x00}, // I
	{0x00, 0x20, 0x40, 0x41, 0x3F, 0x01}, // J
	{0x00, 0x7F, 0x08, 0x14, 0x22, 0x41}, // K
	{0x00, 0x7F, 0x40, 0x40, 0x40, 0x40}, // L
	{0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
	{0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
	{0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
	{0x00, 0x7F, 0x09, 0x09, 0x09, 0x06}, // P
	{0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
	{0x00, 0x7F, 0x09, 0x19, 0x29, 0x46}, // R
	{0x00, 0x46, 0x49, 0x49, 0x49, 0x31}, // S
	{0x00, 0x01, 0x01, 0x7F, 0x01, 0x01}, // T
	{0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
	{0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
	{0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
	{0x00, 0x63, 0x14, 0x08, 0x14, 0x63}, // X
	{0x00, 0x07, 0x08, 0x70, 0x08, 0x07}, // Y
	{0x00, 0x61, 0x51, 0x49, 0x45, 0x43}, // Z
	{0x00, 0x00, 0x7F, 0x41, 0x41, 0x00}, // [
	{0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55}, // '\'
	{0x00, 0x00, 0x41, 0x41, 0x7F, 0x00}, // ]
	{0x00, 0x04, 0x02, 0x01, 0x02, 0x04}, // ^
	{0x00, 0x40, 0x40, 0x40, 0x40, 0x40}, // _
	{0x00, 0x00, 0x01, 0x02, 0x04, 0x00}, // '
	{0x00, 0x20, 0x54, 0x54, 0x54, 0x78}, // a
	{0x00, 0x7F, 0x48, 0x44, 0x44, 0x38}, // b
	{0x00, 0x38, 0x44, 0x44, 0x44, 0x20}, // c
	{0x00, 0x38, 0x44, 0x44, 0x48, 0x7F}, // d
	{0x00, 0x38, 0x54, 0x54, 0x54, 0x18}, // e
	{0x00, 0x08, 0x7E, 0x09, 0x01, 0x02}, // f
	{0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C}, // g
	{0x00, 0x7F, 0x08, 0x04, 0x04, 0x78}, // h
	{0x00, 0x00, 0x44, 0x7D, 0x40, 0x00}, // i
	{0x00, 0x40, 0x80, 0x84, 0x7D, 0x00}, // j
	{0x00, 0x7F, 0x10, 0x28, 0x44, 0x00}, // k
	{0x00, 0x00, 0x41, 0x7F, 0x40, 0x00}, // l
	{0x00, 0x7C, 0x04, 0x18, 0x04, 0x78}, // m
	{0x00, 0x7C, 0x08, 0x04, 0x04, 0x78}, // n
	{0x00, 0x38, 0x44, 0x44, 0x44, 0x38}, // o
	{0x00, 0xFC, 0x24, 0x24, 0x24, 0x18}, // p
	{0x00, 0x18, 0x24, 0x24, 0x18, 0xFC}, // q
	{0x00, 0x7C, 0x08, 0x04, 0x04, 0x08}, // r
	{0x00, 0x48, 0x54, 0x54, 0x54, 0x20}, // s
	{0x00, 0x04, 0x3F, 0x44, 0x40, 0x20}, // t
	{0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C}, // u
	{0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C}, // v
	{0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C}, // w
	{0x00, 0x44, 0x28, 0x10, 0x28, 0x44}, // x
	{0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C}, // y
	{0x00, 0x44, 0x64, 0x54, 0x4C, 0x44}, // z
	{0x14, 0x14, 0x14, 0x14, 0x14, 0x14}  // horiz lines
};

uchar preArr[8][128] = {0};
uchar nowArr[8][128] = {0};

void oled_SendCmd(uchar cmd)
{
	digitalWrite(PIN_DC, 0);
	wiringPiSPIDataRW(0, &cmd, 1);
}

void oled_SetPos(uchar x, uchar y)
{
	oled_SendCmd(0xb0 + y);
	oled_SendCmd(((x & 0xf0) >> 4) | 0x10);
	oled_SendCmd((x & 0x0f) | 0x00);
}

void oled_SendData(uchar data)
{
	digitalWrite(PIN_DC, 1);
	wiringPiSPIDataRW(0, &data, 1);
	digitalWrite(PIN_DC, 0);
}

void oled_CLS(void)
{
	uchar y, x;
	for (y = 0; y < 8; y++)
	{
		oled_SendCmd(0xb0 + y);
		oled_SendCmd(0x01);
		oled_SendCmd(0x10);
		for (x = 0; x < 128; x++)
			oled_SendData(0);
	}
}

void oled_DELAY_MS(long ms)
{
	long a;
	while (ms)
	{
		a = 1335;
		while (a--)
			;
		ms--;
	}
	return;
}

void oled_SetAddressingMode(unsigned char d)
{
	oled_SendCmd(0x20); //	Set Memory Addressing Mode
	oled_SendCmd(d);	//	Default => 0x02
						//	0x00 => Horizontal Addressing Mode
						//	0x01 => Vertical Addressing Mode
						//	0x02 => Page Addressing Mode
}

void oled_SetStartLine(unsigned char d)
{
	oled_SendCmd(0x40 | d); //	Set Display Start Line
							//	Default => 0x40 (0x00)
}

void oled_SetContrastControl(unsigned char d)
{
	oled_SendCmd(0x81); //	Set Contrast Control
	oled_SendCmd(d);	//	Default => 0x7F
}

void oled_Set_Charge_Pump(unsigned char d)
{
	oled_SendCmd(0x8D);		//	Set Charge Pump
	oled_SendCmd(0x10 | d); //	Default => 0x10
							//	0x10 (0x00) => Disable Charge Pump
							//	0x14 (0x04) => Enable Charge Pump
}

void oled_Set_Segment_Remap(unsigned char d)
{
	oled_SendCmd(0xA0 | d); //	Set Segment Re-Map
							//	Default => 0xA0
							//	0xA0 (0x00) => Column Address 0 Mapped to SEG0
							//	0xA1 (0x01) => Column Address 0 Mapped to SEG127
}

void oled_Set_Entire_Display(unsigned char d)
{
	oled_SendCmd(0xA4 | d); //	Set Entire Display On / Off
							//	Default => 0xA4
							//	0xA4 (0x00) => Normal Display
							//	0xA5 (0x01) => Entire Display On
}

void oled_Set_Inverse_Display(unsigned char d)
{
	oled_SendCmd(0xA6 | d); //	Set Inverse Display On/Off
							//	Default => 0xA6
							//	0xA6 (0x00) => Normal Display
							//  0xA7 (0x01) => Inverse Display On
}

void oled_Set_Multiplex_Ratio(unsigned char d)
{
	oled_SendCmd(0xA8); //	Set Multiplex Ratio
	oled_SendCmd(d);	//	Default => 0x3F (1/64 Duty)
}

void oled_Set_Display_On_Off(unsigned char d)
{
	oled_SendCmd(0xAE | d); //	Set Display On/Off
							//	Default => 0xAE
							//	0xAE (0x00) => Display Off
							//	0xAF (0x01) => Display On
}

void oled_Set_Common_Remap(unsigned char d)
{
	oled_SendCmd(0xC0 | d); //	Set COM Output Scan Direction
							//	Default => 0xC0
							//	0xC0 (0x00) => Scan from COM0 to 63
							//	0xC8 (0x08) => Scan from COM63 to 0
}

void oled_Set_Display_Offset(unsigned char d)
{
	oled_SendCmd(0xD3); //	Set Display Offset
	oled_SendCmd(d);	//	Default => 0x00
}

void oled_Set_Display_Clock(unsigned char d)
{
	oled_SendCmd(0xD5); //	Set Display Clock Divide Ratio / Oscillator Frequency
	oled_SendCmd(d);	//	Default => 0x80
						//	D[3:0] => Display Clock Divider
						//	D[7:4] => Oscillator Frequency
}

void oled_Set_Precharge_Period(unsigned char d)
{
	oled_SendCmd(0xD9); //	Set Pre-Charge Period
	oled_SendCmd(d);	//	Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
						//	D[3:0] => Phase 1 Period in 1~15 Display Clocks
						//	D[7:4] => Phase 2 Period in 1~15 Display Clocks
}

void oled_Set_Common_Config(unsigned char d)
{
	oled_SendCmd(0xDA);		//	Set COM Pins Hardware Configuration
	oled_SendCmd(0x02 | d); //	Default => 0x12 (0x10)
							//	Alternative COM Pin Configuration
							//	Disable COM Left/Right Re-Map
}

void oled_Set_VCOMH(unsigned char d)
{
	oled_SendCmd(0xDB); //	Set VCOMH Deselect Level
	oled_SendCmd(d);	//	Default => 0x20 (0.77*VCC)
}

void oled_Init()
{
	wiringPiSetup();
	wiringPiSPISetup(0, 16000000); //500,000 to 32,000,000

	pinMode(PIN_RST, OUTPUT);
	pullUpDnControl(PIN_RST, PUD_UP);
	pinMode(PIN_DC, OUTPUT);
	pullUpDnControl(PIN_DC, PUD_UP);

	digitalWrite(PIN_SCL, 1);
	digitalWrite(PIN_DC, 1);

	digitalWrite(PIN_RST, 0);
	oled_DELAY_MS(50);
	digitalWrite(PIN_RST, 1);

	oled_Set_Display_On_Off(0x00);		 // Display Off (0x00/0x01)
	oled_Set_Display_Clock(0x80);		 // Set Clock as 100 Frames/Sec
	oled_Set_Multiplex_Ratio(0x3F);		 // 1/64 Duty (0x0F~0x3F)
	oled_Set_Display_Offset(0x00);		 // Shift Mapping RAM Counter (0x00~0x3F)
	oled_SetStartLine(0x00);			 // Set Mapping RAM Display Start Line (0x00~0x3F)
	oled_Set_Charge_Pump(0x04);			 // Enable Embedded DC/DC Converter (0x00/0x04)
	oled_SetAddressingMode(0x02);		 // Set Page Addressing Mode (0x00/0x01/0x02)
	oled_Set_Segment_Remap(0x01);		 // Set SEG/Column Mapping     0x00 left right flip 0x01 normal
	oled_Set_Common_Remap(0x08);		 // Set COM/Row Scan Direction 0x00 up down flip 0x08 normal
	oled_Set_Common_Config(0x10);		 // Set Sequential Configuration (0x00/0x10)
	oled_SetContrastControl(Brightness); // Set SEG Output Current
	oled_Set_Precharge_Period(0xF1);	 // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	oled_Set_VCOMH(0x40);				 // Set VCOM Deselect Level
	oled_Set_Entire_Display(0x00);		 // Disable Entire Display On (0x00/0x01)
	oled_Set_Inverse_Display(0x00);		 // Disable Inverse Display On (0x00/0x01)
	oled_Set_Display_On_Off(0x01);		 // Display On (0x00/0x01)
	oled_CLS();
}

uchar oled_IsValid(uchar value)
{
	if (value >= 0 && value < 64)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void oled_SetPix(uchar x, uchar y, uchar value)
{
	uchar row = y / 8;
	uchar num = y % 8;
	switch (value)
	{
	case 0:
		nowArr[row][x] &= ~(1 << num);
		break;
	default:
		nowArr[row][x] |= (1 << num);
		break;
	}
}

void oled_DrawLine(uchar x0, uchar y0, uchar x1, uchar y1)
{
	int steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep)
	{
		int tx = x0;
		x0 = y0;
		y0 = tx;
		int ty = x1;
		x1 = y1;
		y1 = ty;
	}
	if (x0 > x1)
	{
		int tx = x0;
		x0 = x1;
		x1 = tx;
		int ty = y0;
		y0 = y1;
		y1 = ty;
	}
	int dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);
	int err = dx / 2;
	int ystep;
	if (y0 < y1)
	{
		ystep = 1;
	}
	else
	{
		ystep = -1;
	}
	for (; x0 <= x1; x0++)
	{
		if (!oled_IsValid(x0) || !oled_IsValid(y0))
		{
			continue;
		}
		if (steep)
		{
			oled_SetPix(x0, y0, 1);
		}
		else
		{
			oled_SetPix(y0, x0, 1);
		}
		err -= dy;
		if (err < 0)
		{
			y0 += ystep;
			err += dx;
		}
	}
}

void oled_ClearArr()
{
	for (uchar r = 0; r < 8; ++r)
	{
		for (uchar c = 0; c < 128; ++c)
		{
			nowArr[r][c] = 0;
		}
	}
}

void oled_OverWritePixelCol(uchar col, uchar row, uchar value)
{
	oled_SetPos(col, row);
	oled_SendData(value);
}

void oled_SetPixCol(uchar x, uchar y, uchar value)
{
	for (uchar i = 0; i < 8; i++)
	{
		oled_SetPix(x, y + i, value & (1 << i));
	}
}

void oled_P6x8Str(uchar x, uchar y, uchar ch)
{
	uchar c = ch - 32;
	for (uchar i = 0; i < 6; i++)
	{
		oled_SetPixCol(x + i, y, F6x8[c][i]);
	}
}

void oled_PrintString(uchar x, uchar y, uchar ch[])
{
	uchar i = 0;
	while (ch[i] != '\0')
	{
		oled_P6x8Str(x % 128, y + (x / 127) * 8, ch[i]);
		x += 6;
		i += 1;
	}
}

void float2char(float slope, char *buffer, int n)
{
	int temp, i, j;
	if (slope >= 0)
	{
		buffer[0] = '+';
	}
	else
	{
		buffer[0] = '-';
		slope = -slope;
	}
	temp = (int)slope;
	for (i = 0; temp != 0; i++)
		temp /= 10;
	temp = (int)slope;
	for (j = i; j > 0; j--)
	{
		buffer[j] = temp % 10 + '0';
		temp /= 10;
	}
	buffer[i + 1] = '.';
	slope -= (int)slope;
	for (i = i + 2; i < n - 1; i++)
	{
		slope *= 10;
		buffer[i] = (int)slope + '0';
		slope -= (int)slope;
	}
	buffer[n - 1] = '\0';
}

void oled_PrintFloat(uchar x, uchar y, float f)
{
	char buffer[8];
	float2char(f, buffer, 8);
	oled_PrintString(x, y, (uchar *)buffer);
}

void oled_ShowBorder()
{
	oled_DrawLine(0, 0, 0, 63);
	oled_DrawLine(63, 0, 63, 63);
	oled_DrawLine(0, 0, 63, 0);
	oled_DrawLine(0, 63, 63, 63);
}

void oled_Update()
{
	for (uchar r = 0; r < 8; ++r)
	{
		for (uchar c = 0; c < 128; ++c)
		{
			if (preArr[r][c] != nowArr[r][c])
			{
				preArr[r][c] = nowArr[r][c];
				oled_OverWritePixelCol(c, r, preArr[r][c]);
			}
		}
	}
	oled_ClearArr();
}

void oled_ShowDate()
{
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	char year[5] = {0};
	sprintf(year, "%d", timeinfo->tm_year + 1900);
	char month[3] = {0};
	sprintf(month, "%02d", timeinfo->tm_mon + 1);
	char day[3] = {0};
	sprintf(day, "%02d", timeinfo->tm_mday);
	char hour[3] = {0};
	sprintf(hour, "%02d", timeinfo->tm_hour);
	char min[3] = {0};
	sprintf(min, "%02d", timeinfo->tm_min);
	char sec[3] = {0};
	sprintf(sec, "%02d", timeinfo->tm_sec);

	oled_PrintString(65, 40, (uchar *)year);
	oled_PrintString(89, 40, (uchar *)"/");

	oled_PrintString(95, 40, (uchar *)month);
	oled_PrintString(107, 40, (uchar *)"/");

	oled_PrintString(113, 40, (uchar *)day);

	oled_PrintString(70, 50, (uchar *)hour);
	oled_PrintString(82, 50, (uchar *)":");

	oled_PrintString(88, 50, (uchar *)min);
	oled_PrintString(100, 50, (uchar *)":");

	oled_PrintString(106, 50, (uchar *)sec);
}