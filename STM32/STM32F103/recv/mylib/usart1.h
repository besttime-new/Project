#ifndef __USART1_H
#define __USART1_H
/*
�������豸		�����ӵĹܽ�
USART1_TX					PA9
USART1_RX					PA10
��֪оƬ�Ĺܽ��Ƕ๦�ܸ����͵Ĺܽ�
оƬ�ĹܽŻ��� ���빦�� ������� ���ù��� ģ�⹦��
�������ʹ�õ��Ǵ���,����ѡ���ù���
*/
#include "stm32f10x_conf.h"//�����˸�ͷ�ļ����е�����ͷ�ļ����Ѱ���

typedef void (*usart1_handler)(unsigned char);//ͨ��typedef���庯��ָ������

extern void usart_1_init(void);//��ʼ���ĺ���(��ʼ�������ӵ�GPIO�ܽźʹ��ڹ����Լ��жϿ�����)

extern void usart_1_send_byte(unsigned char c);//����һ���ֽ�����

extern void usart_1_send_data(char *buf);//����һ���ַ�������

extern unsigned char usart_1_recv_byte(void);//����һ���ֽ�����(��ѯ�ķ�ʽ)

extern void set_usart1_handler(usart1_handler h);//���ûص�����

#endif









