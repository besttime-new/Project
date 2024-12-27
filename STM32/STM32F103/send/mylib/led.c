#include "led.h"//����ͷ�ļ�
#include "bitband.h"//�����й�λ����ͷ�ļ�

void led_init(void)//��ʼ���ĺ���(��ʼ�������ӵ�GPIO�ܽ�)
{
	GPIO_InitTypeDef Led_Value;//������GPIO�̼��Ľṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//ͨ��APB2����ʹ��GPIOC���ʱ��
	
	Led_Value.GPIO_Mode = GPIO_Mode_Out_PP;//ѡ����������ģʽ
	Led_Value.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;//ѡ�� 1 2 3 �Źܽ�
	Led_Value.GPIO_Speed = GPIO_Speed_50MHz;//ѡ����50MHz������ٶ�
	GPIO_Init(GPIOC, &Led_Value);//�����������ó�ʼ��GPIOC��Ĺܽ�
	
	PCOut(1) = 0;//ʹ��λ��ʹPC1�ܽ�����
	PCOut(2) = 0;//ʹ��λ��ʹPC2�ܽ�����
	PCOut(3) = 0;//ʹ��λ��ʹPC3�ܽ�����
}

void led_on(int nu)//���ݲ���nu������Ӧ��led�Ƶĺ���
{
	switch(nu)
	{
		case 0 : PCOut(1) = 1; break;//��PC1�ܽ�����
		case 1 : PCOut(2) = 1; break;//��PC2�ܽ�����
		case 2 : PCOut(3) = 1; break;//��PC3�ܽ�����
	}
}

void led_off(int nu)//���ݲ���nu�ر���Ӧ��led�Ƶĺ���
{
	switch(nu)
	{
		case 0 : PCOut(1) = 0; break;//��PC1�ܽ�����
		case 1 : PCOut(2) = 0; break;//��PC2�ܽ�����
		case 2 : PCOut(3) = 0; break;//��PC3�ܽ�����
	}
}




