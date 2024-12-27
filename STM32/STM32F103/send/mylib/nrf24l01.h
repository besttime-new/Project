#ifndef __SPI_NRF_H__
#define __SPI_NRF_H__
/*
�������豸		�����ӵĹܽ�
CE						PC5
CSN						PC4
SCK						PA7
MOSI					PA6
MISO					PA5
IRQ						PA4
��֪оƬ�Ĺܽ��Ƕ๦�ܸ����͵Ĺܽ�
оƬ�ĹܽŻ��� ���빦�� ������� ���ù��� ģ�⹦��
������β������ǿ������ϵ�����ͨ��ģ��Nrf24L01
��֪��ģ��Ľӿ���SPI�ӿ�,������Ҫ��ģ����뵽��SPI�ӿڵĹܽ���
����Ϊ�˿��ǵ���ֲ�Ե�����,���ǻ��ǲ�ȡʹ��GPIO�ķ�ʽģ��SPIЭ��
*/
#include "stm32f10x_conf.h"//�����˸�ͷ�ļ����е�����ͷ�ļ����Ѱ���
//��Nrfģ��֮��ͨ��(һ�η�������<=32�ֽ�)
//�����������ṹ��(32�ֽ�)����ͨ��
//����ĳ�Ա�ĺ����Լ�˵����
struct nrf_msg_st//����ķ������ݰ��Ľṹ��(���ܳ���32�ֽ�)
{
	u32 Dht_Hum;//�洢DHT11��ʪ��
	u32 Dht_Temp;//�洢DHT11���¶�
	float Sht_Temp;//�洢SHT30���¶�
	float Sht_Hum;//�洢SHT30��ʪ��
	u8 buf[16];//�洢��������
};
//��GPIO��ģ��SPI����(��ֲ�Ժ�)
//��������ֲʱ�ⲿ����Ҫ�޸�(��Ӳ��ԭ��ͼ)
//���Ҳ�����޸ĳ�λ������
#define NRF_CSN_HIGH()      GPIO_SetBits(GPIOC, GPIO_Pin_4)//Ƭѡ����(���ܲ����豸)
#define NRF_CSN_LOW()      	GPIO_ResetBits(GPIOC, GPIO_Pin_4)//Ƭѡ����(���Բ����豸)
#define NRF_CE_HIGH()	   		GPIO_SetBits(GPIOC, GPIO_Pin_5)//����CE(����Ƶ)
#define NRF_CE_LOW()	   		GPIO_ResetBits(GPIOC, GPIO_Pin_5)//����CE(�ر���Ƶ)
#define NRF_Read_IRQ()			GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4)//���жϵĹܽ�(�����Ƿ������ݻ��ǽ������ݶ��ǵ͵�ƽ)

#define NRF_MOSI_HIGH()			GPIO_SetBits(GPIOA, GPIO_Pin_6)//MOSI���� 
#define NRF_MOSI_LOW()			GPIO_ResetBits(GPIOA, GPIO_Pin_6)//MOSI����
#define NRF_CLK_HIGH()			GPIO_SetBits(GPIOA, GPIO_Pin_7)//CLK����
#define NRF_CLK_LOW()				GPIO_ResetBits(GPIOA, GPIO_Pin_7)//CLK����
#define NRF_Read_MISO()			GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)//��MISO(����ӳ�)

#define TX_ADR_WIDTH 		5  	//�����ַ���(5���ֽ�)

#define RX_ADR_WIDTH    5		//ÿһ��Nrf������һ����ַ(3 4 5bytes)

// SPI(nRF24L01) commands
#define NRF_READ_REG    0x00  // Define read command to register
#define NRF_WRITE_REG   0x20  // Define write command to register
#define RD_RX_PLOAD 		0x61  // Define RX payload register address
#define WR_TX_PLOAD 		0xA0  // Define TX payload register address
#define FLUSH_TX    		0xE1  // Define flush TX register command
#define FLUSH_RX    		0xE2  // Define flush RX register command
#define REUSE_TX_PL 		0xE3  // Define reuse TX payload register command
#define NOP         		0xFF  // Define No Operation, might be used to read status register

// SPI(nRF24L01) registers(addresses)
#define CONFIG      		0x00  // 'Config' register address
#define EN_AA       		0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   		0x02  // 'Enabled RX addresses' register address
#define SETUP_AW    		0x03  // 'Setup address width' register address
#define SETUP_RETR  		0x04  // 'Setup Auto. Retrans' register address
#define RF_CH       		0x05  // 'RF channel' register address
#define RF_SETUP    		0x06  // 'RF setup' register address
#define STATUS      		0x07  // 'Status' register address
#define OBSERVE_TX  		0x08  // 'Observe TX' register address
#define CD          		0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0  		0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1  		0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2  		0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3  		0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4  		0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5  		0x0F  // 'RX address pipe5' register address
#define TX_ADDR     		0x10  // 'TX address' register address
#define RX_PW_P0    		0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1    		0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2    		0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3    		0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4    		0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5    		0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS 		0x17  // 'FIFO Status Register' register address

#define MAX_RT      		0x10 //�ﵽ����ط������жϱ�־λ(��ʱ)
#define TX_DS						0x20 //��������жϱ�־λ	  // ����OK

#define RX_DR						0x40 //���յ������жϱ�־λ //����OK

void SPI_NRF_Init(void);//��Nrf���г�ʼ��
u8 SPI_NRF_RW(u8 dat);//��Nrf���ж�д
//(��SPI���߿���Nrf�Ķ���дʱ��һ��,ֻ������һ���ֽ��еĶ�д���һ��)
u8 SPI_NRF_ReadReg(u8 reg);//��Nrf�Ĵ���
u8 SPI_NRF_WriteReg(u8 reg,u8 dat);//дNrf�Ĵ���
u8 SPI_NRF_ReadBuf(u8 reg,u8 *pBuf,u8 bytes);//��Buf(����һ�ζ��ڶ���Ĵ������в���<�����������ļĴ���>)
u8 SPI_NRF_WriteBuf(u8 reg ,u8 *pBuf,u8 bytes);	//дBuf()

void NRF_RX_Mode(u8 *rxAddr, u8 channel, u8 recvLen);//��Nrf����Ϊ����ģʽ(�Լ��ĵ�ַ,�Լ���ͨ��,�Լ��������ݳ���)
void NRF_TX_Mode(u8 *txAddr, u8 channel);//��Nrf����Ϊ����ģʽ(���͵ĵ�ַ,ͨ��)<���͵�ַ���Լ��Ľ��յ�ַһ��>
u8 NRF_Rx_Dat(u8 *rxbuf, u8 recvLen);//��������
u8 NRF_Tx_Dat(u8 *txbuf, u8 recvLen);//��������
u8 NRF_Check(void); //���STM32��NRFӲ������û�����Ӻ�
//����˵��Nrfĳ���Ĵ���д��10�ٶ�һ������Ĵ���,��������ֵû�����˵��OK
//��������ʵ���õĶ�����ѯ�ķ�ʽ(S5P6818���õ����жϵķ�ʽ)
#endif 





















