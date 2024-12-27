#include "sht.h"//����ͷ�ļ�
#include "gpio_iic.h"//����ͷ�ļ�
#include "delay.h"//����ͷ�ļ�
#include "led.h"//����ͷ�ļ�

void sht_init(void)//��ʼ��sht30�ĺ���
{
	IIC_Init();//��ʼ��IICʹ�õ�GPIO
	delay_us(250);//��ʱ250΢��
	IIC_Start();//������ʼ�ź�
	IIC_Send_Byte(SLAVE_ADDR);//����IIC���߷�������(�����豸��ַ)
	while(IIC_Wait_Ack());//�ȴ�ACK
	IIC_Send_Byte(PERIODIC_MODE_MEDIUM_1PMPS >> 8);//����IIC���߷�������(����sht30�Ķ�ȡ����)
	while(IIC_Wait_Ack());//�ȴ�ACK
	IIC_Send_Byte(PERIODIC_MODE_MEDIUM_1PMPS & 0xFF);//����IIC���߷�������(����sht30�Ķ�ȡ����)
	while(IIC_Wait_Ack());//�ȴ�ACK
	IIC_Stop();//���������ź�
	delay_us(150);//��ʱ150΢��
}

void sht_write_mode(void)//����sht30�Ķ�ȡ����
{
	IIC_Start();//������ʼ�ź�
	IIC_Send_Byte(SLAVE_ADDR);//����IIC���߷�������(�����豸��ַ)
	while(IIC_Wait_Ack());//�ȴ�ACK
	IIC_Send_Byte(PERIODIC_MODE_MEDIUM_1PMPS >> 8);//����IIC���߷�������(����sht30�Ķ�ȡ����)
	while(IIC_Wait_Ack());//�ȴ�ACK
	IIC_Send_Byte(PERIODIC_MODE_MEDIUM_1PMPS & 0xFF);//����IIC���߷�������(����sht30�Ķ�ȡ����)
	while(IIC_Wait_Ack());//�ȴ�ACK
	IIC_Stop();//���������ź�
	delay_ms(5);//��ʱ5����
}

void sht_write_read_cmd(void)//д���SHT30���ݵ�����
{
	IIC_Start();//������ʼ�ź�
	IIC_Send_Byte(SLAVE_ADDR);//����IIC���߷�������(�����豸��ַ)
	while(IIC_Wait_Ack());//�ȴ�ACK
	IIC_Send_Byte(READOUT_PERIODIC_MODE >> 8);//����IIC���߷�������(���Ͷ�SHT30���ݵ�����)
	while(IIC_Wait_Ack());//�ȴ�ACK
	IIC_Send_Byte(READOUT_PERIODIC_MODE & 0xFF);//����IIC���߷�������(���Ͷ�SHT30���ݵ�����)
	while(IIC_Wait_Ack());//�ȴ�ACK
}

void sht_read_data(double sht_data[2])//��SHT30�ɼ���������
{
	uint8_t buf[6] = {0};//�洢SHT30����������
	uint16_t Temp = 0;//�洢�¶�����
	uint16_t Hum = 0;//�洢ʪ������
	
	IIC_Start();//������ʼ�ź�
	IIC_Send_Byte(SLAVE_ADDR | 0x01);//����IIC���߷�������(�����豸��ַ)
	delay_us(5);//��ʱ5΢��
	while(IIC_Wait_Ack());//�ȴ�ACK
	buf[0] = IIC_Recv_Byte(1);//����IIC���߽�������
	buf[1] = IIC_Recv_Byte(1);//����IIC���߽�������
	buf[2] = IIC_Recv_Byte(1);//����IIC���߽�������
	buf[3] = IIC_Recv_Byte(1);//����IIC���߽�������
	buf[4] = IIC_Recv_Byte(1);//����IIC���߽�������
	buf[5] = IIC_Recv_Byte(0);//����IIC���߽�������
	IIC_Stop();//���������ź�
	
	Temp = buf[0] & 0xFF;
	Temp <<= 8;
	Temp |= buf[1] & 0xFF;//���¶ȵĸ߰�λ�͵Ͱ�λ���
	Hum = buf[3] & 0xFF;
	Hum <<= 8;
	Hum |= buf[4] & 0xFF;//��ʪ�ȵĸ߰�λ�͵Ͱ�λ���
	
	sht_data[0] = (double)(-45 + 175 * (double)(Temp) / 65535);//ͨ��ת����ʽת�����¶�(С�������λ)
	sht_data[1] = (double)(100 * (double)(Hum) / 65535);//ͨ��ת����ʽת����ʪ��(С�������λ)
}








