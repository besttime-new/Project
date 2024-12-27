#include "usart1.h"//����ͷ�ļ�

usart1_handler u1_h;//����һ��ȫ�ֵĺ���ָ��

void usart_1_init(void)//��ʼ���ĺ���(��ʼ�������ӵ�GPIO�ܽźʹ��ڹ����Լ��жϿ�����)
{
	GPIO_InitTypeDef Gpio_Value;//������GPIO�̼��Ľṹ�����
	USART_InitTypeDef Usart_Value;//�����˴��ڹ̼��Ľṹ�����
	NVIC_InitTypeDef Nvic_Value;//�������жϿ������̼��Ľṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	//ͨ��APB2����ʹ��GPIOA���USART1��ʱ��
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_AF_PP;//ѡ��������ĸ���ģʽ
	Gpio_Value.GPIO_Pin = GPIO_Pin_9;//ѡ���˹ܽ�9(PA9�Ƿ��Ͷ�)
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//ѡ����50MHz������ٶ�
	GPIO_Init(GPIOA, &Gpio_Value);//�����������ó�ʼ��GPIOA��Ĺܽ�
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;//ѡ���˸��յ�����ģʽ
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;//ѡ���˹ܽ�10(PA10�ǽ��ն�)
	GPIO_Init(GPIOA, &Gpio_Value);//�����������ó�ʼ��GPIOA��Ĺܽ�
	
	Usart_Value.USART_BaudRate = 115200;//ѡ����115200�Ĳ�����
	Usart_Value.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ѡ��ر���Ӳ������
	Usart_Value.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ѡ���˷��ͺͽ���ģʽ
	Usart_Value.USART_Parity = USART_Parity_No;//ѡ��û����ż����λ
	Usart_Value.USART_StopBits = USART_StopBits_1;//ѡ��1��ֹͣλ
	Usart_Value.USART_WordLength = USART_WordLength_8b;//ѡ��8������λ
	USART_Init(USART1, &Usart_Value);//�����������ó�ʼ��USART1
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���������ȼ��������ȼ�����ռ����
	Nvic_Value.NVIC_IRQChannel = USART1_IRQn;//ѡ���жϺ�
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;//ʹ�ܸ��ж�
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;//���������ȼ�
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;//���������ȼ�
	NVIC_Init(&Nvic_Value);//�����������ó�ʼ��NVIC
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//ʹ����USART1�Ľ��մ����ж�
	
	USART_Cmd(USART1, ENABLE);//ʹ��USART1�Ĺ���
}

void usart_1_send_byte(unsigned char c)//����һ���ֽ�����
{
	USART_SendData(USART1, c);//ͨ��USART1���ͱ���c���洢������
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);//����USART1�ķ��������Ƿ�ɹ�
	USART_ClearFlag(USART1, USART_FLAG_TC);//���USART1�ķ��ͳɹ��ı�־
}

void usart_1_send_data(char *buf)//����һ���ַ�������
{
	while(*buf)//��ѯ�ж��Ƿ������ַ���β���λ��
	{
		usart_1_send_byte(*buf);//���÷���һ���ֽ����ݵĺ����ѻ�ȡ�����ַ�����
		buf++;//ָ��ƫ�Ƶ���һ���ַ�
	}
}

unsigned char usart_1_recv_byte(void)//����һ���ֽ�����(��ѯ�ķ�ʽ)
{
	unsigned char ret = 0;//����ret�������մ��ڽӵ�������
	
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)//�ж�USART1�Ƿ�ӵ�������
	{
		ret = USART_ReceiveData(USART1);//�ѽ��յ������ݴ洢��ret������
		USART_ClearFlag(USART1, USART_FLAG_RXNE);//���USART1�ı�־״̬
	}
	
	return ret;//���ؽ��յ�������
}

void USART1_IRQHandler(void)//�쳣���������й�USART1���жϴ�����
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)//�ж��Ƿ���USART1�Ľ��մ������ж�
	{
		u1_h(USART_ReceiveData(USART1));//��������,���ѽ��յ������ݴ��ݸ��ص�����
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);//���USART1�Ľ����жϱ�־
	}
}

void set_usart1_handler(usart1_handler h)//���ûص�����
{
	u1_h = h;//���β�h���洢�ĵ�ַ���浽ȫ�ֱ���u1_h��
}




