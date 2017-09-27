#include "rng.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//RNG(�����������)��������	
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//��ʼ��RNG
//����ֵ:0,�ɹ�;1,ʧ��
u8 RNG_Init(void)
{
	u16 retry=0; 
	RCC->AHB2ENR=1<<6;	//����RNGʱ��,����PLL48CLK
	RNG->CR|=1<<2;		//ʹ��RNG
	while((RNG->SR&0X01)==0&&retry<10000)	//�ȴ����������
	{
		retry++;
		delay_us(100);
	}
	if(retry>=10000)return 1;//���������������������
	return 0;
}
//�õ������
//����ֵ:��ȡ���������
u32 RNG_Get_RandomNum(void)
{	 
	while((RNG->SR&0X01)==0);	//�ȴ����������  
	return RNG->DR;	
}
//�õ�ĳ����Χ�ڵ������
//min,max,��С,���ֵ.
//����ֵ:�õ��������(rval),����:min<=rval<=max
int RNG_Get_RandomRange(int min,int max)
{ 
   return RNG_Get_RandomNum()%(max-min+1)+min;
}
















