#include "nrf24l01.h"//����ͷ�ļ�

void SPI2_For_Nrf24l01(void)//��ʼ��NRF��ʹ�õ���SPI�ӿڵĹܽ�
{
	GPIO_InitTypeDef GPIO_InitStructure;//������GPIO�̼��Ľṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	//ͨ��APB2����ʹ��GPIOA���GPIOC���ʱ��
	
	//CSN PC4 OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//ѡ������������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//ѡ����4�Źܽ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//ѡ����50MHz����ٶ�
	GPIO_Init(GPIOC, &GPIO_InitStructure);//�����������ó�ʼ��GPIOC��Ĺܽ�
	GPIO_SetBits(GPIOC, GPIO_Pin_4);//Ĭ���Ǹߵ�ƽ(�͵�ƽ��Ч)
	
	//CE PC5 OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//ѡ������������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//ѡ����5�Źܽ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//ѡ����50MHz����ٶ�
	GPIO_Init(GPIOC, &GPIO_InitStructure);//�����������ó�ʼ��GPIOC��Ĺܽ�
	GPIO_ResetBits(GPIOC, GPIO_Pin_5);//Ĭ���ǵ͵�ƽ(�Ȳ�����Ҳ������)

	//MOSI PA6 OUT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//ѡ������������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//ѡ����6�Źܽ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//ѡ����50MHz����ٶ�
	GPIO_Init(GPIOA, &GPIO_InitStructure);//�����������ó�ʼ��GPIOA��Ĺܽ�
	
	//SPI2_CLK PA7 OUT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//ѡ����7�Źܽ�
	GPIO_Init(GPIOA, &GPIO_InitStructure);//�����������ó�ʼ��GPIOA��Ĺܽ�
	
	//MISO PA5 IN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//ѡ���˸�������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//ѡ����5�Źܽ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//ѡ����50MHz������ٶ�
	GPIO_Init(GPIOA, &GPIO_InitStructure);//�����������ó�ʼ��GPIOA��Ĺܽ�
	
	//IRQ PA4 IN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//ѡ������������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//ѡ����5�Źܽ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//ѡ����50MHz������ٶ�
	GPIO_Init(GPIOA, &GPIO_InitStructure);//�����������ó�ʼ��GPIOA��Ĺܽ�
}
//�����޸ĳ�delay_ms delay_us
void Delay(__IO u32 nCount)//��ʱ����
{
  for(; nCount != 0; nCount--);
} 
//----------------------------------------------------------------//


//----------���´�����оƬ�޹�------------------------------------//
//����ʹ�õ���SPI���ߵ�
//SCK��ʼ�͵�ƽ��������Ч
/*
 * ��������SPI_NRF_RW
 * ����  ����Nrf���ж�д
 * ����  ��dat:��Ҫд�������
 * ���  ��дÿλ����ʱ��״̬
 * ����  ���ڲ�����
 */
u8 SPI_NRF_RW(u8 dat)//����һ�߷�һ����
{  	
	int count = 0;

	/*ÿһ��ѭ��дһλ����,���һ�ȡһλ����*/
	for(count = 0; count < 8; count++){
		NRF_CLK_LOW();
		if(dat & 0x80) {
			NRF_MOSI_HIGH();
		} else {
			NRF_MOSI_LOW();
		}
		dat <<= 1;
		NRF_CLK_HIGH();
		dat |= NRF_Read_MISO();	
	}
	NRF_CLK_LOW();	

	return dat;
}



/*
 * ��������SPI_NRF_Init
 * ����  �����ڶ�Nrf���г�ʼ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void SPI_NRF_Init(void)
{
	/*��ʼ��Nrf24l01��ʹ�õ�GPIO*/
	SPI2_For_Nrf24l01();
	/*Ĭ�ϲ�������״̬�°�Ƭѡ����*/
	NRF_CSN_HIGH(); 
}

/*
 * ��������SPI_NRF_WriteReg
 * ����  ��������NRF�ض��ļĴ���д������
 * ����  ��	reg:NRF������+�Ĵ�����ַ��
						dat:��Ҫ��Ĵ���д�������
 * ���  ��NRF��status�Ĵ�����״̬
 * ����  ���ڲ�����
 */
u8 SPI_NRF_WriteReg(u8 reg,u8 dat)
{
	u8 status;
	/*�ر���Ƶ*/
	NRF_CE_LOW();
	/*�õ�CSN��ʹ��SPI����*/
	NRF_CSN_LOW();
				
	/*��������Ĵ����� */
	status = SPI_NRF_RW(reg);//��д��ʱ��Ҳ������״̬��
		 
	/*��Ĵ���д������*/
	SPI_NRF_RW(dat); 
	          
	/*CSN���ߣ����*/	   
	NRF_CSN_HIGH();	
		
	/*����״̬�Ĵ�����ֵ*/
	return(status);
}


/*
 * ��������SPI_NRF_ReadReg
 * ����  �����ڴ�NRF�ض��ļĴ�����������
 * ����  ��reg:NRF������+�Ĵ�����ַ��
 * ���  ���Ĵ����е�����
 * ����  ���ڲ�����
 */
u8 SPI_NRF_ReadReg(u8 reg)
{
 	u8 reg_val;

	/*�ر���Ƶ*/
	NRF_CE_LOW();
	/*�õ�CSN��ʹ��SPI����*/
 	NRF_CSN_LOW();
				
	/*���ͼĴ�����*/
	SPI_NRF_RW(reg); 

	/*��ȡ�Ĵ�����ֵ */
	reg_val = SPI_NRF_RW(NOP);
	            
	/*CSN���ߣ����*/
	NRF_CSN_HIGH();		
   	
	return reg_val;
}	


/*
 * ��������SPI_NRF_ReadBuf
 * ����  �����ڴ�NRF�ļĴ����ж���һ������
 * ����  ��	reg:NRF������+�Ĵ�����ַ��
						pBuf:���ڴ洢���������ļĴ������ݵ����飬�ⲿ����
						bytes:pBuf�����ݳ���	
 * ���  ��NRF��status�Ĵ�����״̬
 * ����  ���ⲿ����
 */
u8 SPI_NRF_ReadBuf(u8 reg,u8 *pBuf,u8 bytes)
{
 	u8 status, byte_cnt;

	NRF_CE_LOW();
	
	/*�õ�CSN��ʹ��SPI����*/
	NRF_CSN_LOW();
		
	/*���ͼĴ�����*/		
	status = SPI_NRF_RW(reg); 

 	/*��ȡ����������*/
	for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)		  
		pBuf[byte_cnt] = SPI_NRF_RW(NOP); //��NRF24L01��ȡ����  

	/*CSN���ߣ����*/
	NRF_CSN_HIGH();	
		
 	return status;		//���ؼĴ���״ֵ̬
}


/*
 * ��������SPI_NRF_WriteBuf
 * ����  ��������NRF�ļĴ�����д��һ������
 * ����  ��reg:NRF������+�Ĵ�����ַ��
 		   pBuf���洢�˽�Ҫд��д�Ĵ������ݵ����飬�ⲿ����
		   bytes: pBuf�����ݳ���	
 * ���  ��NRF��status�Ĵ�����״̬
 * ����  ���ⲿ����
 */
u8 SPI_NRF_WriteBuf(u8 reg ,u8 *pBuf,u8 bytes)
{
	u8 status,byte_cnt;
	
	NRF_CE_LOW();
	/*�õ�CSN��ʹ��SPI����*/
	NRF_CSN_LOW();			

	/*���ͼĴ�����*/	
	status = SPI_NRF_RW(reg); 
 	
	/*�򻺳���д������*/
	for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)
		SPI_NRF_RW(*pBuf++);	//д���ݵ������� 	 
	  	   
	/*CSN���ߣ����*/
	NRF_CSN_HIGH();			
  
	return (status);	//����NRF24L01��״̬ 		
}

/*
 * ��������NRF_RX_Mode
 * ����  �����ò��������ģʽ
 * ����  ����	
 * ���  ����
 * ����  ���ⲿ����
 */
void NRF_RX_Mode(u8 *rxAddr, u8 channel, u8 recvLen)
{
	NRF_CE_LOW();	

	SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0, rxAddr, RX_ADR_WIDTH);//����RX_ADDR_P0�ĵ�ַ
	SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA, 0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
	SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR, 0x01);//ʹ��ͨ��0�Ľ��յ�ַ    
	SPI_NRF_WriteReg(NRF_WRITE_REG+SETUP_AW, 0x03);//��ַΪ5���ֽ�
	SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH, channel); //����RFͨ��Ƶ��    
	SPI_NRF_WriteReg(NRF_WRITE_REG+RX_PW_P0, recvLen);//ѡ��ͨ��0����Ч���ݿ��      
	SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP, 0x0f); //����TX�������,0db����,2Mbps,���������濪��   
	SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG, 0x0f);  //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
	
	/*CE���ߣ��������ģʽ,��Ƶ��ʼ����*/	
  NRF_CE_HIGH();
	Delay(0xffff); 
}    

/*
 * ��������NRF_TX_Mode
 * ����  �����÷���ģʽ
 * ����  ����	
 * ���  ����
 * ����  ���ⲿ����
 */
void NRF_TX_Mode(u8 *txAddr, u8 channel)
{  
	NRF_CE_LOW();		

	SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR, txAddr, TX_ADR_WIDTH);//����TX_ADDR��ַ 
	SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0, txAddr, RX_ADR_WIDTH);//����RX�ڵ��ַ,��ҪΪ��ʹ��ACK   
	SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA, 0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
	SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR, 0x01); //ʹ��ͨ��0�Ľ��յ�ַ 
	SPI_NRF_WriteReg(NRF_WRITE_REG+SETUP_AW, 0x03);//��ַΪ5λ��
	SPI_NRF_WriteReg(NRF_WRITE_REG+SETUP_RETR, 0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
	SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH, channel);       //����RFͨ��ΪCHANAL
	SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP, 0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
	SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG, 0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	
	/*CE���ߣ����뷢��ģʽ*/	
  NRF_CE_HIGH();
	Delay(0xffff); //CEҪ����һ��ʱ��Ž��뷢��ģʽ
}


/*
 * ��������NRF_Check
 * ����  ����Ҫ����NRF��MCU�Ƿ���������
 * ����  ����	
 * ���  ��SUCCESS/ERROR ��������/����ʧ��
 * ����  ���ⲿ����
 */
u8 NRF_Check(void)
{
	u8 buf[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
	u8 buf1[5];
	u8 i; 
	 
	/*д��5���ֽڵĵ�ַ.  */  
	SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,buf,5);

	/*����д��ĵ�ַ */
	SPI_NRF_ReadBuf(TX_ADDR,buf1,5); 
	 
	/*�Ƚ�*/               
	for(i=0;i<5;i++){
		if(buf1[i]!=0xC2)
			break;
	} 
	       
	if(i==5){
		return SUCCESS ;        //MCU��NRF�ɹ����� 
	}else{
		return ERROR ;        //MCU��NRF����������
	}
}

 /*
 * ��������NRF_Tx_Dat
 * ����  ��������NRF�ķ��ͻ�������д������
 * ����  ��txBuf���洢�˽�Ҫ���͵����ݵ����飬�ⲿ����	
 * ���  �����ͽ�����ɹ�����TXDS,ʧ�ܷ���MAXRT��ERROR
 * ����  ���ⲿ����
 */ 
u8 NRF_Tx_Dat(u8 *txbuf, u8 sendLen)
{
	u8 state;
  
	/*ceΪ�ͣ��������ģʽ1*/
	NRF_CE_LOW();

	/*д���ݵ�TX BUF ��� 32���ֽ�*/						
	SPI_NRF_WriteBuf(WR_TX_PLOAD,txbuf,sendLen);

	/*CEΪ�ߣ�txbuf�ǿգ��������ݰ� */   
	NRF_CE_HIGH();
	  	
	while(NRF_Read_IRQ()!=0); //�����޸ĳ��жϴ���
		                         
	state = SPI_NRF_ReadReg(STATUS);
   
	/*���TX_DS��MAX_RT�жϱ�־*/                  
	SPI_NRF_WriteReg(NRF_WRITE_REG+STATUS,state); 	
	SPI_NRF_WriteReg(FLUSH_TX,NOP);    //���TX FIFO�Ĵ��� 

	/*�ж��ж�����*/    
	if(state&MAX_RT){                     //�ﵽ����ط�����
			 return MAX_RT; 
	}else if(state&TX_DS){                  //�������
		 	return TX_DS;
	}else{						  
			return ERROR;                 //����ԭ����ʧ��
	}
} 

 /*
 * ��������NRF_Rx_Dat
 * ����  �����ڴ�NRF�Ľ��ջ������ж�������
 * ����  ��rxBuf�����ڽ��ո����ݵ����飬�ⲿ����	
 * ���  �����ս����
 * ����  ���ⲿ����
 */ 
u8 NRF_Rx_Dat(u8 *rxbuf, u8 recvLen)
{
	u8 state; 
	NRF_CE_HIGH();	 //�������״̬
	 /*�ȴ������ж�*/
	while(NRF_Read_IRQ()!=0); 
	
	NRF_CE_LOW();  	 //�������״̬
	/*��ȡstatus�Ĵ�����ֵ  */               
	state=SPI_NRF_ReadReg(STATUS);
	 
	/* ����жϱ�־*/      
	SPI_NRF_WriteReg(NRF_WRITE_REG+STATUS,state);

	/*�ж��Ƿ���յ�����*/
	if(state&RX_DR){                                 //���յ�����
	  SPI_NRF_ReadBuf(RD_RX_PLOAD,rxbuf,recvLen);//��ȡ����
		SPI_NRF_WriteReg(FLUSH_RX,NOP);          //���RX FIFO�Ĵ���
	  return RX_DR; 
	}else{    
		return ERROR;                    //û�յ��κ�����
	}
}

