/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			lcd.h
  Author:				Raymond
  Date:				2017.5.16
  Version:        
  Description:    // 用于详细说明此程序文件完成的主要功能，与其他模块
                  // 或函数的接口，输出值、取值范围、含义及参数间的控
                  // 制、顺序、独立或依赖等关系
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述  
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/



#ifndef __LCD_H__
#define __LCD_H__

#include "stm32f4xx.h"
#include "stdlib.h" 


/* LCD重要参数集 */
typedef struct  
{		 	 
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	wram_cmd;		//开始写gram指令
	u16 setx_cmd;		//设置x坐标指令
	u16 sety_cmd;		//设置y坐标指令 
}T_lcd_dev; 	  


//LCD参数
extern T_lcd_dev lcd_dev;//管理LCD重要参数
//LCD的画笔颜色和背景色	   
extern u32  point_color;//默认红色    
extern u32  back_color; //背景颜色.默认为白色


/* PB5控制LCD背光 */ 
#define	LCD_BACK_LIGHT GPIO_SetBits(GPIOB,GPIO_Pin_5) 


/* LCD地址结构体 */
typedef struct
{
	vu16 lcd_reg;
	vu16 lcd_ram;
} T_lcd_typedef;


//使用NOR/SRAM的 Bank1.sector1,地址位HADDR[27,26]=00 A18作为数据命令区分线 
//注意设置时STM32内部会右移一位对其!  			    
#define LCD_BASE        	((u32)(0x60000000 | 0x0007FFFE))
#define MOWER_LCD           ((T_lcd_typedef *) LCD_BASE)

/* 扫描方向定义 */
#define L2R_U2D  0 		//从左到右,从上到下
#define L2R_D2U  1 		//从左到右,从下到上
#define R2L_U2D  2 		//从右到左,从上到下
#define R2L_D2U  3 		//从右到左,从下到上

#define U2D_L2R  4 		//从上到下,从左到右
#define U2D_R2L  5 		//从上到下,从右到左
#define D2U_L2R  6 		//从下到上,从左到右
#define D2U_R2L  7		//从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向


/* 画笔颜色 */
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
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色


//GUI颜色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
void lcd_init(void);													   	//初始化
void lcd_display_on(void);													//开显示
void lcd_display_off(void);													//关显示
void lcd_clear(u32 Color);	 												//清屏
void lcd_set_cursor(u16 Xpos, u16 Ypos);										//设置光标
void lcd_draw_point(u16 x,u16 y);											//画点
void lcd_fast_draw_point(u16 x,u16 y,u32 color);								//快速画点
u32  lcd_read_point(u16 x,u16 y); 											//读点 
void lcd_draw_circle(u16 x0,u16 y0,u8 r);						 			//画圆
void lcd_draw_line(u16 x1, u16 y1, u16 x2, u16 y2);							//画线
void lcd_draw_rectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   				//画矩形
void lcd_fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);		   				//填充单色
void lcd_color_fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);				//填充指定颜色
void lcd_show_char(u16 x,u16 y,u8 num,u8 size,u8 mode);						//显示一个字符
void lcd_show_num(u16 x,u16 y,u32 num,u8 len,u8 size);  						//显示一个数字
void lcd_show_string_num(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//显示 数字
void lcd_show_string(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//显示一个字符串,12/16字体

void lcd_write_reg_data(u16 LCD_Reg, u16 LCD_RegValue);
u16 lcd_read_reg_data(u16 LCD_Reg);
void lcd_write_ram_prepare(void);
void lcd_write_ram(u16 RGB_Code);
void lcd_ssd_back_light_set(u8 pwm);							//SSD1963 背光控制
void lcd_scan_dir(u8 dir);									//设置屏扫描方向
void lcd_display_dir(u8 dir);								//设置屏幕显示方向
void lcd_set_window(u16 sx,u16 sy,u16 width,u16 height);	//设置窗口					   						   																			 


//LCD分辨率设置
#define SSD_HOR_RESOLUTION		800		//LCD水平分辨率
#define SSD_VER_RESOLUTION		480		//LCD垂直分辨率
//LCD驱动参数设置
#define SSD_HOR_PULSE_WIDTH		1		//水平脉宽
#define SSD_HOR_BACK_PORCH		46		//水平前廊
#define SSD_HOR_FRONT_PORCH		210		//水平后廊

#define SSD_VER_PULSE_WIDTH		1		//垂直脉宽
#define SSD_VER_BACK_PORCH		23		//垂直前廊
#define SSD_VER_FRONT_PORCH		22		//垂直前廊

//如下几个参数，自动计算
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

extern struct rt_device lcd_dev_driver;

void lcd_nt35510_config(void);
rt_err_t lcd_nt35510_init(rt_device_t lcd);

#endif  
	 
	 



