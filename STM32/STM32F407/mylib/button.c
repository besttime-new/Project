#include "button.h"
#include "bitband.h"
void button_init(void)
{
	
	/*
	KEY0    PF9     上拉输入
	KEY1    PF8     上拉输入
	KEY2    PF7     上拉输入
	KEY3    PE6     上拉输入
	*/
	GPIO_InitTypeDef BUTTON;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOE,ENABLE);
	BUTTON.GPIO_Mode=GPIO_Mode_IN;
	BUTTON.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	BUTTON.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOF,&BUTTON);
	
	BUTTON.GPIO_Pin=GPIO_Pin_6;
	GPIO_Init(GPIOF,&BUTTON);
}
int button_scan(int nu)
{
	int ret=0;
	switch(nu)
	{
		case 0:ret=PFIn(9);break;
		case 1:ret=PFIn(8);break;
		case 2:ret=PFIn(7);break;
		case 3:ret=PEIn(6);break;
	}
	return !ret;
}

