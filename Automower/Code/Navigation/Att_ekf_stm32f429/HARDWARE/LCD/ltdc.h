#ifndef __LTDC_H
#define __LTDC_H		
#include "sys.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//LTDC����	   	    
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/10
//�汾��V1.2
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved	 
//********************************************************************************
//�޸�˵�� 
//V1.1   20160321
//1,������1024*600��7��RGB��֧��
//2,������480*272��4.3��RGB��֧��
//V1.2   20160503
//����LTDC_Init����SSCR�Ĵ������õ�bug
//////////////////////////////////////////////////////////////////////////////////	 

//LCD LTDC��Ҫ������
typedef struct  
{							 
	u32 pwidth;			//LCD���Ŀ��,�̶�����,������ʾ����ı�,���Ϊ0,˵��û���κ�RGB������
	u32 pheight;		//LCD���ĸ߶�,�̶�����,������ʾ����ı�
	u16 hsw;			//ˮƽͬ�����
	u16 vsw;			//��ֱͬ�����
	u16 hbp;			//ˮƽ����
	u16 vbp;			//��ֱ����
	u16 hfp;			//ˮƽǰ��
	u16 vfp;			//��ֱǰ�� 
	u8 activelayer;		//��ǰ����:0/1	
	u8 dir;				//0,����;1,����;
	u16 width;			//LCD���
	u16 height;			//LCD�߶�
	u32 pixsize;		//ÿ��������ռ�ֽ���
}_ltdc_dev; 	  
extern _ltdc_dev lcdltdc;							//����LCD LTDC����



#define LCD_PIXFORMAT_ARGB8888		0X00			//ARGB8888��ʽ
#define LCD_PIXFORMAT_RGB888 		0X01			//ARGB8888��ʽ
#define LCD_PIXFORMAT_RGB565 		0X02 			//ARGB8888��ʽ
#define LCD_PIXFORMAT_ARGB1555		0X03			//ARGB8888��ʽ
#define LCD_PIXFORMAT_ARGB4444		0X04			//ARGB8888��ʽ
#define LCD_PIXFORMAT_L8			0X05			//ARGB8888��ʽ
#define LCD_PIXFORMAT_AL44			0X06			//ARGB8888��ʽ
#define LCD_PIXFORMAT_AL88 			0X07			//ARGB8888��ʽ

///////////////////////////////////////////////////////////////////////
//�û��޸����ò���:

//������ɫ���ظ�ʽ,һ����RGB565
#define LCD_PIXFORMAT				LCD_PIXFORMAT_RGB565	
//����Ĭ�ϱ�������ɫ
#define LTDC_BACKLAYERCOLOR			0X00000000	
//LCD֡�������׵�ַ,���ﶨ����SDRAM����.
#define LCD_FRAME_BUF_ADDR			0XC0000000  	
 


void LTDC_Switch(u8 sw);					//LTDC����
void LTDC_Layer_Switch(u8 layerx,u8 sw);	//�㿪��
void LTDC_Select_Layer(u8 layerx);			//��ѡ��
void LTDC_Display_Dir(u8 dir);				//��ʾ�������
void LTDC_Draw_Point(u16 x,u16 y,u32 color);//���㺯��
u32 LTDC_Read_Point(u16 x,u16 y);			//���㺯��
void LTDC_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);			//���ε�ɫ��亯��
void LTDC_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);	//���β�ɫ��亯��
void LTDC_Clear(u32 color);					//��������
u8 LTDC_Clk_Set(u32 pllsain,u32 pllsair,u32 pllsaidivr);//LTDCʱ������
void LTDC_Layer_Window_Config(u8 layerx,u16 sx,u16 sy,u16 width,u16 height);//LTDC�㴰������
void LTDC_Layer_Parameter_Config(u8 layerx,u32 bufaddr,u8 pixformat,u8 alpha,u8 alpha0,u8 bfac1,u8 bfac2,u32 bkcolor);//LTDC������������
u16 LTDC_PanelID_Read(void);				//LCD ID��ȡ����
void LTDC_Init(void);						//LTDC��ʼ������
#endif







