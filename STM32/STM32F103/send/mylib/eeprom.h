#ifndef __EEPROM_H
#define __EEPROM_H
/*
��GPIO�ķ�ʽ��ģ��IICЭ��
���������ŵ�����ֲ������õ�
ͨ��Ӳ��ԭ��ͼ��֪
ʱ����	IIC_SCL		PB6
������	IIC_SDA		PB7
eeprom.c �� eeprom.h �������������������й�EEPROM��ʵ��
*/
#include "stm32f10x_conf.h"//�����˸�ͷ�ļ����е�����ͷ�ļ����Ѱ���

extern void eeprom_init(void);//��ʼ��eeprom�ĺ���

extern void eeprom_byte_write(u8 daddr, u8 waddr, u8 data);//���ֽ���д����

extern void eeprom_page_write(u8 daddr, u8 waddr, u8 *buf, u8 size);//��ҳ��д����

extern u8 eeprom_cur_addr_read(u8 daddr);//��ǰ��ַ������

extern u8 eeprom_rand_read(u8 daddr, u8 waddr);//�漴��ַ������

extern void eeprom_sequ_read(u8 daddr, u8 waddr, u8 *buf, u8 size);//˳���ַ������

#endif










