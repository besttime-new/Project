#include "start.h"
#include "delay.h"
#include "lcd.h"
#include "cfont_start.h"
void show_picture(u16 x, u16 y, u16 column, u16 row, unsigned short *pic)
{
  u16 m, h;
  u16 *data = (u16 *)pic;
  for (h = 0 + y; h < row + y; h++) // 60
  {
    for (m = 0 + x; m < column + x; m++) // 180
    {
      LCD_Color_DrawPoint(m,h , *data++);
    }
  }
}
void show_start_1(void)
{
  int i = 0;
  for (i = 0; i < 3; i++)
  {
    show_picture(40, 0, 240, 240, (u16 *)gImage_start[i]);
    delay_ms(1000);
  }
}
void show_start_2(void)
{
  int n=0,num=0;
  show_picture(70,10,179,179,(u16 *)gImage_daijik);
	for(n=0;n<6;n=(n+1)%6)
	{
	show_picture(60,200,211,40,(u16 *)gImage_daiji_1_6[n]);
	delay_ms(500);
	num++;
	if(num>20)
	break;
	}
}

