#ifndef __BITBAND_H
#define __BITBAND_H
#include "stm32f4xx_conf.h"
#define BITBAND(ADDR,BITNUM) ((ADDR&0xF0000000)+0x2000000+((ADDR&0xFFFFF)<<5)+(BITNUM<<2))
#define MEM_ADDR(ADDR) *(volatile unsigned int *)(ADDR)
#define BIT_BAND(ADDR,BITNUM) MEM_ADDR(BITBAND(ADDR,BITNUM))
#define GPIOA_IDR (GPIOA_BASE+0x10)
#define GPIOA_ODR (GPIOA_BASE+0x14)

#define GPIOB_IDR (GPIOB_BASE+0x10)
#define GPIOB_ODR (GPIOB_BASE+0x14)

#define GPIOC_IDR (GPIOC_BASE+0x10)
#define GPIOC_ODR (GPIOC_BASE+0x14)

#define GPIOD_IDR (GPIOD_BASE+0x10)
#define GPIOD_ODR (GPIOD_BASE+0x14)

#define GPIOE_IDR (GPIOE_BASE+0x10)
#define GPIOE_ODR (GPIOE_BASE+0x14)

#define GPIOF_IDR (GPIOF_BASE+0x10)
#define GPIOF_ODR (GPIOF_BASE+0x14)

#define GPIOG_IDR (GPIOG_BASE+0x10)
#define GPIOG_ODR (GPIOG_BASE+0x14)

#define GPIOH_IDR (GPIOH_BASE+0x10)
#define GPIOH_ODR (GPIOH_BASE+0x14)

#define PAIn(n)  BIT_BAND(GPIOA_IDR,n)
#define PAOut(n) BIT_BAND(GPIOA_ODR,n)

#define PBIn(n)  BIT_BAND(GPIOB_IDR,n)
#define PBOut(n) BIT_BAND(GPIOB_ODR,n)

#define PCIn(n)  BIT_BAND(GPIOC_IDR,n)
#define PCOut(n) BIT_BAND(GPIOC_ODR,n)

#define PDIn(n)  BIT_BAND(GPIOD_IDR,n)
#define PDOut(n) BIT_BAND(GPIOD_ODR,n)

#define PEIn(n)  BIT_BAND(GPIOE_IDR,n)
#define PEOut(n) BIT_BAND(GPIOE_ODR,n)

#define PFIn(n)  BIT_BAND(GPIOF_IDR,n)
#define PFOut(n) BIT_BAND(GPIOF_ODR,n)

#define PGIn(n)  BIT_BAND(GPIOG_IDR,n)
#define PGOut(n) BIT_BAND(GPIOG_ODR,n)

#define PHIn(n)  BIT_BAND(GPIOH_IDR,n)
#define PHOut(n) BIT_BAND(GPIOH_ODR,n)
#endif

