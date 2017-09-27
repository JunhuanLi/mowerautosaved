#include "dac.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//DAC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved 
////////////////////////////////////////////////////////////////////////////////// 	
 

//DACͨ��1�����ʼ��
void Dac1_Init(void)
{   	
	RCC->APB1ENR|=1<<29;   	//ʹ��DACʱ��	   
	RCC->AHB1ENR|=1<<0;  	//ʹ��PORTAʱ��	  
	GPIO_Set(GPIOA,PIN4,GPIO_MODE_AIN,0,0,GPIO_PUPD_PU);//PA4,ģ������,����   

	DAC->CR|=1<<0;	//ʹ��DAC1
	DAC->CR|=1<<1;	//DAC1������治ʹ�� BOFF1=1
	DAC->CR|=0<<2;	//��ʹ�ô������� TEN1=0
	DAC->CR|=0<<3;	//DAC TIM6 TRGO,����ҪTEN1=1����
	DAC->CR|=0<<6;	//��ʹ�ò��η���
	DAC->CR|=0<<8;	//���Ρ���ֵ����
	DAC->CR|=0<<12;	//DAC1 DMA��ʹ��    
	DAC->DHR12R1=0;
}
//����ͨ��1�����ѹ
//vol:0~3300,����0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC->DHR12R1=temp;
}





















































