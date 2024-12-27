#include "select_mode.h"
#include "lcd.h"
#include "cfont_mode.h"
void LCD_Draw_Paper_r(const unsigned char *picture, u16 startX, u16 startY, u16 picWidth, u16 picHeight)
{
    u16 i, j, k;
    u16 x;

    for (i = 0; i < picHeight; i++)
    {
        for (j = 0; j < picWidth / 8; j++)
        {
            for (k = 0; k < 8; k++)
            {
                x = (((picture[i * (picWidth / 8) + j] << k) & 0x80) >> 7);
                LCD_Color_DrawPoint(startX + j * 8 + k, startY + i, x ? CYAN : BLACK);
            }
        }
    }
}
void show_pictures(u16 x, u16 y, u16 column, u16 row, unsigned short *pic)
{
    u16 m, h;
    u16 *data = (u16 *)pic;
    for (h = 0 + y; h < row + y; h++) // 60
    {
        for (m = 0 + x; m < column + x; m++) // 180
        {
            LCD_Color_DrawPoint(m, h, *data++);
        }
    }
}
void LCD_Display_wait(struct time_st *new_t, struct date_st *new_d) // 显示待机界面
{
    LCD_Draw_Paper_r(gImage_daiji, 0, 0, 320, 240);     // 待机背景
    LCD_DisplayNum(40, 35, new_d->year+2000, 4, 24, 0);      // 年
    LCD_DisplayNum(140, 35, new_d->month, 2, 24, 0);    // 月
    LCD_DisplayNum(230, 35, new_d->day, 2, 24, 0);      // 日
    LCD_DisplayNum(260, 85, new_d->week, 2, 24, 0);     // 星期
    LCD_DisplayNum(45, 135, new_t->hours, 2, 24, 0);    // 小时
    LCD_DisplayNum(105, 135, new_t->minutes, 2, 24, 0); // 分钟
    LCD_DisplayNum(170, 135, new_t->seconds, 2, 24, 0); // 秒
}
void LCD_DISplay_select_ADC(void) // 显示ADC界面
{
    LCD_Draw_Paper_r(gImage_ADC, 40, 0, 240, 240); // ADC转换
}
void LCD_DISplay_select_TIME(void) // 显示秒表界面
{
    LCD_Draw_Paper_r(gImage_Selection, 40, 0, 240, 240); // 秒表
}
void LCD_DISplay_select_DHT(void) // 显示DHT11界面
{
    LCD_Draw_Paper_r(gImage_culvwendu, 40, 0, 240, 240); // 粗略温湿度
}
void LCD_DISplay_select_SHT(void) // 显示SHT30界面
{
    LCD_Draw_Paper_r(gImage_jingquwendu, 40, 0, 240, 240); // 精确温湿度
}
void segment(int arr[4], unsigned int k)
{
    arr[0] = k / 1000;
    arr[1] = (k / 100) % 10;
    arr[2] = (k / 10) % 10;
    arr[3] = k % 10;
}

void hmin(int arr[6], unsigned int k) // 分割计时器时间
{
    int h = 0, m = 0, s = 0;
    s = k % 60;
    h = k / 3600;
    m = (k - (h * 3600)) / 60;
    arr[0] = h / 10;
    arr[1] = h % 10;
    arr[2] = m / 10;
    arr[3] = m % 10;
    arr[4] = s / 10;
    arr[5] = s % 10;
}
void LCD_DISplay_mode_DHT(int Dht_Hum, int Dht_Temp) // DHT11功能界面
{
    int arr[4] = {0};
    arr[0] = Dht_Hum / 10;
    arr[1] = Dht_Hum % 10;
    arr[2] = Dht_Temp / 10;
    arr[3] = Dht_Temp % 10;
    show_pictures(40, 0, 240, 240, (u16 *)gImage_wendu);
    show_pictures(110, 55, 33, 42, (u16 *)gImage_0_9[arr[2]]);
    show_pictures(143, 55, 33, 42, (u16 *)gImage_0_9[arr[3]]);
    show_pictures(176, 55, 36, 44, (u16 *)gImage_sheshidu);
    show_pictures(110, 97, 33, 42, (u16 *)gImage_0_9[arr[0]]);
    show_pictures(143, 97, 33, 42, (u16 *)gImage_0_9[arr[1]]);
    show_pictures(176, 97, 36, 44, (u16 *)gImage_baifenhao);
}
void LCD_DISplay_mode_ADC(int Data_ADC) // ADC功能界面
{
    int arr[4] = {0};
    segment(arr, Data_ADC);
    show_pictures(40, 0, 240, 240, (u16 *)gImage_adc);
    show_pictures(116, 67, 33, 42, (u16 *)gImage_0_9[arr[0]]);
    show_pictures(149, 67, 33, 42, (u16 *)gImage_0_9[arr[1]]);
    show_pictures(182, 67, 33, 42, (u16 *)gImage_0_9[arr[2]]);
    show_pictures(215, 67, 33, 42, (u16 *)gImage_0_9[arr[3]]);
}
void LCD_DISplay_mode_TIME(int Data_TIME) // TIME功能界面
{
    int arr[4] = {0};
    segment(arr, Data_TIME);
    show_pictures(40, 0, 240, 240, (u16 *)gImage_miaobiao);
    show_pictures(41, 95, 33, 42, (u16 *)gImage_0_9[arr[0]]);
    show_pictures(74, 95, 33, 42, (u16 *)gImage_0_9[arr[1]]);
    show_pictures(107, 95, 16, 42, (u16 *)gImage_maohao);
    show_pictures(123, 95, 33, 42, (u16 *)gImage_0_9[arr[2]]);
    show_pictures(156, 95, 33, 42, (u16 *)gImage_0_9[arr[3]]);
    /*show_picture(189,95,16,42,(u16 *)gImage_maohao);
    show_picture(205,95,33,42,(u16 *)gImage_0_9[arr[4]]);
    show_picture(238,95,33,42,(u16 *)gImage_0_9[arr[5]]);*/
}
void LCD_DISplay_mode_SHT(int Sht_Hum,int Sht_Temp) //SHT30功能界面
{
    int arr[4]={0};
    int arrs[4]={0};
    segment(arr,Sht_Temp);
	show_pictures(40,0,240,240,(u16 *)gImage_wendu);
   	show_pictures(102,55,33,42,(u16 *)gImage_0_9[arr[0]]);
	show_pictures(135,55,33,42,(u16 *)gImage_0_9[arr[1]]);
	show_pictures(168,55,9,42,(u16*)gImage_xiaoshudian);
	show_pictures(177,55,33,42,(u16 *)gImage_0_9[arr[2]]); 
	show_pictures(210,55,33,42,(u16 *)gImage_0_9[arr[3]]); 
	show_pictures(243,55,36,44,(u16 *)gImage_sheshidu);

	segment(arrs,Sht_Hum);
	show_pictures(102,97,33,42,(u16 *)gImage_0_9[arrs[0]]); 
	show_pictures(135,97,33,42,(u16 *)gImage_0_9[arrs[1]]);
	show_pictures(168,97,9,42,(u16*)gImage_xiaoshudian);
	show_pictures(177,97,33,42,(u16 *)gImage_0_9[arrs[2]]); 
	show_pictures(210,97,33,42,(u16 *)gImage_0_9[arrs[3]]);  
	show_pictures(243,97,36,44,(u16 *)gImage_baifenhao);
}


