#ifndef __LDT_H
#define __LDT_H
/*
�������豸		�����ӵĹܽ�
HC138_A0			PC11
HC138_A1			PC12
HC138_A2			PD2
HC595_SCLK		PB5
HC595_LCLK		PB4
HC595_DATA		PB3
��֪оƬ�Ĺܽ��Ƕ๦�ܸ����͵Ĺܽ�
оƬ�ĹܽŻ��� ���빦�� ������� ���ù��� ģ�⹦��
������β����������Ϊ��λ�������������
������������74HC138
LED�ƵĶ�������74HC595
*/
#include "stm32f10x_conf.h"//�����˸�ͷ�ļ����е�����ͷ�ļ����Ѱ���

extern void ldt_init(void);//��ʼ������ܵĺ���(��ʼ�������ӵ�GPIO�ܽ�)

extern void hc138_out_data(uint8_t data);//����74HC138ģ��ѡ�񹫹���

extern void hc595_send_data(uint8_t data);//����74HC595ģ�鷢������

extern void digit_show_data(int data);//�����������ʾ����

#endif







