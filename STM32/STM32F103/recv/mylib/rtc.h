#ifndef __RTC_H
#define __RTC_H
/*
����Ŀ����ʹ�õ���STM32F103RBT6ϵ�е�оƬ
�ڲ��Ѿ�������RTC����,����ֱ�ӽ���ʹ��
оƬ�ڲ���RTC����ļ�������Ҫ�洢��������
������ൽ2��32�η���1�Ĵ�С,���������޴�
�������������ĵ�ʱ�䷶Χ��
1970/1/1/ 00:00:00 ------ 2099/12/31 23:59:59
����������ǵ�һ������RTC�Ļ���Ҫ����ʱ��
Ҳ����ζ�Ž�������������Ҫ�洢
����:�����Ҫ�洢2019-9-20 09:00:00
��Ҫ�����ʱ�˿̵�1970/1/1/ 00:00:00�ж�����
�洢����������
*/
#include "stm32f10x_conf.h"//�����˸�ͷ�ļ����е�����ͷ�ļ����Ѱ���

struct time_st//����������ʱ��Ľṹ��
{
	int Y;//���
	int M;//�·�
	int D;//����
	int W;//����
	int h;//Сʱ
	int m;//����
	int s;//����
};

extern void rtc_init(struct time_st *t);//��ʼ��RTC

extern void set_time(struct time_st *t);//����ʱ��

extern void conv_time(struct time_st* t);//ת��ʱ��

extern void get_time(struct time_st *t);//��ȡʱ��

#endif








