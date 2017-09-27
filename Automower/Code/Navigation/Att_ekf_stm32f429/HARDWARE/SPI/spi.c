#include "spi.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������ 
//SPI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/28
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI5�ĳ�ʼ��
void SPI5_Init(void)
{	 
	u16 tempreg=0;
	RCC->AHB1ENR|=1<<5;    	//ʹ��PORTFʱ��	   
	RCC->APB2ENR|=1<<20;   	//SPI5ʱ��ʹ�� 
	GPIO_Set(GPIOF,7<<7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);	//PF7~9���ù������	
  	GPIO_AF_Set(GPIOF,7,5);	//PF7,AF5
 	GPIO_AF_Set(GPIOF,8,5);	//PF8,AF5
 	GPIO_AF_Set(GPIOF,9,5);	//PF9,AF5 

	//����ֻ���SPI�ڳ�ʼ��
	RCC->APB2RSTR|=1<<20;	//��λSPI5
	RCC->APB2RSTR&=~(1<<20);//ֹͣ��λSPI5
	tempreg|=0<<10;			//ȫ˫��ģʽ	
	tempreg|=1<<9;			//���nss����
	tempreg|=1<<8;			 
	tempreg|=1<<2;			//SPI����  
	tempreg|=0<<11;			//8λ���ݸ�ʽ	
	tempreg|=1<<1;			//����ģʽ��SCKΪ1 CPOL=1 
	tempreg|=1<<0;			//���ݲ����ӵ�2��ʱ����ؿ�ʼ,CPHA=1  
 	//��SPI5����APB2������.ʱ��Ƶ�����Ϊ96MhzƵ��.
	tempreg|=7<<3;			//Fsck=Fpclk1/256
	tempreg|=0<<7;			//MSB First  
	tempreg|=1<<6;			//SPI���� 
	SPI5->CR1=tempreg; 		//����CR1  
	SPI5->I2SCFGR&=~(1<<11);//ѡ��SPIģʽ
	SPI5_ReadWriteByte(0xff);//��������		 
}   
//SPI5�ٶ����ú���
//SpeedSet:0~7
//SPI�ٶ�=fAPB2/2^(SpeedSet+1)
//fAPB2ʱ��һ��Ϊ96Mhz
void SPI5_SetSpeed(u8 SpeedSet)
{
	SpeedSet&=0X07;			//���Ʒ�Χ
	SPI5->CR1&=0XFFC7; 
	SPI5->CR1|=SpeedSet<<3;	//����SPI5�ٶ�  
	SPI5->CR1|=1<<6; 		//SPI�豸ʹ��	  
} 
//SPI5 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI5_ReadWriteByte(u8 TxData)
{		 			 
	while((SPI5->SR&1<<1)==0);		//�ȴ��������� 
	SPI5->DR=TxData;	 	  		//����һ��byte  
	while((SPI5->SR&1<<0)==0);		//�ȴ�������һ��byte  
 	return SPI5->DR;          		//�����յ�������				    
}








