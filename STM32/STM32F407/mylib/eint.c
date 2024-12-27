#include "eint.h"
#include "delay.h"
enit_handler T[4];
int flag_status=0;  // 状态标志位----确定or返回
int flag_timeout=0; // 超时标志位
int flag_cur_fun=0;  // 当前所在的功能标志位
void eint_init(void)
{
	/*
	KEY0    PF9     上拉输入
	KEY1    PF8     上拉输入
	KEY2    PF7     上拉输入
	KEY3    PE6     上拉输入
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

	// 使能SYSCFG系统配置控制寄存器，及EXT IT.WKUP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	// 使用的是SYSCFG寄存器给外部中断源配置中断触发管脚
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource7);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource8);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource9);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource6);
	// 初始化中断
	eint.EXTI_Line = EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9;
	eint.EXTI_LineCmd = ENABLE;
	eint.EXTI_Mode = EXTI_Mode_Interrupt;
	eint.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&eint);

	// 配置组优先级和子优先级的比例
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// 初始化NVIC
	nvic.NVIC_IRQChannel = EXTI9_5_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 2;
	nvic.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&nvic);
}
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line6) == SET) // 检查中断标志位    key0控制切换下一个功能
	{
		delay_ms(200);  //使按键稳定
		flag_cur_fun=(flag_cur_fun+1)%4;
		flag_timeout = 0;
		
		EXTI_ClearITPendingBit(EXTI_Line6); // 清除中断标志位
	}
	if (EXTI_GetITStatus(EXTI_Line7) == SET)   // key1控制切换上一个功能
	{
		delay_ms(200); //使按键稳定
		if(flag_cur_fun!=FUN0)
		flag_cur_fun=(flag_cur_fun-1)%4;
		else
		flag_cur_fun=3;
		flag_timeout = 0;
		
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if (EXTI_GetITStatus(EXTI_Line8) == SET)   // key2控制确定功能
	{
		delay_ms(200);   //使按键稳定
		flag_status = 1;
		flag_timeout = 0;
		
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if (EXTI_GetITStatus(EXTI_Line9) == SET)   //key3控制返回功能
	{
		delay_ms(200);    //使按键稳定
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


