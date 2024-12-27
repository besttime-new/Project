#ifndef __SELECT_MODE_H
#define __SELECT_MODE_H
#include "stm32f4xx_conf.h"
#include "rtc.h"
extern void LCD_Draw_Paper_r(const unsigned char *picture, u16 startX, u16 startY, u16 picWidth, u16 picHeight);//将图片字模显示在LCD上
extern void LCD_Display_wait(struct time_st *new_t,struct date_st *new_d);//显示待机界面
extern void LCD_DISplay_select_ADC(void);//显示ADC界面
extern void LCD_DISplay_select_TIME(void);//显示秒表界面
extern void LCD_DISplay_select_DHT(void);//显示DHT11界面
extern void LCD_DISplay_select_SHT(void);//显示SHT30界面
extern void LCD_DISplay_mode_DHT(int Dht_Hum,int Dht_Temp); //DHT11功能界面
extern void LCD_DISplay_mode_ADC(int Date_ADC); //ADC功能界面
extern void LCD_DISplay_mode_TIME(int Data_TIME); //TIME功能界面
extern void LCD_DISplay_mode_SHT(int Sht_Temp,int Sht_Hum); //SHT30功能界面

#endif


