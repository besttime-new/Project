#ifndef __GPIO_IIC_H
#define __GPIO_IIC_H
/*
��GPIO�ķ�ʽ��ģ��IICЭ��
���������ŵ�����ֲ������õ�
ͨ��Ӳ��ԭ��ͼ��֪
ʱ����	IIC_SCL		PB6
������	IIC_SDA		PB7
*/
#include "stm32f10x_conf.h"//�����˸�ͷ�ļ����е�����ͷ�ļ����Ѱ���

typedef unsigned char u8;//ͨ��typedef����u8����

extern void IIC_Init(void);									//��ʼ��IICʹ�õ�GPIO

extern void IIC_Start(void);								//������ʼ�ź�

extern void IIC_Stop(void);									//���������ź�

extern void IIC_Send_Byte(u8 txd);					//����IIC���߷�������(һλһλ�ķ���)

extern u8 IIC_Recv_Byte(unsigned char ack);	//����IIC���߽�������

extern u8 IIC_Wait_Ack(void);								//�ȴ�ACK

extern void IIC_Ack(void);									//����ACK

extern void IIC_NAck(void);									//������ACK

#endif
















