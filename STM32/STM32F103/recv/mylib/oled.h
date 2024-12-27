#ifndef __OLED_H
#define __OLED_H
/*
��GPIO�ķ�ʽ��ģ��IICЭ��
���������ŵ�����ֲ������õ�
ͨ��Ӳ��ԭ��ͼ��֪
ʱ����	IIC_SCL		PB6
������	IIC_SDA		PB7
oled.c �� oled.h �������������������й�OLED��ʵ��
*/
#include "stdlib.h"//�����˱�׼���ͷ�ļ�

typedef unsigned char u8;//ͨ��typedef����u8����
typedef unsigned int u32;//ͨ��typedef����u32����
     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


//OLED�����ú���
extern void OLED_WR_Byte(u8 data, u8 cmd);//IICΪCMDд��һ���ֽڵ�����

extern void OLED_Display_On(void);//OLED������ʾ

extern void OLED_Display_Off(void);//OLED�ر���ʾ		   		    

extern void OLED_Init(void);//��ʼ��OLED

extern void OLED_Clear(void);//OLED����

extern void OLED_DrawPoint(u8 x, u8 y, u8 t);//��һ����

extern void OLED_Fill(u8 x1, u8 y1, u8 x2, u8 y2, u8 dot);//��һ������

extern void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 Char_Size);//��ʾһ���ַ�

extern void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size);//��ʾһ������

extern void OLED_ShowString(u8 x, u8 y, u8 *p, u8 Char_Size);//��ʾһ���ַ���

extern void OLED_Set_Pos(u8 x, u8 y);//�������ص������

extern void OLED_ShowTEmp(u8 x, u8 y);//��ʾ�����¶�

extern void OLED_ShowHUm(u8 x, u8 y);//��ʾ����ʪ��

extern void OLED_ShowTIttle(u8 x, u8 y);//��ʾ���϶ȵķ���

extern void OLED_ShowCHinese(u8 x, u8 y);//��ʾ����

extern void OLED_DrawBMP(u8 x0, u8 y0, u8 x1, u8 y1, u8 BMP[]);//��ʾͼƬ

extern void fill_picture(u8 fill_Data);//���ͼƬ

#endif  
	 



