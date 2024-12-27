#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
#include "gpio_iic.h"
/*
һ����˵OLED��ʾ�����һ�����ȫ���ֲ�Ͱ���,ֱ����ֲ����
*/
//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127

/**********************************************
//IIC Start
**********************************************/
void IIC_Start_Oled(void)
{
	IIC_Start();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop_Oled(void)
{	
	IIC_Stop();	
}

/**********************************************
//IIC Wait Ack
**********************************************/
void IIC_Wait_Ack_Oled(void)
{
	IIC_Wait_Ack();
}

/**********************************************
// IIC Write byte
**********************************************/
void Write_IIC_Byte(u8 IIC_Byte)
{
	IIC_Send_Byte(IIC_Byte);
}

/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(u8 IIC_Command)
{
	IIC_Start();
	IIC_Send_Byte(0x78);				//Slave address,SA0=0
	IIC_Wait_Ack();	
	IIC_Send_Byte(0x00);				//write command
	IIC_Wait_Ack();	
	IIC_Send_Byte(IIC_Command); 
	IIC_Wait_Ack();	
	IIC_Stop();
}

/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(u8 IIC_Data)
{
	IIC_Start();
	IIC_Send_Byte(0x78);				//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
	IIC_Send_Byte(0x40);				//write data
	IIC_Wait_Ack();	
	IIC_Send_Byte(IIC_Data); 
	IIC_Wait_Ack();	
	IIC_Stop();
}

/**********************************************
// IIC writes a byte of data for a cmd
**********************************************/
void OLED_WR_Byte(u8 data, u8 cmd)
{
	if(cmd)
		Write_IIC_Data(data);
	else
		Write_IIC_Command(data);
}

/********************************************
// fill_Picture
********************************************/
void fill_picture(u8 fill_Data)
{
	u8 m,n;
	
	for(m = 0; m < 8; m++)
	{
		OLED_WR_Byte(0xb0 + m, 0);	//page0-page1
		OLED_WR_Byte(0x00, 0);			//low column start address
		OLED_WR_Byte(0x10, 0);			//high column start address
		for(n = 0; n < 128; n++)
		{
			OLED_WR_Byte(fill_Data, 1);
		}
	}
}

/********************************************
// Oled Setting coordinates
********************************************/
void OLED_Set_Pos(u8 x, u8 y) 
{ 	
	OLED_WR_Byte(0xb0 + y, OLED_CMD);
	OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte((x & 0x0f), OLED_CMD); 
}

/********************************************
// Oled Open Display
********************************************/
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14, OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD);  //DISPLAY ON
}

/********************************************
// Oled Close Display
********************************************/
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10, OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE, OLED_CMD);  //DISPLAY OFF
}

/********************************************
// Oled Clear Screen(All points do not show color)
********************************************/
void OLED_Clear(void)  
{  
	u8 i, n;
	for(i = 0; i < 8; i++)  
	{  
		OLED_WR_Byte(0xb0 + i, OLED_CMD);	//����ҳ��ַ��0~7��
		OLED_WR_Byte(0x00, OLED_CMD);			//������ʾλ�á��е͵�ַ
		OLED_WR_Byte(0x10, OLED_CMD);			//������ʾλ�á��иߵ�ַ   
		for(n = 0; n < 128; n++)
			OLED_WR_Byte(0, OLED_DATA); 
	} //������ʾ
}

/********************************************
// Oled Clear Screen(All Points Display Color)
********************************************/
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i = 0; i < 8; i++)  
	{  
		OLED_WR_Byte(0xb0 + i, OLED_CMD);	//����ҳ��ַ��0~7��
		OLED_WR_Byte(0x00, OLED_CMD);			//������ʾλ�á��е͵�ַ
		OLED_WR_Byte(0x10, OLED_CMD);			//������ʾλ�á��иߵ�ַ   
		for(n = 0; n < 128; n++)
			OLED_WR_Byte(1, OLED_DATA); 
	} //������ʾ
}

/********************************************
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//chr:����Ҫ��ʾ���ַ�
//size:ѡ������ 16/12
********************************************/ 
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 Char_Size)
{      	
	u8 c = 0, i = 0;
	
	c = chr - ' ';//�õ�ƫ�ƺ��ֵ			
	if(x > 128 - 1)
	{
		x = 0;
		y += 2;
	}
	if(Char_Size == 16)
	{
		OLED_Set_Pos(x, y);	
		for(i = 0; i < 8; i++)
			OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
		OLED_Set_Pos(x, y + 1);
		for(i = 0; i < 8; i++)
			OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
	}
	else
	{	
		OLED_Set_Pos(x, y);
		for(i = 0; i < 6; i++)
			OLED_WR_Byte(F6x8[c][i], OLED_DATA);
	}
}

/********************************************
//��m��n�η��ĺ���
********************************************/
u32 oled_pow(u8 m, u8 n)
{
	u32 result = 1;	 
	while(n--)result *= m;    
	return result;
}
/********************************************
//��ʾ��������
//x,y :�������(x:0~127 y:0~63)
//num:��ֵ(0~4294967295);
//len :���ֵ�λ��
//size:�����С(16/12)
********************************************/	 		  
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size2)
{         	
	u8 t, temp;
	u8 enshow=0;
	
	for(t = 0; t < len; t++)
	{
		temp = (num / oled_pow(10, len - t - 1)) % 10;
		if(enshow == 0 && t < (len - 1))
		{
			if(temp == 0)
			{
				OLED_ShowChar(x + (size2 / 2) * t, y, ' ', size2);
				continue;
			}else 
				enshow = 1;
		}
	 	OLED_ShowChar(x + (size2 / 2) * t, y , temp + '0', size2); 
	}
}

/********************************************
//��ʾһ���ַ��Ŵ�
//x,y :�������(x:0~127 y:0~63)
//chr :��ʾ���ַ���
//size:�����С(16/12)
********************************************/
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 Char_Size)
{
	u8 j = 0;
	
	while(chr[j])
	{
		OLED_ShowChar(x, y, chr[j], Char_Size);
		x += 8;
		if(x > 120)
		{
			x = 0;
			y += 2;
		}
		j++;
	}
}

/********************************************
//��ʾ�����¶�(������������oledfont.h�ļ���)
//x,y :�������(x:0~127 y:0~63)
********************************************/
void OLED_ShowTEmp(u8 x, u8 y)
{
	u8 t, i, adder = 0, flag = 0;
	
	for(i = 0; i < 4; i++)
	{
		if(i == 2) flag = 16;
		OLED_Set_Pos(x + flag, y + (i % 2));	
		for(t = 0; t < 16; t++)
		{
			OLED_WR_Byte(Temp[i][t], OLED_DATA);
			adder += 1;
		}
	}
}

/********************************************
//��ʾ����ʪ��(������������oledfont.h�ļ���)
//x,y :�������(x:0~127 y:0~63)
********************************************/
void OLED_ShowHUm(u8 x, u8 y)
{
	u8 t, i, adder = 0, flag = 0;
	
	for(i = 0; i < 4; i++)
	{
		if(i == 2) flag = 16;
		OLED_Set_Pos(x + flag, y + (i % 2));	
		for(t = 0; t < 16; t++)
		{
			OLED_WR_Byte(Hum[i][t], OLED_DATA);
			adder += 1;
		}
	}
}

/********************************************
//��ʾ���϶ȵķ���(���϶ȵķ����Ѿ�������oledfont.h�ļ���)
//x,y :�������(x:0~127 y:0~63)
********************************************/
void OLED_ShowTIttle(u8 x, u8 y)
{
	u8 t, i, adder = 0;
	
	for(i = 0; i < 2; i++)
	{
		OLED_Set_Pos(x, y + (i % 2));	
		for(t = 0; t < 16; t++)
		{
			OLED_WR_Byte(T[i][t], OLED_DATA);
			adder += 1;
		}
	}
}

/********************************************
//��ʾ��������(��Ҫ����ģ�޸ĳ���)
//x,y :�������(x:0~127 y:0~63)
********************************************/
void OLED_ShowCHinese(u8 x, u8 y)
{      			    
	u8 t, adder = 0;
	OLED_Set_Pos(x, y);	
	for(t = 0; t < 16; t++)
	{
		OLED_WR_Byte(T[0][t], OLED_DATA);
		adder += 1;
	}
	OLED_Set_Pos(x, y + 1);	
	for(t = 0; t < 16; t++)
	{
		OLED_WR_Byte(T[1][t], OLED_DATA);
		adder += 1;
	}	
}

/********************************************
//��ʾBMPͼƬ128��64��ʼ������(x,y),
********************************************/
void OLED_DrawBMP(u8 x0, u8 y0, u8 x1, u8 y1, u8 BMP[])
{ 	
	u32 j = 0;
	u8 x, y;

	if(y1 % 8 == 0)
		y = y1 / 8;      
	else
		y = y1 / 8 + 1;
	for(y = y0; y < y1; y++)
	{
		OLED_Set_Pos(x0, y);
		for(x = x0; x < x1; x++)
		{      
			OLED_WR_Byte(BMP[j++], OLED_DATA);	    	
		}
	}
} 

//��ʼ��SSD1306  
void OLED_Init(void)
{ 	
 	IIC_Init();

	delay_ms(800);
	OLED_WR_Byte(0xAE, OLED_CMD);//--display off
	OLED_WR_Byte(0x00, OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10, OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40, OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0, OLED_CMD);//--set page address
	OLED_WR_Byte(0x81, OLED_CMD); // contract control
	OLED_WR_Byte(0xFF, OLED_CMD);//--128   
	OLED_WR_Byte(0xA1, OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6, OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8, OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F, OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8, OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3, OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00, OLED_CMD);//
	
	OLED_WR_Byte(0xD5, OLED_CMD);//set osc division
	OLED_WR_Byte(0x80, OLED_CMD);//
	
	OLED_WR_Byte(0xD8, OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05, OLED_CMD);//
	
	OLED_WR_Byte(0xD9, OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1, OLED_CMD);//
	
	OLED_WR_Byte(0xDA, OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12, OLED_CMD);//
	
	OLED_WR_Byte(0xDB, OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30, OLED_CMD);//
	
	OLED_WR_Byte(0x8D, OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14, OLED_CMD);//
	
	OLED_WR_Byte(0xAF, OLED_CMD);//--turn on oled panel
}  







