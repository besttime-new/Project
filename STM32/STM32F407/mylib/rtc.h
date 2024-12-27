#ifndef __RTC_H
#define __RTC_H

#include "stm32f4xx_conf.h"
struct time_st
{
    u8 hours;
    u8 minutes;
    u8 seconds;
    u8 RTC_H12; //AM 0x00 /PM  0x40
};
struct date_st
{
    u8 week;
    u8 month;
    u8 day;
    u8 year;
};
extern void rtc_Initconf(struct time_st *t,struct date_st *d);
extern void rtc_get_time(struct time_st *t);
extern void rtc_get_date(struct date_st *d);


#endif

