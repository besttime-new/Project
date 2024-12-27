#include "eint.h"
#include "led.h"
#include "delay.h"
#include "beep.h"
#include "rtc.h"
#include "lcd.h"
#include "string.h"
#include "stdio.h"
#include "start.h"
#include "music.h"
#include "nrf24l01.h"
#include "select_mode.h"
#define WAIT_TIME 80  	 // 设置为三十秒后进入待机
extern int flag_status;	 // 状态标志位----1:确定   0:返回
extern int flag_timeout; // 超时标志位
extern int flag_cur_fun; // 当前所在的功能标志位
int main(void)
{
	// RTC的起始时间和获取的实时时间
	struct time_st old_t = {14, 10, 0, 0x40}; // 起始时间
	struct date_st old_d = {5, 11, 29, 24};	  // 起始日期
	struct time_st new_t = {0};				  // 实时时间
	struct date_st new_d = {0};				  // 实时日期
	// NRF各个通道的地址
	u8 rxAddr[5][5] = {{0x11, 0x11, 0x11, 0x22, 0x22},	// 通道0的地址
					   {0x12, 0x20, 0x30, 0x40, 0x50},	// 通道1的地址
					   {0x13, 0x20, 0x30, 0x40, 0x50},	// 通道2的地址
					   {0x14, 0x20, 0x30, 0x40, 0x50},	// 通道3的地址
					   {0x15, 0x20, 0x30, 0x40, 0x50}}; // 通道4的地址
	u8 recvLen[5] = {32, 32, 32, 32, 32};				// 每个通道传输数据的宽度
	int data[2]={0};                                    // 存储温湿度
	struct nrf_msg_st msg;								// 自定义结构体类型<32byte>存放需要传输的数据
	int flag_clear = 1;									// 清屏标志位
	int ret = 0;										// 接收返回值
	//int i = 0;											// 循环变量
	delay_init();										// 系统时钟初始化
	led_init();											// LED初始化
	eint_init();										// 中断初始化
	beep_init();										// 蜂鸣器初始化
	LCD_Init();											// LCD初始化
	SPI_NRF_Init();										// NRF初始化
	rtc_Initconf(&old_t, &old_d);						// 初始化RTC
	BRUSH_COLOR = GREEN;								// 初始画色笔为绿色
	BACK_COLOR = BLACK;									// 初始化背景为红色
	LCD_Clear(BACK_COLOR);								// 清屏LCD
	show_start_1();                                     // 开机界面--开机第一部分
	//MUSIC1_Play();                                      // 蜂鸣器音乐
	LCD_Clear(WHITE);				                    // 清屏白色			    
    show_start_2();                                     //加载界面——开机第二部分
	LCD_Clear(BLACK);                                   //清屏黑色 
	ret = NRF_Check();				                    // 检测NRF是否存在
	if(ret==0)
	LCD_DisplayString(0,0,24,(u8 *)"LINK ERROR!");       //检测连接是否成功
	NRF_RX_Mode(rxAddr, 21, recvLen);                    // 设置接收吗模式(打开NRF的通道0~4)
	while (1)
	{
		ret = NRF_Rx_Dat((u8 *)&msg, sizeof(msg));          // 接收通道各个通道传输的数据
		while (flag_timeout > WAIT_TIME)		            // 判断是否超时
		{
			rtc_get_time(&new_t);              //获取实时时间
			rtc_get_date(&new_d);	           //获取实时日期
			LCD_Display_wait(&new_t,&new_d);   //显示待机界面
			delay_ms(1000);                    //延时1s
			flag_clear=0;                      //清屏标志置零
		}
		if(flag_clear==0)//退出待机界面进行清屏
		{
			LCD_Clear(BLACK);//清屏
			flag_clear=1;//清屏标志位置1
		}
		switch (flag_cur_fun) //功能选择
		{
		case FUN0:
		{
			if (!flag_status)
				// 选择功能1界面
                LCD_DISplay_select_DHT();//显示选择DHT11界面
				if (flag_status)
				{
					// 具体功能1界面
					LCD_DISplay_mode_DHT(msg.Dht_Hum,msg.Dht_Temp); //DHT11功能界面
				}  
		}
		break;
		case FUN1:
		{
			if (!flag_status)
				// 选择功能2界面
                LCD_DISplay_select_SHT();//显示选择SHT30界面
				if (flag_status)
				{
					// 具体功能2界面
                    data[0]=msg.Sht_Hum*100;
					data[1]=msg.Sht_Temp*100;
					LCD_DISplay_mode_SHT(data[0],data[1]); //SHT30功能界面
				}
		}
		break;
		case FUN2:
		{
			if (!flag_status)
				// 选择功能3界面
                LCD_DISplay_select_ADC(); //显示选择ADC界面
				if (flag_status)
				{
					// 具体功能3界面
					LCD_DISplay_mode_ADC(msg.Data_ADC); //ADC功能界面
				}
		}
		break;
		case FUN3:
		{
			if (!flag_status)
				// 选择功能4界面
                LCD_DISplay_select_TIME();//显示选择秒表界面
				if (flag_status)
				{
					// 具体功能4界面
				    LCD_DISplay_mode_TIME(msg.Data_time); //TIME功能界面
				}
		}
		break;
		/*case FUN4:
		{
			if (!flag_status)
				// 选择功能5界面

				if (flag_status)
				{
					// 具体功能5界面
					
				}
		}
		break;*/
		}
		delay_ms(100); //延时100ms
		flag_timeout++; //超时标志位加一
	}
}
