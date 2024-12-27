#include "ldt.h"//����ͷ�ļ�
#include "bitband.h"//����ͷ�ļ�
#include "delay.h"//����ͷ�ļ�

#define	HC138_A0		PCOut(11)//��PC11��������ɺ�
#define	HC138_A1		PCOut(12)//��PC12��������ɺ�
#define	HC138_A2		PDOut(2)//��PD2��������ɺ�
#define	HC595_SCLK	PBOut(5)//��PB5��������ɺ�
#define	HC595_LCLK	PBOut(4)//��PB4��������ɺ�
#define	HC595_DATA	PBOut(3)//��PB3��������ɺ�

void ldt_init(void)//��ʼ�����õ���GPIO�ܽ�
{
	GPIO_InitTypeDef Gpio_Value;//������GPIO�̼��Ľṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	//ͨ��APB2����ʹ��GPIOB�� GPIOC�� GPIOD�� �� AFIO ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ı�ָ���ܽŵ�ӳ�� GPIO_Remap_SWJ_JTAGDisable,JTAG-DP ���� + SW-DP ʹ�� 
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;//ѡ������������ģʽ
	Gpio_Value.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3;//ѡ����5�� 4�� 3��GPIO�ܽ�
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//ѡ����50MHz������ٶ�
	GPIO_Init(GPIOB, &Gpio_Value);//�����������ó�ʼ��GPIOB��Ĺܽ�
	
	Gpio_Value.GPIO_Pin = GPIO_Pin_11 |GPIO_Pin_12;//ѡ����11�� 12��GPIO�ܽ�
	GPIO_Init(GPIOC, &Gpio_Value);//�����������ó�ʼ��GPIOC��Ĺܽ�
	
	Gpio_Value.GPIO_Pin = GPIO_Pin_2;//ѡ����2��GPIO�ܽ�
	GPIO_Init(GPIOD, &Gpio_Value);//�����������ó�ʼ��GPIOD��Ĺܽ�
	
	HC138_A0 = 0;//��HC138_A0�����ӵ�GPIO�ܽ�����
	HC138_A1 = 0;//��HC138_A1�����ӵ�GPIO�ܽ�����
	HC138_A2 = 0;//��HC138_A2�����ӵ�GPIO�ܽ�����
	HC595_SCLK = 0;//��HC595_SCLK�����ӵ�GPIO�ܽ�����
	HC595_LCLK = 0;//��HC595_LCLK�����ӵ�GPIO�ܽ�����
	HC595_DATA = 0;//��HC595_DATA�����ӵ�GPIO�ܽ�����
}

void hc138_out_data(uint8_t data)//����74HC138ģ��ѡ�񹫹���
{
	switch(data)//ͨ����֧�������ʹ�õ�����һ�������
	{
		case 0 : HC138_A0 = 0; HC138_A1 = 0; HC138_A2 = 0; break;//0�������ʹ��
		case 1 : HC138_A0 = 1; HC138_A1 = 0; HC138_A2 = 0; break;//1�������ʹ��
		case 2 : HC138_A0 = 0; HC138_A1 = 1; HC138_A2 = 0; break;//2�������ʹ��
		case 3 : HC138_A0 = 1; HC138_A1 = 1; HC138_A2 = 0; break;//3�������ʹ��
	}
}

void hc595_send_data(uint8_t data)//����74HC595ģ�鷢������
{
	int i = 0;//ѭ������
	
	for(i = 0; i < 8; i++)//ѭ��8��
	{
		HC595_SCLK = 0;//��HC595_SCLK����
		if(data & 0x80)//�ж�data�����е����λ�Ƿ�Ϊ��
			HC595_DATA = 1;//��HC595_DATA����
		else
			HC595_DATA = 0;//��HC595_DATA����
		data <<= 1;//data��λ���Ƶ���1
		HC595_SCLK = 1;//��HC595_SCLK����
	}
	HC595_LCLK = 0;//��HC595_LCLK����
	HC595_LCLK = 1;//��HC595_LCLK����
}

void digit_show_data(int data)//�����������ʾ����
{
	uint8_t d[4] = {0};//�洢��ʾ���ݵ�ÿһλ
	uint8_t i = 0;//ѭ������
	uint8_t table[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
	//�������������ʾ0~F������
	
	d[0] = data / 1000;//ȡ��data��ǧλ
	d[1] = data / 100 % 10;//ȡ��data�İ�λ
	d[2] = data / 10 % 10;//ȡ��data��ʮλ
	d[3] = data % 10;//ȡ��data�ĸ�λ
	
	for(i = 0; i < 4; i++)//ѭ��4��
	{
		hc138_out_data(i);//ѡ����λ����������ܵĹ�����
		hc595_send_data(table[d[i]]);//���ʹ�������
		delay_ms(1);//��ʱ1����
		hc595_send_data(0);////���ʹ�������
	}
}








