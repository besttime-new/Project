#ifndef __BUZZER_H
#define __BUZZER_H
/*
�������豸		�����ӵĹܽ�
BEEP					PC7
��ԭ��ͼ��֪
GPIO�ܽ�����ߵ�ƽ	��������
GPIO�ܽ�����͵�ƽ	����������
*/
#include "stm32f10x_conf.h"//�����˸�ͷ�ļ����е�����ͷ�ļ����Ѱ���

extern void buzzer_init(void);//��ʼ���ĺ���(��ʼ�������ӵ�GPIO�ܽ�)

extern void buzzer_on(void);//ʹ��������ĺ���

extern void buzzer_off(void);//ʹ����������ĺ���

#endif






