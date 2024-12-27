#include "rtc.h"
#include "string.h"
void rtc_get_time(struct time_st *t)
{
	   RTC_TimeTypeDef rtc_time;
       RTC_GetTime(RTC_Format_BIN,&rtc_time);
	   memcpy(t,&rtc_time,sizeof(struct time_st));
}

void rtc_get_date(struct date_st *d)
{
	   RTC_DateTypeDef rtc_date;
       RTC_GetDate(RTC_Format_BIN,&rtc_date);
	   memcpy(d,&rtc_date,sizeof(struct date_st));
}
void rtc_Initconf(struct time_st *t, struct date_st *d)
{
    RTC_InitTypeDef rtc_conf;
    RTC_TimeTypeDef rtc_time;
    RTC_DateTypeDef rtc_date;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); // 使能PWR的时钟
    PWR_BackupAccessCmd(ENABLE);                        // 打开写访问  
    if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x7678)
    {                 
        RCC_LSEConfig(RCC_LSE_ON);                          // 打开LSE时钟源
        while (RTC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); // 选择LSE作为RTC的时钟源
        RCC_RTCCLKCmd(ENABLE);                  // 使能RTC时钟
        rtc_conf.RTC_AsynchPrediv = 127;     // 异步预分频(硬件会自动加一)
        rtc_conf.RTC_SynchPrediv = 255;      // 同步预分频(硬件会自动加一)
        rtc_conf.RTC_HourFormat =RTC_HourFormat_24;  //24小时制
        RTC_Init(&rtc_conf); //初始化RTC
        
        rtc_date.RTC_Year=d->year;
        rtc_date.RTC_Month=d->month;
        rtc_date.RTC_Date=d->day;
        rtc_date.RTC_WeekDay=d->week;
        RTC_SetDate(RTC_Format_BIN,&rtc_date);

        rtc_time.RTC_Hours=t->hours;
        rtc_time.RTC_Minutes=t->minutes;
        rtc_time.RTC_Seconds=t->seconds;
        rtc_time.RTC_H12=RTC_H12_AM; //AM(0-12:00)  PM(12-24:00)
        RTC_SetTime(RTC_Format_BIN,&rtc_time);
        RTC_WriteBackupRegister(RTC_BKP_DR0, 0x7678); //向BKP寄存器中写入特定的值
    }
}

