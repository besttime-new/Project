#ifndef __ADJ_RES_H
#define __ADJ_RES_H
/*
�������豸			�����ӵĹܽ�
�߾��ȿɵ�����	PC0/ADC_IN10
��֪оƬ�Ĺܽ��Ƕ๦�ܸ����͵Ĺܽ�
оƬ�ĹܽŻ��� ���빦�� ������� ���ù��� ģ�⹦��
��ԭ��ͼ��֪���������й�ADת����ģ�����Ԥ����GPIO�ӿ�����
�������ϵĸ߾��ȿɵ����������ӵ�GPIO�ܽ�Ҳ��ADת������
ADC_VOL		PC0/ADC_IN10
*/
#include "stm32f10x_conf.h"//�����˸�ͷ�ļ����е�����ͷ�ļ����Ѱ���

extern void adj_res_init(void);//��ʼ���߾��ȿɵ�����ĺ���(��ʼ�������ӵ�GPIO�ܽ��Լ�ADת��)

extern int get_adj_res_value(void);//��ȡADת��֮��Ľ��

#endif





