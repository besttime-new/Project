#ifndef __LED_H
#define __LED_H
/*
�������豸		�����ӵĹܽ�
LED0					PC1
LED1					PC2
LED2					PC3
��ԭ��ͼ��֪
GPIO�ܽ�����ߵ�ƽ	LED����
GPIO�ܽ�����͵�ƽ	LED����
*/
#include "stm32f10x_conf.h"//�����˸�ͷ�ļ����е�����ͷ�ļ����Ѱ���

extern void led_init(void);//��ʼ���ĺ���(��ʼ�������ӵ�GPIO�ܽ�)

extern void led_on(int nu);//���ݲ���nu������Ӧ��led�Ƶĺ���

extern void led_off(int nu);//���ݲ���nu�ر���Ӧ��led�Ƶĺ���

#endif




