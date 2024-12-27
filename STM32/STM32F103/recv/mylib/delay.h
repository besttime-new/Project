#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x_conf.h"//�����˸�ͷ�ļ����е�����ͷ�ļ����Ѱ���

#define SYSCLK 72					//ϵͳʱ�ӵĻ���(ע:��72������72M)

typedef uint32_t  u32;		//ͨ��typedef������u32����
typedef uint16_t  u16;		//ͨ��typedef������u16����
typedef uint8_t   u8;			//ͨ��typedef������u8����
	
void delay_init(void);	//��ʱ�����ĳ�ʼ��
void delay_ms(u16 nms);	//���뼶����ʱ����(����Ϊ������)
void delay_us(u32 nus);	//΢�뼶����ʱ����(����Ϊ΢����)

#endif





























