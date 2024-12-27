#ifndef __SHT_H
#define __SHT_H
/*
��GPIO�ķ�ʽ��ģ��IICЭ��
���������ŵ�����ֲ������õ�
ͨ��Ӳ��ԭ��ͼ��֪
ʱ����	IIC_SCL		PB6
������	IIC_SDA		PB7
sht.c �� sht.h �������������������й�SHT30��ʵ��
*/
#include "stm32f10x_conf.h"//�����˸�ͷ�ļ����е�����ͷ�ļ����Ѱ���

#define	SLAVE_ADDR									0x88		//SHT30���豸��ַ
#define PERIODIC_MODE_MEDIUM_1PMPS	0x2737	//����SHT30�Ķ�ȡ����
#define READOUT_PERIODIC_MODE				0xE000	//��SHT30���ݵ�����

extern void sht_init(void);//��ʼ��sht30�ĺ���

extern void sht_write_mode(void);//����sht30�Ķ�ȡ����

extern void sht_write_read_cmd(void);//д���SHT30���ݵ�����

extern void sht_read_data(double sht_data[2]);//��SHT30�ɼ���������

#endif







