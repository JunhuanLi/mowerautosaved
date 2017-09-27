/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			lcd.h
  Author:				Raymond
  Date:				2017.5.16
  Version:        
  Description:    // ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
                  // �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
                  // �ơ�˳�򡢶����������ȹ�ϵ
  History:        // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                  // �߼��޸����ݼ���  
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/



#ifndef __LCD_H__
#define __LCD_H__

#include "stm32f4xx.h"
#include "stdlib.h" 


/* LCD��Ҫ������ */
typedef struct  
{		 	 
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;			//���������������ƣ�0��������1��������	
	u16	wram_cmd;		//��ʼдgramָ��
	u16 setx_cmd;		//����x����ָ��
	u16 sety_cmd;		//����y����ָ�� 
}T_lcd_dev; 	  


//LCD����
extern T_lcd_dev lcd_dev;//����LCD��Ҫ����
//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern u32  point_color;//Ĭ�Ϻ�ɫ    
extern u32  back_color; //������ɫ.Ĭ��Ϊ��ɫ


/* PB5����LCD���� */ 
#define	LCD_BACK_LIGHT GPIO_SetBits(GPIOB,GPIO_Pin_5) 


/* LCD��ַ�ṹ�� */
typedef struct
{
	vu16 lcd_reg;
	vu16 lcd_ram;
} T_lcd_typedef;


//ʹ��NOR/SRAM�� Bank1.sector1,��ַλHADDR[27,26]=00 A18��Ϊ�������������� 
//ע������ʱSTM32�ڲ�������һλ����!  			    
#define LCD_BASE        	((u32)(0x60000000 | 0x0007FFFE))
#define MOWER_LCD           ((T_lcd_typedef *) LCD_BASE)

/* ɨ�跽���� */
#define L2R_U2D  0 		//������,���ϵ���
#define L2R_D2U  1 		//������,���µ���
#define R2L_U2D  2 		//���ҵ���,���ϵ���
#define R2L_D2U  3 		//���ҵ���,���µ���

#define U2D_L2R  4 		//���ϵ���,������
#define U2D_R2L  5 		//���ϵ���,���ҵ���
#define D2U_L2R  6 		//���µ���,������
#define D2U_R2L  7		//���µ���,���ҵ���	 

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��


/* ������ɫ */
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ


//GUI��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
	    															  
void lcd_init(void);													   	//��ʼ��
void lcd_display_on(void);													//����ʾ
void lcd_display_off(void);													//����ʾ
void lcd_clear(u32 Color);	 												//����
void lcd_set_cursor(u16 Xpos, u16 Ypos);										//���ù��
void lcd_draw_point(u16 x,u16 y);											//����
void lcd_fast_draw_point(u16 x,u16 y,u32 color);								//���ٻ���
u32  lcd_read_point(u16 x,u16 y); 											//���� 
void lcd_draw_circle(u16 x0,u16 y0,u8 r);						 			//��Բ
void lcd_draw_line(u16 x1, u16 y1, u16 x2, u16 y2);							//����
void lcd_draw_rectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   				//������
void lcd_fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);		   				//��䵥ɫ
void lcd_color_fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);				//���ָ����ɫ
void lcd_show_char(u16 x,u16 y,u8 num,u8 size,u8 mode);						//��ʾһ���ַ�
void lcd_show_num(u16 x,u16 y,u32 num,u8 len,u8 size);  						//��ʾһ������
void lcd_show_string_num(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//��ʾ ����
void lcd_show_string(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//��ʾһ���ַ���,12/16����

void lcd_write_reg_data(u16 LCD_Reg, u16 LCD_RegValue);
u16 lcd_read_reg_data(u16 LCD_Reg);
void lcd_write_ram_prepare(void);
void lcd_write_ram(u16 RGB_Code);
void lcd_ssd_back_light_set(u8 pwm);							//SSD1963 �������
void lcd_scan_dir(u8 dir);									//������ɨ�跽��
void lcd_display_dir(u8 dir);								//������Ļ��ʾ����
void lcd_set_window(u16 sx,u16 sy,u16 width,u16 height);	//���ô���					   						   																			 


//LCD�ֱ�������
#define SSD_HOR_RESOLUTION		800		//LCDˮƽ�ֱ���
#define SSD_VER_RESOLUTION		480		//LCD��ֱ�ֱ���
//LCD������������
#define SSD_HOR_PULSE_WIDTH		1		//ˮƽ����
#define SSD_HOR_BACK_PORCH		46		//ˮƽǰ��
#define SSD_HOR_FRONT_PORCH		210		//ˮƽ����

#define SSD_VER_PULSE_WIDTH		1		//��ֱ����
#define SSD_VER_BACK_PORCH		23		//��ֱǰ��
#define SSD_VER_FRONT_PORCH		22		//��ֱǰ��

//���¼����������Զ�����
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

extern struct rt_device lcd_dev_driver;

void lcd_nt35510_config(void);
rt_err_t lcd_nt35510_init(rt_device_t lcd);

#endif  
	 
	 



