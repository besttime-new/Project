#include "eint.h"//����ͷ�ļ�

eint_handler e_h0, e_h1;//��������ȫ�ֵĺ���ָ��

void eint_init(void)//��ʼ���ĺ���(��ʼ�������ӵ�GPIO�ܽź��жϿ������Լ��ⲿ�ж�)
{
	GPIO_InitTypeDef Gpio_Value;//������GPIO�̼��Ľṹ�����
	NVIC_InitTypeDef Nvic_Value;//�������жϿ������̼��Ľṹ�����
	EXTI_InitTypeDef Exti_Value;//�������ⲿ�жϹ̼��Ľṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	//ͨ��APB2����ʹ��GPIOC��ĺ�AFIO��ʱ��
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IPU;//ѡ����������ģʽ
	Gpio_Value.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;//ѡ��ܽ�9 8
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//ѡ��50MHz����ٶ�
	GPIO_Init(GPIOC, &Gpio_Value);//�����������ó�ʼ��GPIOC��Ĺܽ�
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);
	//Ϊ�ⲿ�ж�9ѡ����PC9���� Ϊ�ⲿ�ж�8ѡ����PC8����
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���������ȼ��������ȼ�����ռ����
	Nvic_Value.NVIC_IRQChannel = EXTI9_5_IRQn;//ѡ���жϺ�
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;//ʹ�ܸ��ж�
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;//���������ȼ�
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;//���������ȼ�
	NVIC_Init(&Nvic_Value);//�����������ó�ʼ��NVIC
	
	Exti_Value.EXTI_Line = EXTI_Line9 | EXTI_Line8;//ѡ���ⲿ�жϺ�9 8
	Exti_Value.EXTI_LineCmd = ENABLE;//ʹ�ܸ��ⲿ�ж�
	Exti_Value.EXTI_Mode = EXTI_Mode_Interrupt;//ѡ���жϵ�ģʽ
	Exti_Value.EXTI_Trigger = EXTI_Trigger_Falling;//ѡ���½��ش����ж�
	EXTI_Init(&Exti_Value);//��������ƥ���ʼ��EXTI
}

void EXTI9_5_IRQHandler(void)//�쳣���������й��ⲿ�ж�9_5���жϴ�����
{
	if(EXTI_GetITStatus(EXTI_Line9) == SET)//�ж��Ƿ������ⲿ�ж�9�������ж�
	{
		e_h0();//���ð�������֮��Ҫ���Ĵ���
		EXTI_ClearITPendingBit(EXTI_Line9);//����ⲿ�ж�9�жϱ�־λ
	}
	if(EXTI_GetITStatus(EXTI_Line8) == SET)//�ж��Ƿ������ⲿ�ж�8�������ж�
	{
		e_h1();//���ð�������֮��Ҫ���Ĵ���
		EXTI_ClearITPendingBit(EXTI_Line8);//����ⲿ�ж�8�жϱ�־λ
	}
}

void set_eint_handler(eint_handler h0, eint_handler h1)//���ûص�����
{
	e_h0 = h0;//���β�h0���洢�ĵ�ַ���浽ȫ�ֱ���e_h0��
	e_h1 = h1;//���β�h1���洢�ĵ�ַ���浽ȫ�ֱ���e_h1��
}






