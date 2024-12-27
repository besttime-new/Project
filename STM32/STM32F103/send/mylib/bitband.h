#ifndef __BITBAND_H
#define __BITBAND_H

#include "stm32f10x_conf.h"//�����˸�ͷ�ļ����е�����ͷ�ļ����Ѱ���

#define BIT_BAND(ADDR,BITNUM) ((ADDR & 0xF0000000) + 0x2000000 + ((ADDR & 0xFFFFF) << 5) + (BITNUM << 2))
//�����ĺ궨���ǰ���������ڴ���֧��λ�����ʵĵ�ַ ת���� λ���������ĵ�ַ
#define MEM_ADDR(ADDR) *(volatile unsigned int *)(ADDR)
//��ʮ����������ת���������ĵ�ַ
#define BITBAND(ADDR,BITNUM) MEM_ADDR(BIT_BAND(ADDR,BITNUM))
//ʹ���������궨����

#define GPIOA_IDR_ADDR	(GPIOA_BASE + 0x08)
#define GPIOA_ODR_ADDR	(GPIOA_BASE + 0x0C)

#define GPIOB_IDR_ADDR	(GPIOB_BASE + 0x08)
#define GPIOB_ODR_ADDR	(GPIOB_BASE + 0x0C)

#define GPIOC_IDR_ADDR	(GPIOC_BASE + 0x08)
#define GPIOC_ODR_ADDR	(GPIOC_BASE + 0x0C)

#define GPIOD_IDR_ADDR	(GPIOD_BASE + 0x08)
#define GPIOD_ODR_ADDR	(GPIOD_BASE + 0x0C)
//������GPIOÿһ���е�������ݼĴ������������ݼĴ����ĵ�ַ���ɺ궨��

#define PAOut(n)	BITBAND(GPIOA_ODR_ADDR,n)
#define PAIn(n)		BITBAND(GPIOA_IDR_ADDR,n)

#define PBOut(n)	BITBAND(GPIOB_ODR_ADDR,n)
#define PBIn(n)		BITBAND(GPIOB_IDR_ADDR,n)

#define PCOut(n)	BITBAND(GPIOC_ODR_ADDR,n)
#define PCIn(n)		BITBAND(GPIOC_IDR_ADDR,n)

#define PDOut(n)	BITBAND(GPIOD_ODR_ADDR,n)
#define PDIn(n)		BITBAND(GPIOD_IDR_ADDR,n)
//������GPIO�������������λ���ĺ궨��

#endif





