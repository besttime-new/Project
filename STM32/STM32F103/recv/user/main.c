/****************************************************************
���̽���	:	��Ҫ��ͨ���������ϵ�NRF����ͨ��ģ�����һ��һ�յ�ʵ��
						���շ����շ��ͷ�������������,��������ʾ
ʹ�÷���	:	ͨ��GPIO�ķ�ʽģ��SPIЭ��Ӷ�����NRF����ͨ��ģ��
��������	:	���԰ѽ��յ���NRF���ͷŷ��͹���������ͨ��OLED������ʾ
****************************************************************/
#include "led.h"//�����й�led�Ʋ�����ͷ�ļ�
#include "buzzer.h"//�����йط�����������ͷ�ļ�
#include "button.h"//�����йع��ܰ���������ͷ�ļ�
#include "delay.h"//�����й���ʱ������ͷ�ļ�
#include "eint.h"//�����йذ����жϲ�����ͷ�ļ�
#include "iwdg.h"//�����йض������Ź�������ͷ�ļ�
#include "usart1.h"//�����йش���1������ͷ�ļ�
#include "stdio.h"//�����йر�׼�������������ͷ�ļ�
#include "dht.h"//�����й�DHT11������������ͷ�ļ�
#include "eeprom.h"//�����й�EEPROM������ͷ�ļ�
#include "sht.h"//�����й�SHT30������ͷ�ļ�
#include "oled.h"//�����й�OLED������ͷ�ļ�
#include "ldt.h"//�����й�����ܲ�����ͷ�ļ�
#include "adj_res.h"//�����йظ߾��ȿɵ����������ͷ�ļ�
#include "rtc.h"//�����й�RTC������ͷ�ļ�
#include "nrf24l01.h"//�����й�NRF������ͷ�ļ�

int main(void)
{
	u8 ret;//���շ���ֵ
	struct nrf_msg_st msg;//�Զ���ṹ������<32byte>
	char Sht_Data[12] = {0};//�洢SHT30ת�����ַ���������
	//u8 rxAddr[] = {0x10, 0x20, 0x30, 0x40, 0x53};//���ܵ�ַ
	//u8 txAddr[] = {0x10, 0x20, 0x30, 0x40, 0x51};//���͵�ַ
		//???????
	u8 rxAddr[5][5]={{0x11, 0x11, 0x11, 0x22, 0x22},  //??0???
	                 {0x12, 0x20, 0x30, 0x40, 0x50}, //??1???
	                 {0x13, 0x20, 0x30, 0x40, 0x50},
									 {0x14, 0x20, 0x30, 0x40, 0x50},
									 {0x15, 0x20, 0x30, 0x40, 0x50}};
	u8 rxAddr2=0x10;  //??3???
	u8 rxAddr3=0x20;  //??3???
	u8 rxAddr4=0x30;  //??3???
	u8 recvLen[5]={32,32,32,32,32}; //???????????
	led_init();//���ó�ʼ��led�Ƶĺ���
	delay_init();//���ó�ʼ����ʱ�ĺ���
	OLED_Init();//���ó�ʼ��OLED�ĺ���
	SPI_NRF_Init();//��ʼ��NRF
	
	OLED_Clear();//�����ĺ���
	
	ret = NRF_Check();//���NRF�Ƿ����
	if (ret == SUCCESS)//���ret����SUCCESS
	{
		OLED_ShowString(0, 0, (u8 *)"spi ok", 12);//��ʾspi ok����Ϣ
		OLED_ShowString(0, 2, (u8 *)"R:10,20,30,40,53", 12);//��ʾ���յ�ַ
		OLED_ShowString(0, 4, (u8 *)"T:10,20,30,40,51", 12);//��ʾ���͵�ַ
	} 
	else 
	{
		OLED_ShowString(0, 0, (u8 *)"spi error", 12);//��ʾspi error����Ϣ
	}
	
	NRF_RX_Mode(rxAddr,1, recvLen,0);//����Ϊ����ģʽ
	NRF_RX_Mode(rxAddr,1, recvLen,1);//����Ϊ����ģʽ
	NRF_RX_Mode(rxAddr,1, recvLen,2);//����Ϊ����ģʽ
	NRF_RX_Mode(rxAddr,1, recvLen,3);//����Ϊ����ģʽ
	NRF_RX_Mode(rxAddr,1, recvLen,4);//����Ϊ����ģʽ
	delay_ms(2000);//��ʱ2s
	OLED_Clear();//�����ĺ���
	
	while(1)
	{
		ret = NRF_Rx_Dat((u8 *)&msg, sizeof(msg));//��������
		led_on(0);//0��LED����
		if(ret == RX_DR)//�ж��Ƿ�ӵ�����
		{
			OLED_ShowString(0, 0, (u8 *)"rx bingo", 12);//��ʾrx bingo��ʾ��Ϣ
			OLED_ShowString(0, 2, (u8 *)msg.buf, 12);//��ʾ���յ�������
			OLED_ShowNum(0, 4, msg.Dht_Hum, 2, 12);//��ʾ���յ�������
			OLED_ShowNum(30, 4, msg.Dht_Temp, 2, 12);//��ʾ���յ�������
			sprintf(Sht_Data, "%.2f %.2f", msg.Sht_Temp, msg.Sht_Hum);//ת�����յ�������
			OLED_ShowString(0, 6, (u8 *)Sht_Data, 12);//��ʾ���յ�������
		} 
		else
		{
			OLED_ShowString(0, 0, (u8 *)"rx error", 12);//��ʾrx error��ʾ��Ϣ
		}
	}
}





