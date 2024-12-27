#include "iwdg.h"//����ͷ�ļ�

void iwdg_init(int ms)//��ʼ���ĺ���(��������Ԥ��Ƶ���ͼ���ֵ)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//����д����(�ر�д����)
	IWDG_SetPrescaler(IWDG_Prescaler_64);//64��Ƶ 40KHz / 64 = 625Hz
	IWDG_SetReload(ms);//��������ֵ
	IWDG_ReloadCounter();//ι��
	IWDG_Enable();//�������Ź�
}

void iwdg_feed_dog(void)//ι���Ĺ��ܺ���
{
	IWDG_ReloadCounter();//ι��
}






