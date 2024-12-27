#include "gpio_iic.h"//����ͷ�ļ�
#include "delay.h"//����ͷ�ļ�
#include "bitband.h"//����ͷ�ļ�

/*
��GPIO�ķ�ʽģ��IIC�������ַ���,��Ӳ�����ӵ�GPIO�й�
�ҵ��豸�ӵ���ͬ��GPIO��,��������Ҫ������Ӧ��GPIO
��Ϊ��������ʵ����OLED EEPROM SHT30,����Щ�豸��������������ͬ�Ĺܽ���
����,��ʹ��ʱ���豸��ַһ����Ҫд��
*/

//���´�����Ӳ����صģ���ֲʱ����Ҫ�޸�
GPIO_InitTypeDef  GPIO_InitForSCL;//������GPIO�̼��Ľṹ�����
GPIO_InitTypeDef  GPIO_InitForSDAOut;//������GPIO�̼��Ľṹ�����
GPIO_InitTypeDef  GPIO_InitForSDAIn;//������GPIO�̼��Ľṹ�����

#define IIC_SCL					PBOut(6)//��PB6��������ɺ�
#define IIC_SDA					PBOut(7)//��PB7��������ɺ�
#define IIC_SDA_STATUS 	PBIn(7)//��PB7���������ɺ�

void IIC_Init(void)//��ʼ��IICʹ�õ�GPIO
{			
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ͨ��APB2����ʹ��GPIOB���ʱ��
	
	//PB6 SCL
  GPIO_InitForSCL.GPIO_Mode = GPIO_Mode_Out_PP;//ѡ������������ģʽ
	GPIO_InitForSCL.GPIO_Pin = GPIO_Pin_6;//ѡ����6��GPIO�ܽ�
	GPIO_InitForSCL.GPIO_Speed = GPIO_Speed_50MHz;//ѡ����50MHz������ٶ�
	
	//PB7 SDA OUT
	GPIO_InitForSDAOut.GPIO_Mode = GPIO_Mode_Out_PP;//ѡ������������ģʽ
  GPIO_InitForSDAOut.GPIO_Pin = GPIO_Pin_7;//ѡ����7��GPIO�ܽ�
	GPIO_InitForSDAOut.GPIO_Speed = GPIO_Speed_50MHz;//ѡ����50MHz������ٶ�
	
  //PB7 SDA IN
  GPIO_InitForSDAIn.GPIO_Mode = GPIO_Mode_IPU;//ѡ��������������ģʽ
  GPIO_InitForSDAIn.GPIO_Pin = GPIO_Pin_7;//ѡ����7��GPIO�ܽ�
	GPIO_InitForSDAIn.GPIO_Speed = GPIO_Speed_50MHz;//ѡ����50MHz������ٶ�
	
  GPIO_Init(GPIOB, &GPIO_InitForSDAOut);//�����������ó�ʼ��GPIOB��Ĺܽ�
  GPIO_Init(GPIOB, &GPIO_InitForSCL);//�����������ó�ʼ��GPIOB��Ĺܽ�
	
	IIC_SCL = 1;//��ʱ��������
	IIC_SDA = 1;//������������
}

void IIC_Sda_In(void)//����������ʹ�õ�GPIO�ܽ�����Ϊ����ģʽ
{
	GPIO_Init(GPIOB, &GPIO_InitForSDAIn);//�����������ó�ʼ��GPIOB��Ĺܽ�
}

void IIC_Sda_Out(void)//����������ʹ�õ�GPIO�ܽ�����Ϊ���ģʽ
{
	GPIO_Init(GPIOB, &GPIO_InitForSDAOut);//�����������ó�ʼ��GPIOB��Ĺܽ�
}
//���ϴ�����Ӳ����صģ���ֲʱ����Ҫ�޸�

//���´���Ӳ���޹أ�ֻ��iicЭ���й�
void IIC_Start(void)//������ʼ�ź�
{
	IIC_Sda_Out();//����������ʹ�õ�GPIO�ܽ�����Ϊ���ģʽ
	IIC_SDA = 1;//����������
	IIC_SCL = 1;//����ʱ����
	delay_us(2);//��ʱ2΢��
 	IIC_SDA = 0;//����������
	delay_us(2);//��ʱ2΢��
	IIC_SCL = 0;//����ʱ����
}	  

void IIC_Stop(void)//���������ź�
{
	IIC_Sda_Out();//����������ʹ�õ�GPIO�ܽ�����Ϊ���ģʽ
	IIC_SCL = 0;//����ʱ����
	IIC_SDA = 0;//����������
 	delay_us(2);//��ʱ2΢��
	IIC_SCL = 1;//����ʱ����
	delay_us(1);//��ʱ1΢��
	IIC_SDA = 1;//����������
	delay_us(2);//��ʱ2΢��
}

u8 IIC_Wait_Ack(void)//�ȴ�ACK
{
	u8 ucErrTime = 0;//��ʱ����ļ�������
	
	IIC_Sda_In();//����������ʹ�õ�GPIO�ܽ�����Ϊ����ģʽ
	IIC_SCL = 0;//����ʱ����
	delay_us(1);//��ʱ1΢��
	IIC_SCL = 1;//����ʱ����
	delay_us(1);//��ʱ1΢��
	while(IIC_SDA_STATUS)//�ж��������Ǹߵ�ƽ���ǵ͵�ƽ
	{
		ucErrTime++;//������������
		if(ucErrTime > 250)//�жϼ���������ֵ�Ƿ񳬹��ض���ֵ
		{
			IIC_Stop();//���������ź�
			return 1;//������ֵ1
		}
	}
	IIC_SCL = 0;//����ʱ����
	return 0;//������ֵ0
} 

void IIC_Ack(void)//����ACK
{
	IIC_SCL = 0;//����ʱ����
	IIC_Sda_Out();//����������ʹ�õ�GPIO�ܽ�����Ϊ���ģʽ
	IIC_SDA = 0;//����������
	delay_us(2);//��ʱ2΢��
	IIC_SCL = 1;//����ʱ����
	delay_us(2);//��ʱ2΢��
	IIC_SCL = 0;//����ʱ����
}
    
void IIC_NAck(void)//������ACK
{
	IIC_SCL = 0;//����ʱ����
	IIC_Sda_Out();//����������ʹ�õ�GPIO�ܽ�����Ϊ���ģʽ
	IIC_SDA = 1;//����������
	delay_us(2);//��ʱ2΢��
	IIC_SCL = 1;//����ʱ����
	delay_us(2);//��ʱ2΢��
	IIC_SCL = 0;//����ʱ����
}					 				     

void IIC_Send_Byte(u8 txd)//����IIC���߷�������(һλһλ�ķ���)
{                        
  u8 t;//ѭ������
	
	IIC_Sda_Out();//����������ʹ�õ�GPIO�ܽ�����Ϊ���ģʽ
  for (t = 0;t < 8; t++)//ѭ��8��
	{  
		IIC_SCL = 0;//����ʱ����
		delay_us(1);//��ʱ1΢��
    IIC_SDA = (txd & 0x80) >> 7;//������txd����һλһλ�ķ�ʽ����(�ȷ��͸�λ)
    txd <<= 1;//txd��λ���Ƶ���1
		delay_us(1);//��ʱ1΢��
		IIC_SCL = 1;//����ʱ����
		delay_us(2);//��ʱ2΢��
  }
	IIC_SCL = 0;//����ʱ����
} 	    

u8 IIC_Recv_Byte(unsigned char ack)//����IIC���߽�������
{
	unsigned char i = 0;//ѭ������
	unsigned char receive = 0;//������յ���IIC������
	
	IIC_Sda_In();//����������ʹ�õ�GPIO�ܽ�����Ϊ����ģʽ
  for(i = 0; i < 8; i++)
	{
    IIC_SCL = 0;//����ʱ����
    delay_us(2);//��ʱ2΢��
		IIC_SCL = 1;//����ʱ����
		delay_us(2);//��ʱ2΢��
    receive <<= 1;//receive�������Ƶ���1
    if(IIC_SDA_STATUS)//�ж��������Ǹߵ�ƽ���ǵ͵�ƽ
			receive |= 1;//receive��λ�����1
  }					 
	IIC_SCL = 0;//����ʱ����
  if (!ack)//�ж��β�ackȡ���Ƿ�Ϊ��
		IIC_NAck();//������ACK
  else
		IIC_Ack();//����ACK
	
  return receive;//�ѻ�ȡ����һ���ֽ����ݷ���
}
