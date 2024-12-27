#include "eeprom.h"//����ͷ�ļ�
#include "gpio_iic.h"//����ͷ�ļ�

void eeprom_init(void)//��ʼ��eeprom�ĺ���
{
	IIC_Init();//��ʼ��IICʹ�õ�GPIO
}

void eeprom_byte_write(u8 daddr, u8 waddr, u8 data)//���ֽ���д����
{
	IIC_Start();//������ʼ�ź�
	IIC_Send_Byte(daddr);//����IIC���߷�������(�����豸��ַ)
	IIC_Wait_Ack();//�ȴ�ACK
	IIC_Send_Byte(waddr);//����IIC���߷�������(�����ڲ��洢�ռ��ַ)
	IIC_Wait_Ack();//�ȴ�ACK
	IIC_Send_Byte(data);//����IIC���߷�������(��������)
	IIC_Wait_Ack();//�ȴ�ACK
	IIC_Stop();//���������ź�
}

void eeprom_page_write(u8 daddr, u8 waddr, u8 *buf, u8 size)//��ҳ��д����
{
	u8 i = 0;//ѭ������
	
	IIC_Start();//������ʼ�ź�
	IIC_Send_Byte(daddr);//����IIC���߷�������(�����豸��ַ)
	IIC_Wait_Ack();//�ȴ�ACK
	IIC_Send_Byte(waddr);//����IIC���߷�������(�����ڲ��洢�ռ��ַ)
	IIC_Wait_Ack();//�ȴ�ACK
	for(i = 0; i < size; i++)//��ѯsize��
	{
		IIC_Send_Byte(buf[i]);//����IIC���߷�������(��������)
		IIC_Wait_Ack();//�ȴ�ACK
	}
	IIC_Stop();//���������ź�
}

u8 eeprom_cur_addr_read(u8 daddr)//��ǰ��ַ������
{
	u8 data = 0;//���ڽ�������
	
	IIC_Start();//������ʼ�ź�
	IIC_Send_Byte(daddr | 1);//����IIC���߷�������(�����豸��ַ)
	IIC_Wait_Ack();//�ȴ�ACK
	data = IIC_Recv_Byte(0);//����IIC���߽�������
	IIC_Stop();//���������ź�
	
	return data;//�ѻ�ȡ�������ݷ���
}

u8 eeprom_rand_read(u8 daddr, u8 waddr)//�漴��ַ������
{
	u8 data = 0;//���ڽ�������
	
	IIC_Start();//������ʼ�ź�
	IIC_Send_Byte(daddr);//����IIC���߷�������(�����豸��ַ)
	IIC_Wait_Ack();//�ȴ�ACK
	IIC_Send_Byte(waddr);//����IIC���߷�������(�����ڲ��洢�ռ��ַ)
	IIC_Wait_Ack();//�ȴ�ACK
	IIC_Start();//������ʼ�ź�
	IIC_Send_Byte(daddr | 1);//����IIC���߷�������(�����豸��ַ)
	IIC_Wait_Ack();//�ȴ�ACK
	data = IIC_Recv_Byte(0);//����IIC���߽�������
	IIC_Stop();//���������ź�
	
	return data;//�ѻ�ȡ�������ݷ���
}

void eeprom_sequ_read(u8 daddr, u8 waddr, u8 *buf, u8 size)//˳���ַ������
{
	u8 i = 0;//ѭ������
	
	IIC_Start();//������ʼ�ź�
	IIC_Send_Byte(daddr);//����IIC���߷�������(�����豸��ַ)
	IIC_Wait_Ack();//�ȴ�ACK
	IIC_Send_Byte(waddr);//����IIC���߷�������(�����ڲ��洢�ռ��ַ)
	IIC_Wait_Ack();//�ȴ�ACK
	IIC_Start();//������ʼ�ź�
	IIC_Send_Byte(daddr | 1);//����IIC���߷�������(�����豸��ַ)
	IIC_Wait_Ack();//�ȴ�ACK
	for(i = 0; i < size; i++)//��ѯsize��
	{
		if(i == size - 1)//�ж��Ƿ������һ�ζ�����
			buf[i] = IIC_Recv_Byte(0);//����IIC���߽�������
		else
			buf[i] = IIC_Recv_Byte(1);//����IIC���߽�������
	}
	IIC_Stop();//���������ź�
}








