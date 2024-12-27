/****************************************************************
���̽���	:	��Ҫ��ͨ���������ϵ�NRF����ͨ��ģ�����һ��һ�յ�ʵ��
						���ͷ����԰��Լ��ɼ������¶�ʪ��ADת���Ľ�����з���
ʹ�÷���	:	ͨ��GPIO�ķ�ʽģ��SPIЭ��Ӷ�����NRF����ͨ��ģ��
��������	:	���԰��Լ��ɼ����ĵ��¶� ʪ�� ADת����ֵ��ʾ���Լ���
						OLED��ʾ����,���ҿ��԰���Щ�������ϳ����ݰ�ͨ��NRF
						���͵����շ�
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
	char Dht_Data[5] = {0};//�洢DHT11������
	double Sht_Data[2] = {0};//�洢SHT30������
	char Sht_Buf[12] = {0};//�洢ת��SHT30֮����ַ���
	int Adj_Res_Value = 0;//�洢ADת��������
	u8 ret = 0;//���շ���ֵ
	struct time_st t = {2019, 9, 17, 2, 22, 37, 0};//�洢��ǰ��Ҫ���õ�ʱ��
	int count = 0;//��������
	struct nrf_msg_st msg;//�Զ���ṹ������<32byte>
	//u8 rxAddr[] = {0x10, 0x20, 0x30, 0x40, 0x51};//���ܵ�ַ
	u8 txAddr[] = {0x10, 0x20, 0x30, 0x40, 0x53};//���͵�ַ
	
	led_init();//���ó�ʼ��led�Ƶĺ���
	buzzer_init();//���ó�ʼ���������ĺ���
	button_init();//���ó�ʼ�����ܰ����ĺ���
	delay_init();//���ó�ʼ����ʱ�ĺ���
	eint_init();//���ó�ʼ�������жϵĺ���
	usart_1_init();//���ó�ʼ��USART1�ĺ���
	dht_init();//���ó�ʼ��DHT11�ĺ���
	eeprom_init();//���ó�ʼ��eeprom�ĺ���
	sht_init();//���ó�ʼ��SHT30�ĺ���
	OLED_Init();//���ó�ʼ��OLED�ĺ���
	ldt_init();//���ó�ʼ������ܵĺ���
	adj_res_init();//���ó�ʼ���߾��ȿɵ�����ĺ���
	rtc_init(&t);//���ó�ʼ��RTC�ĺ���
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
	
	sht_write_mode();//����sht30�Ķ�ȡ����
	
	NRF_TX_Mode(txAddr, 7);//����Ϊ����ģʽ

	delay_ms(2000);//��ʱ2s
	OLED_Clear();//�����ĺ���
	
	while(1)
	{
		get_dht_value(Dht_Data);//��ȡDHT11����
		sht_write_read_cmd();//д���SHT30���ݵ�����
		sht_read_data(Sht_Data);//��SHT30�ɼ���������
		Adj_Res_Value = get_adj_res_value();//��ȡADת���Ľ��
		msg.Dht_Hum = (int)Dht_Data[0];//DHT11��ʪ��ת�浽NRF���ݰ��Ľṹ����
		msg.Dht_Temp = (int)Dht_Data[2];//DHT11���¶�ת�浽NRF���ݰ��Ľṹ����
		msg.Sht_Temp = (float)Sht_Data[0];//SHT30���¶�ת�浽NRF���ݰ��Ľṹ����
		msg.Sht_Hum = (float)Sht_Data[1];//SHT30��ʪ��ת�浽NRF���ݰ��Ľṹ����
		sprintf((char *)msg.buf, "%d", Adj_Res_Value);//ADת�������ݴ洢��NRF���ݰ��Ľṹ����
		OLED_ShowString(0, 2, (u8 *)msg.buf, 12);//��ʾ�Լ��ɼ�����ADֵ
		OLED_ShowNum(0, 4, msg.Dht_Hum, 2, 12);//��ʾ�Լ��ɼ�����DHT11��ʪ��
		OLED_ShowNum(30, 4, msg.Dht_Temp, 2, 12);//��ʾ�Լ��ɼ�����DHT11���¶�
		sprintf(Sht_Buf, "%.2f %.2f", msg.Sht_Temp, msg.Sht_Hum);//ת��SHT30�ɼ���������
		OLED_ShowString(0, 6, (u8 *)Sht_Buf, 12);//��ʾSHT30�ɼ����ĵ�����
		count = 0;
		send_again:
		delay_ms(20);//��ʱ20ms
		ret = NRF_Tx_Dat((u8 *)&msg, sizeof(msg));//��������
		if(ret == TX_DS)//�ж��Ƿ��ͳɹ�
		{
			OLED_ShowString(0, 0, (u8 *)"tx bingo", 12);//��ӡtx bingo��ʾ��Ϣ
		} 
		else 
		{
			OLED_ShowString(0, 0, (u8 *)"tx error", 12);//��ӡtx error��ʾ��Ϣ
			count++;
			if(count < 5)//���5��û�з��ͳɹ�
			{
				goto send_again;//��ת��send_again
			}
		}
		led_on(0);//0��led����
		delay_ms(500);//��ʱ1s
		led_off(0);//0��led����
		delay_ms(500);//��ʱ1s
	}
}





