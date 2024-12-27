#ifndef __EINT_H
#define __EINT_H
/*
�������豸		�����ӵĹܽ�		��ʹ�õ��ⲿ�жϱ��
KEY0					PC9							EXTI9
KEY1					PC8							EXTI8
��ԭ��ͼ��֪
�����ܰ������¼�⵽GPIO�ܽ�Ϊ				�͵�ƽ
�����ܰ���û�а��¼�⵽GPIO�ܽ�Ϊ		�ߵ�ƽ
��������Ϊ�½��ش����ⲿ�ж�
*/
#include "stm32f10x_conf.h"//�����˸�ͷ�ļ����е�����ͷ�ļ����Ѱ���

typedef void (*eint_handler)(void);//ͨ��typedef���庯��ָ������

extern void eint_init(void);//��ʼ���ĺ���(��ʼ�������ӵ�GPIO�ܽź��жϿ������Լ��ⲿ�ж�)

extern void set_eint_handler(eint_handler h0, eint_handler h1);//���ûص�����

#endif








