#include "button.h"//����ͷ�ļ�
#include "bitband.h"//�����й�λ����ͷ�ļ�

void button_init(void)//��ʼ���ĺ���(��ʼ�������ӵ�GPIO�ܽ�)
{
	GPIO_InitTypeDef Button_Value;//������GPIO�̼��Ľṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//ͨ��APB2ʹ��GPIOC���ʱ��
	
	Button_Value.GPIO_Mode = GPIO_Mode_IPU;//ѡ�������������ģʽ
	Button_Value.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;//ѡ���� 8 9 �Źܽ�
	Button_Value.GPIO_Speed = GPIO_Speed_50MHz;//ѡ����50MHz������ٶ�
	GPIO_Init(GPIOC, &Button_Value);//�������������ó�ʼ��GPIOC��Ĺܽ�
}

int button_status(int nu)//���ݲ���nu�����Ӧ�Ĺ��ܰ����Ƿ񱻰���
{
	int ret = 0;//������ջ�ȡֵ�ı���
	
	switch(nu)
	{
		case 0 : ret = PCIn(9); break;//��ȡGPIOC���9�Źܽŵĵ�ƽ�����浽ret������
		case 1 : ret = PCIn(8); break;//��ȡGPIOC���8�Źܽŵĵ�ƽ�����浽ret������
	}
	return !ret;//�ѻ�ȡ����ֵ���߼��ǵĲ���������
}






