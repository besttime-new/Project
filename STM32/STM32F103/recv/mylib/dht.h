#ifndef __DHT_H
#define __DHT_H
/*
�������豸		�����ӵĹܽ�
DHT11					PC10
��֪оƬ�Ĺܽ��Ƕ๦�ܸ����͵Ĺܽ�
оƬ�ĹܽŻ��� ���빦�� ������� ���ù��� ģ�⹦��
������β����Ĵ�������DHT11�ӿ���GPIO
���Լ���Ҫ����ģʽҲ��Ҫ���ģʽ
*/
#include "stm32f10x_conf.h"//�����˸�ͷ�ļ����е�����ͷ�ļ����Ѱ���

extern void dht_init(void);//��ʼ���ĺ���(��ʼ�������ӵ�GPIO�ܽ�)

extern void get_dht_value(char *buf);//��ȡDHT11�ɼ���������

#endif




