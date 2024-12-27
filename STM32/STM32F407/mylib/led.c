#include "led.h"
void led_init(void)
{
	/*
	LED1    PG9      低电平亮     高电平灭
	LED2    PE4      低电平亮     高电平灭
	LED3    PE3      低电平亮     高电平灭
	*/
	GPIO_InitTypeDef LED;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG,ENABLE);
	LED.GPIO_Mode=GPIO_Mode_OUT;
	LED.GPIO_OType=GPIO_OType_PP;
	LED.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4;
	LED.GPIO_PuPd=GPIO_PuPd_NOPULL;
	LED.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_Init(GPIOE,&LED);
	
	LED.GPIO_Pin=GPIO_Pin_9;
	GPIO_Init(GPIOG,&LED);
	GPIO_SetBits(GPIOG,GPIO_Pin_9);
	GPIO_SetBits(GPIOE,GPIO_Pin_4);
	GPIO_SetBits(GPIOE,GPIO_Pin_3);
}
void led_on(int nu)
{
	switch(nu)
	{
		case 0:GPIO_ResetBits(GPIOG,GPIO_Pin_9);break;
		case 1:GPIO_ResetBits(GPIOE,GPIO_Pin_4);break;
		case 2:GPIO_ResetBits(GPIOE,GPIO_Pin_3);break;
	}
}
void led_off(int nu)
{
	switch(nu)
	{
		case 0:GPIO_SetBits(GPIOG,GPIO_Pin_9);break;
		case 1:GPIO_SetBits(GPIOE,GPIO_Pin_4);break;
		case 2:GPIO_SetBits(GPIOE,GPIO_Pin_3);break;
	}
		
}
