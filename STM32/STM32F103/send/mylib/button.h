#ifndef __BUTTON_H
#define __BUTTON_H
/*
�������豸		�����ӵĹܽ�
KEY0					PC9
KEY1					PC8
��ԭ��ͼ��֪
�����ܰ������¼�⵽GPIO�ܽ�Ϊ				�͵�ƽ
�����ܰ���û�а��¼�⵽GPIO�ܽ�Ϊ		�ߵ�ƽ
*/
#include "stm32f10x_conf.h"//�����˸�ͷ�ļ����е�����ͷ�ļ����Ѱ���

extern void button_init(void);//��ʼ���ĺ���(��ʼ�������ӵ�GPIO�ܽ�)

extern int button_status(int nu);//���ݲ���nu�����Ӧ�Ĺ��ܰ����Ƿ񱻰���

#endif







