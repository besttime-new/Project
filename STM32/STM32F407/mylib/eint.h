#ifndef __EINT_H
#define __EINT_H
#include "stm32f4xx_conf.h"
typedef void (*enit_handler)(void);
extern void eint_init(void);
extern void set_eint_init(enit_handler arr[3]);
enum A
{
	FUN0,
	FUN1,
	FUN2,
	FUN3,
	FUN4
}; // 各个功能标志
#endif

