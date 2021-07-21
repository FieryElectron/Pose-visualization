#define uchar unsigned char

void oled_Init();
void oled_DrawLine(uchar x0, uchar y0, uchar x1, uchar y1);
void oled_Update();
void oled_PrintString(uchar x, uchar y, uchar ch[]);
void oled_PrintFloat(uchar x, uchar y, float f);
void oled_ShowBorder();
void oled_ShowDate();