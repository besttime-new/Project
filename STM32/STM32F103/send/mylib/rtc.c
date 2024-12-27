#include "rtc.h"//����ͷ�ļ�
#include "string.h"//����ͷ�ļ�

const uint8_t monthTable[12] = {31,28,31,30,31,30,31,31,30,31,30,31};//�洢ÿ����(ƽ��)�ж���������ת��ʱ��
static struct time_st T;//����������ʱ��ṹ�����͵�ȫ�ֱ���

static uint8_t isLeapYear(uint16_t y)//�ж��Ƿ�������
{
	return (((!(y % 4)) && (y % 100)) || (!(y % 400)));
}

void RTC_IRQHandler(void)//�쳣���������й�RTC���жϵ��жϴ�����
{
	if(RTC_GetITStatus(RTC_IT_SEC) == SET)//�ж��Ƿ���RTC�����жϴ����ĸ��жϴ�����
	{
		conv_time(&T);//ת��ʱ�䲢���浽ȫ�ֱ���T��
		RTC_ClearITPendingBit(RTC_IT_SEC);//���RTC���жϵ��жϱ�־λ
	}
}

void NVIC_Configuration(void)//�����й�RTC���жϿ�������������Ϣ
{
	NVIC_InitTypeDef Nvic_Value;//�������жϿ������̼��Ľṹ�����
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���������ȼ��������ȼ�����ռ����
	Nvic_Value.NVIC_IRQChannel = RTC_IRQn;//ѡ���жϺ�
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;//ʹ�ܸ��ж�
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;//���������ȼ�
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;//���������ȼ�
	NVIC_Init(&Nvic_Value);//�����������ó�ʼ��NVIC
}

void RTC_Configuration(void)//����RTC�ĳ�ʼ��(�����һ��ʹ��RTC��Ҫ���øú�����������)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR, ENABLE);//ͨ��APB1ʹ��BKP��PWR��ʱ��
	PWR_BackupAccessCmd(ENABLE);//ͨ��PWRʹ�ܺ󱸴洢�����д����(�ر�д����)
	BKP_DeInit();//��λBKP
	RCC_LSEConfig(RCC_LSE_ON);//����LSEʱ��Դ
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);//�ȴ�LSE�ȶ�
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//ΪRTCѡ����LSE�ṩʱ��Դ
	RCC_RTCCLKCmd(ENABLE);//ʹ��RTC��ʱ��
	RTC_WaitForSynchro();//�ȴ�RTCʱ��ͬ��
	RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	RTC_ITConfig(RTC_IT_SEC, ENABLE);//ʹ��RTC�����ж�
	RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	RTC_EnterConfigMode();//����RTC������ģʽ
	RTC_SetPrescaler(32767);//����RTC��Ԥ��Ƶ
	RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	RTC_ExitConfigMode();//�˳�RTC������ģʽ
}

void rtc_init(struct time_st *t)//��ʼ��RTC
{
	NVIC_Configuration();//�����жϿ�����
	if(BKP_ReadBackupRegister(BKP_DR1) != 0x9527)//�ж�BKP�ļĴ������Ƿ�������ض���ֵ(�ж��Ƿ��ǵ�һ�β���RTC)
	{
		RTC_Configuration();//����RTC
		RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_EnterConfigMode();//����RTC������ģʽ
		set_time(t);//����ʱ��
		RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_ExitConfigMode();//�˳�RTC������ģʽ
		BKP_WriteBackupRegister(BKP_DR1, 0x9527);//ΪBKP�ļĴ�����д���ض���ֵ
	}
	else
	{
		RTC_WaitForSynchro();//�ȴ�RTCʱ��ͬ��
		RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_ITConfig(RTC_IT_SEC, ENABLE);//ʹ��RTC�����ж�
		RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	}
}

void set_time(struct time_st* t)//����ʱ��(�Ӳ���t�д洢��ʱ�䵽1970/1/1 00:00:00�������洢����������)
{
	uint32_t secCount = 0;//������ֵ
	uint16_t i;//ѭ������

	if(t->Y < 1970 || t->Y > 2099)//�ж�����Ƿ񳬳��˼��㷶Χ
		return ;
	
	for(i = 1970; i < t->Y; i++)//����t->Y֮ǰ����ݵ�������
	{
		if(isLeapYear(i))//�ж��Ƿ�������
			secCount += 31622400;
		else
			secCount += 31536000;
	}

	for (i = 0; i < t->M - 1; i++)//����t->M֮ǰ���·ݵ�������
	{
		secCount += ((uint32_t)monthTable[i] * 86400);
		if (isLeapYear(t->Y) && i == 1)//��������겢���Ƕ��·ݶ��һ�������
			secCount += 86400;
	}
	secCount += (uint32_t)(t->D - 1) * 86400;//����t->D֮ǰ��������������
	secCount += (uint32_t)(t->h) * 3600;//����t->h֮ǰ��Сʱ��������
	secCount += (uint32_t)(t->m) * 60;//����t->m֮ǰ�ķ��ӵ�������
	secCount += (uint32_t)(t->s);//����t->s������

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE);//ͨ��APB1ʹ��PWR��BKP��ʱ��
	PWR_BackupAccessCmd(ENABLE);//ͨ��PWRʹ�ܺ󱸴洢�����д����(�ر�д����)
	RTC_SetCounter(secCount);//�Ѽ���������ʹ洢����������
	RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������
}

void conv_time(struct time_st* t)//ת��ʱ��(�Ѽ������д洢�������ͼ��������������ʱ����)
{
	uint32_t timeCount = 0;//�洢�Ӽ������л�ȡ��������
	uint32_t Count = 0;//�м����
	uint16_t tmp = 0;//�м����
	
	timeCount = RTC_GetCounter();//�Ӽ������л�ȡ������
	Count = timeCount / 86400;//�����ж�����
	t->W = (4 + Count) % 7;//��������
	if(Count != 0)//�������
	{
		tmp = 1970;
		while(Count >= 365)
		{
			if(isLeapYear(tmp))
			{
				if(Count>= 366)
				{
					Count -= 366;
				}
				else
				{
					tmp++;
					break;
				}
			}
			else
			{
					Count -= 365;
			}
			tmp++;
		}
		t->Y = tmp;
		tmp = 0;
		while(Count >= 28)//�����·�
		{
			if(isLeapYear(t->Y) && tmp == 1)
			{
				if(Count >= 29) 
						Count -= 29;
				else    
						break;
			}
			else
			{
				if(Count >= monthTable[tmp]) 
					Count -= monthTable[tmp];
				else
					break;
			}
			tmp++;
		}
		t->M = tmp + 1;
		t->D = Count + 1;
    }
    Count = timeCount % 86400;
    t->h = Count / 3600;//����Сʱ��
    t->m = (Count % 3600) / 60;//���������
    t->s = (Count % 3600) % 60;//��������
}

void get_time(struct time_st *t)//��ȡʱ��
{
	memcpy(t, &T, sizeof(T));//��T�л�ȡ����ʱ�俽��������t��
}













