#ifndef __LTDC_H
#define __LTDC_H		
#include "sys.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//LTDC驱动	   	    
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/12/10
//版本：V1.2
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved	 
//********************************************************************************
//修改说明 
//V1.1   20160321
//1,新增对1024*600的7寸RGB屏支持
//2,新增对480*272的4.3寸RGB屏支持
//V1.2   20160503
//修正LTDC_Init函数SSCR寄存器配置的bug
//////////////////////////////////////////////////////////////////////////////////	 

//LCD LTDC重要参数集
typedef struct  
{							 
	u32 pwidth;			//LCD面板的宽度,固定参数,不随显示方向改变,如果为0,说明没有任何RGB屏接入
	u32 pheight;		//LCD面板的高度,固定参数,不随显示方向改变
	u16 hsw;			//水平同步宽度
	u16 vsw;			//垂直同步宽度
	u16 hbp;			//水平后廊
	u16 vbp;			//垂直后廊
	u16 hfp;			//水平前廊
	u16 vfp;			//垂直前廊 
	u8 activelayer;		//当前层编号:0/1	
	u8 dir;				//0,竖屏;1,横屏;
	u16 width;			//LCD宽度
	u16 height;			//LCD高度
	u32 pixsize;		//每个像素所占字节数
}_ltdc_dev; 	  
extern _ltdc_dev lcdltdc;							//管理LCD LTDC参数



#define LCD_PIXFORMAT_ARGB8888		0X00			//ARGB8888格式
#define LCD_PIXFORMAT_RGB888 		0X01			//ARGB8888格式
#define LCD_PIXFORMAT_RGB565 		0X02 			//ARGB8888格式
#define LCD_PIXFORMAT_ARGB1555		0X03			//ARGB8888格式
#define LCD_PIXFORMAT_ARGB4444		0X04			//ARGB8888格式
#define LCD_PIXFORMAT_L8			0X05			//ARGB8888格式
#define LCD_PIXFORMAT_AL44			0X06			//ARGB8888格式
#define LCD_PIXFORMAT_AL88 			0X07			//ARGB8888格式

///////////////////////////////////////////////////////////////////////
//用户修改配置部分:

//定义颜色像素格式,一般用RGB565
#define LCD_PIXFORMAT				LCD_PIXFORMAT_RGB565	
//定义默认背景层颜色
#define LTDC_BACKLAYERCOLOR			0X00000000	
//LCD帧缓冲区首地址,这里定义在SDRAM里面.
#define LCD_FRAME_BUF_ADDR			0XC0000000  	
 


void LTDC_Switch(u8 sw);					//LTDC开关
void LTDC_Layer_Switch(u8 layerx,u8 sw);	//层开关
void LTDC_Select_Layer(u8 layerx);			//层选择
void LTDC_Display_Dir(u8 dir);				//显示方向控制
void LTDC_Draw_Point(u16 x,u16 y,u32 color);//画点函数
u32 LTDC_Read_Point(u16 x,u16 y);			//读点函数
void LTDC_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);			//矩形单色填充函数
void LTDC_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);	//矩形彩色填充函数
void LTDC_Clear(u32 color);					//清屏函数
u8 LTDC_Clk_Set(u32 pllsain,u32 pllsair,u32 pllsaidivr);//LTDC时钟配置
void LTDC_Layer_Window_Config(u8 layerx,u16 sx,u16 sy,u16 width,u16 height);//LTDC层窗口设置
void LTDC_Layer_Parameter_Config(u8 layerx,u32 bufaddr,u8 pixformat,u8 alpha,u8 alpha0,u8 bfac1,u8 bfac2,u32 bkcolor);//LTDC基本参数设置
u16 LTDC_PanelID_Read(void);				//LCD ID读取函数
void LTDC_Init(void);						//LTDC初始化函数
#endif







