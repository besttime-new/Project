#include "beep.h"
#include "bitband.h"
void beep_init(void)
{
	/*
	BEEP  PD7    高电平响   低电平不响
	*/
	GPIO_InitTypeDef BEEP;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	BEEP.GPIO_Mode=GPIO_Mode_OUT;
	BEEP.GPIO_OType=GPIO_OType_PP;
	BEEP.GPIO_Pin=GPIO_Pin_7;
	BEEP.GPIO_PuPd=GPIO_PuPd_NOPULL;
	BEEP.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_Init(GPIOD,&BEEP);
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);
}
void beep_on(void)
{
	GPIO_SetBits(GPIOD,GPIO_Pin_7);
}
void beep_off(void)
{
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);
}


