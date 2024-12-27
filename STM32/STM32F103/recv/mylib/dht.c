#include "dht.h"//����ͷ�ļ�
#include "bitband.h"//����ͷ�ļ�
#include "delay.h"//����ͷ�ļ�

void set_dht_gpio_output(void)//��DHT11�����ӵ�GPIO�ܽ����ó����ģʽ
{
	GPIO_InitTypeDef Gpio_Value;//������GPIO�̼��Ľṹ�����
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;//ѡ������������ģʽ
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;//ѡ����10��GPIO�ܽ�
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//ѡ����50MHz������ٶ�
	GPIO_Init(GPIOC, &Gpio_Value);//�����������ó�ʼ��GPIOC��Ĺܽ�
}

void set_dht_gpio_input(void)//��DHT11�����ӵ�GPIO�ܽ����ó�����ģʽ
{
	GPIO_InitTypeDef Gpio_Value;//������GPIO�̼��Ľṹ�����
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;//ѡ���˸��յ�����ģʽ
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;//ѡ����10��GPIO�ܽ�
	GPIO_Init(GPIOC, &Gpio_Value);//�����������ó�ʼ��GPIOC��Ĺܽ�
}

void dht_output_state(int status)//ͨ������status�������ӵ�GPIO�ܽ�����ߵ�ƽ��������͵�ƽ
{
	set_dht_gpio_output();//��DHT11�����ӵ�GPIO�ܽ����ó����ģʽ
	if(status == 0)//�жϲ���status�Ƿ�Ϊ0
		PCOut(10) = 0;//ͨ��λ����PC10�ܽ�����͵�ƽ
	else
		PCOut(10) = 1;//ͨ��λ����PC10�ܽ�����ߵ�ƽ
}

int dht_input_state(void)//���DHT11�����ӵ�GPIO�ܽŵĸߵ͵�ƽ
{
	set_dht_gpio_input();//��DHT11�����ӵ�GPIO�ܽ����ó�����ģʽ
	return PCIn(10);//ͨ��λ����PC10�ܽŲɼ����ĸߵ͵�ƽ����
}

void dht_init(void)//��ʼ���ĺ���(ʹ�������ӵ�GPIO�ܽŵ�ʱ��)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//ͨ��APB2����ʹ��GPIOC���ʱ��
}

void get_dht_value(char *buf)//��ȡDHT11�ɼ���������
{
	char flag = 0;//��ȡ״̬�ı���
	char time = 0;//����ʱ��������ı���
	char i = 0;//ѭ������
	char tmp = 0;//��ȡ�ɼ�����ÿһλ����
	
	dht_output_state(1);//������������
	dht_output_state(0);//������������
	delay_ms(20);				//��ʱ20����
	dht_output_state(1);//������������
	/*�������ȴ������߱�DHT11����(���� ��ʱ����)*/
	//while(dht_input_state());//����
	do//��ʱ����
	{
		flag = dht_input_state();//��ȡDHT11�����ӵ�GPIO�ܽŵĸߵ͵�ƽ
		delay_us(2);//��ʱ2΢��
		time++;//time����
	}while(flag == 1 && time <= 20);//�ж�flag�Ƿ���1����timeС�ڵ���20
	if(time > 20)//���time����20,��ʱ
		return ;//�������ܺ���
	
	while(!dht_input_state());//�ȴ�����������
	
	for(i = 0; i < 40; i++)//һ������������40λ=8λʪ������+8λʪ��С��+8λ�¶�����+8λ�¶�С��+8λУ��λ
	{
		while(dht_input_state());//�ȴ�����������
		while(!dht_input_state());//�ȴ�����������
		delay_us(40);//��ʱ40΢��
		tmp <<= 1;//��λ����1λ
		if(dht_input_state())//�жϼ�⵽�ĵ��ź��Ƿ�Ϊ1
			tmp |= 1;//tmp��λ����1
		if((i + 1) % 8 == 0)//��i = 7 15 23 31 39����if�Ѷ�����8λ���ݱ��浽buf��
		{
			buf[i / 8] = tmp;//��tmp���뵽buf��
			tmp = 0;//tmp����
		}
	}
	dht_output_state(1);//������������
}







