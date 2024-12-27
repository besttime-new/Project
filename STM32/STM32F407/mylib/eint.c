#include "eint.h"
#include "delay.h"
enit_handler T[4];
int flag_status=0;  // ״̬��־λ----ȷ��or����
int flag_timeout=0; // ��ʱ��־λ
int flag_cur_fun=0;  // ��ǰ���ڵĹ��ܱ�־λ
void eint_init(void)
{
	/*
	KEY0    PF9     ��������
	KEY1    PF8     ��������
	KEY2    PF7     ��������
	KEY3    PE6     ��������
	*/
	GPIO_InitTypeDef BUTTON;
	EXTI_InitTypeDef eint;
	NVIC_InitTypeDef nvic;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOE, ENABLE);
	BUTTON.GPIO_Mode = GPIO_Mode_IN;
	BUTTON.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	BUTTON.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOF, &BUTTON);

	BUTTON.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOF, &BUTTON);

	// ʹ��SYSCFGϵͳ���ÿ��ƼĴ�������EXT IT.WKUP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	// ʹ�õ���SYSCFG�Ĵ������ⲿ�ж�Դ�����жϴ����ܽ�
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource7);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource8);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource9);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource6);
	// ��ʼ���ж�
	eint.EXTI_Line = EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9;
	eint.EXTI_LineCmd = ENABLE;
	eint.EXTI_Mode = EXTI_Mode_Interrupt;
	eint.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&eint);

	// ���������ȼ��������ȼ��ı���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// ��ʼ��NVIC
	nvic.NVIC_IRQChannel = EXTI9_5_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 2;
	nvic.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&nvic);
}
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line6) == SET) // ����жϱ�־λ    key0�����л���һ������
	{
		delay_ms(200);  //ʹ�����ȶ�
		flag_cur_fun=(flag_cur_fun+1)%4;
		flag_timeout = 0;
		
		EXTI_ClearITPendingBit(EXTI_Line6); // ����жϱ�־λ
	}
	if (EXTI_GetITStatus(EXTI_Line7) == SET)   // key1�����л���һ������
	{
		delay_ms(200); //ʹ�����ȶ�
		if(flag_cur_fun!=FUN0)
		flag_cur_fun=(flag_cur_fun-1)%4;
		else
		flag_cur_fun=3;
		flag_timeout = 0;
		
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if (EXTI_GetITStatus(EXTI_Line8) == SET)   // key2����ȷ������
	{
		delay_ms(200);   //ʹ�����ȶ�
		flag_status = 1;
		flag_timeout = 0;
		
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if (EXTI_GetITStatus(EXTI_Line9) == SET)   //key3���Ʒ��ع���
	{
		delay_ms(200);    //ʹ�����ȶ�
		flag_status = 0;
		flag_timeout = 0;
		
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
}
void set_eint_init(enit_handler arr[4])
{
	T[0] = arr[0];
	T[1] = arr[1];
	T[2] = arr[2];
	T[3] = arr[3];
}


