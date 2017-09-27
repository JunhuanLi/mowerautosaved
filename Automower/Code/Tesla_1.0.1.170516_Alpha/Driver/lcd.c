
/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			lcd.c
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
    


#include "stdlib.h"
#include "stm32f4xx.h"
#include "lcd.h"
#include "font.h" 
#include "usart.h"	

#include "rtthread.h"
#include "stm32f4xx_gpio.h"

 
//LCD的画笔颜色和背景色	   
u32 point_color=0xFF000000;		//画笔颜色
u32 back_color =0xFFFFFFFF;  	//背景色 
  
//管理LCD重要参数
//默认为竖屏
T_lcd_dev lcd_dev;
	 

void lcd_write_reg_16bit(vu16 regval)
{   
	/* 使用-O2优化的时候,必须插入的延时 */
	regval = regval;
	/* 写入要写的寄存器序号 */
	MOWER_LCD->lcd_reg = regval;
}

void lcd_write_data_16bit(vu16 data)
{	  
	/* 使用-O2优化的时候,必须插入的延时 */
	data = data;			
	MOWER_LCD->lcd_ram = data;		 
}

u16 lcd_read_data_16bit(void)
{
	vu16 ram;
	/* 防止被优化 */
	ram = MOWER_LCD->lcd_ram;	
	return ram;	 
}					   
//写寄存器
//lcd_reg:寄存器地址
//lcd_data:要写入的数据
void lcd_write_reg_data(u16 lcd_reg,u16 lcd_data)
{	
	MOWER_LCD->lcd_reg = lcd_reg;		//写入要写的寄存器序号	 
	MOWER_LCD->lcd_ram = lcd_data;		//写入数据	    		 
}	   
//读寄存器
//LCD_Reg:寄存器地址
//返回值:读到的数据
u16 lcd_read_reg_data(u16 LCD_Reg)
{										   
	lcd_write_reg_16bit(LCD_Reg);		//写入要读的寄存器序号
	//delay_us(5);
	systick_us_delay(5);
	return lcd_read_data_16bit();		//返回读到的值
}   
//开始写GRAM
void lcd_write_ram_prepare(void)
{
 	MOWER_LCD->lcd_reg = lcd_dev.wram_cmd;	  
}	 
//LCD写GRAM
//RGB_Code:颜色值
void lcd_write_ram(u16 RGB_Code)
{							    
	MOWER_LCD->lcd_ram = RGB_Code;//写十六位GRAM
}
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
u16 lcd_bgr_to_rgb(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 
//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(u8 i)
{
	while(i--);
}
//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
u32 lcd_read_point(u16 x,u16 y)
{
	
//	malloc();
	
 	u16 r=0,g=0,b=0;

	if(x >= lcd_dev.width || y >= lcd_dev.height)
	{
		return 0;	//超过了范围,直接返回 
	}
	
	lcd_set_cursor(x,y);
	
	if(lcd_dev.id==0X9341||lcd_dev.id==0X5310||lcd_dev.id==0X1963)
	{
		lcd_write_reg_16bit(0X2E);//9341/3510/1963 发送读GRAM指令
	}
	else if(lcd_dev.id==0X5510)
	{
		lcd_write_reg_16bit(0X2E00);	//5510 发送读GRAM指令
	}
	
 	r=lcd_read_data_16bit();			//dummy Read	
 	
	if(lcd_dev.id==0X1963)
	{
		return r;					//1963直接读就可以 
	}
	
	opt_delay(2);	  
 	r=lcd_read_data_16bit();  		  						//实际坐标颜色
	//9341/NT35310/NT35510要分2次读出 
	opt_delay(2);	  
	b=lcd_read_data_16bit(); 
	g=r&0XFF;		//对于9341/5310/5510,第一次读取的是RG的值,R在前,G在后,各占8位
	g<<=8; 
	
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));	//ILI9341/NT35310/NT35510需要公式转换一下
}			 


//LCD开启显示
void lcd_display_on(void)
{					   
	if(lcd_dev.id==0X9341||lcd_dev.id==0X5310||lcd_dev.id==0X1963)
	{
		lcd_write_reg_16bit(0X29);	//开启显示
	}
	else if(lcd_dev.id==0X5510)
	{
		lcd_write_reg_16bit(0X2900);	//开启显示
	}
}


//LCD关闭显示
void lcd_display_off(void)
{	   
	if(lcd_dev.id==0X9341||lcd_dev.id==0X5310||lcd_dev.id==0X1963)
	{
		lcd_write_reg_16bit(0X28);	//关闭显示
	}
	else if(lcd_dev.id==0X5510)
	{
		lcd_write_reg_16bit(0X2800);	//关闭显示
	}
}  


/****************************
//设置光标位置(对RGB屏无效)
//Xpos:横坐标
//Ypos:纵坐标
******************************/
void lcd_set_cursor(u16 Xpos, u16 Ypos)
{	 
 	if(lcd_dev.id == 0X9341 || lcd_dev.id == 0X5310)
	{		    
		lcd_write_reg_16bit(lcd_dev.setx_cmd); 
		lcd_write_data_16bit(Xpos>>8);
		lcd_write_data_16bit(Xpos&0XFF); 			 
		lcd_write_reg_16bit(lcd_dev.sety_cmd); 
		lcd_write_data_16bit(Ypos>>8);
		lcd_write_data_16bit(Ypos&0XFF); 		
	}
	else if(lcd_dev.id == 0X1963)
	{  			 		
		if(lcd_dev.dir == 0)//x坐标需要变换
		{
			Xpos=lcd_dev.width-1-Xpos;
			lcd_write_reg_16bit(lcd_dev.setx_cmd); 
			lcd_write_data_16bit(0);
			lcd_write_data_16bit(0); 		
			lcd_write_data_16bit(Xpos>>8);
			lcd_write_data_16bit(Xpos&0XFF);		 	 
		}
		else
		{
			lcd_write_reg_16bit(lcd_dev.setx_cmd); 
			lcd_write_data_16bit(Xpos>>8);
			lcd_write_data_16bit(Xpos&0XFF); 		
			lcd_write_data_16bit((lcd_dev.width-1)>>8);
			lcd_write_data_16bit((lcd_dev.width-1)&0XFF);		 	 			
		}	
		lcd_write_reg_16bit(lcd_dev.sety_cmd); 
		lcd_write_data_16bit(Ypos>>8);
		lcd_write_data_16bit(Ypos&0XFF); 		
		lcd_write_data_16bit((lcd_dev.height-1)>>8);
		lcd_write_data_16bit((lcd_dev.height-1)&0XFF); 			 		
	}
	else if(lcd_dev.id == 0X5510)
	{
		lcd_write_reg_16bit(lcd_dev.setx_cmd);
		lcd_write_data_16bit(Xpos>>8); 		
		lcd_write_reg_16bit(lcd_dev.setx_cmd+1);
		lcd_write_data_16bit(Xpos&0XFF);			 
		lcd_write_reg_16bit(lcd_dev.sety_cmd);
		lcd_write_data_16bit(Ypos>>8);  		
		lcd_write_reg_16bit(lcd_dev.sety_cmd+1);
		lcd_write_data_16bit(Ypos&0XFF);			
	} 
} 		 
//设置LCD的自动扫描方向(对RGB屏无效)
//注意:其他函数可能会受到此函数设置的影响(尤其是9341),
//所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//dir:0~7,代表8个方向(具体定义见lcd.h)
//9341/5310/5510/1963等IC已经实际测试	   	   
void lcd_scan_dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	u16 temp;  
	if((lcd_dev.dir==1&&lcd_dev.id!=0X1963)||(lcd_dev.dir==0&&lcd_dev.id==0X1963))//横屏时，对1963不改变扫描方向！竖屏时1963改变方向
	{			   
		switch(dir)//方向转换
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	} 
	if(lcd_dev.id==0x9341||lcd_dev.id==0X5310||lcd_dev.id==0X5510||lcd_dev.id==0X1963)//9341/5310/5510/1963,特殊处理
	{
		switch(dir)
		{
			case L2R_U2D://从左到右,从上到下
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://从左到右,从下到上
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://从右到左,从上到下
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://从右到左,从下到上
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://从上到下,从左到右
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://从上到下,从右到左
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://从下到上,从左到右
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://从下到上,从右到左
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		
		if(lcd_dev.id==0X5510)
		{
			dirreg=0X3600;
		}
		else
		{
			dirreg=0X36;
		}
		
 		if((lcd_dev.id!=0X5310)&&(lcd_dev.id!=0X5510)&&(lcd_dev.id!=0X1963))
		{
			regval|=0X08;//5310/5510/1963不需要BGR   
 		}
		
 		lcd_write_reg_data(dirreg,regval);
		
		if(lcd_dev.id!=0X1963)//1963不做坐标处理
		{
			if(regval&0X20)
			{
				if(lcd_dev.width<lcd_dev.height)//交换X,Y
				{
					temp=lcd_dev.width;
					lcd_dev.width=lcd_dev.height;
					lcd_dev.height=temp;
				}
			}
			else  
			{
				if(lcd_dev.width>lcd_dev.height)//交换X,Y
				{
					temp=lcd_dev.width;
					lcd_dev.width=lcd_dev.height;
					lcd_dev.height=temp;
				}
			}  
		}
		if(lcd_dev.id==0X5510)
		{
			lcd_write_reg_16bit(lcd_dev.setx_cmd);lcd_write_data_16bit(0); 
			lcd_write_reg_16bit(lcd_dev.setx_cmd+1);lcd_write_data_16bit(0); 
			lcd_write_reg_16bit(lcd_dev.setx_cmd+2);lcd_write_data_16bit((lcd_dev.width-1)>>8); 
			lcd_write_reg_16bit(lcd_dev.setx_cmd+3);lcd_write_data_16bit((lcd_dev.width-1)&0XFF); 
			lcd_write_reg_16bit(lcd_dev.sety_cmd);lcd_write_data_16bit(0); 
			lcd_write_reg_16bit(lcd_dev.sety_cmd+1);lcd_write_data_16bit(0); 
			lcd_write_reg_16bit(lcd_dev.sety_cmd+2);lcd_write_data_16bit((lcd_dev.height-1)>>8); 
			lcd_write_reg_16bit(lcd_dev.sety_cmd+3);lcd_write_data_16bit((lcd_dev.height-1)&0XFF);
		}
		else
		{
			lcd_write_reg_16bit(lcd_dev.setx_cmd); 
			lcd_write_data_16bit(0);lcd_write_data_16bit(0);
			lcd_write_data_16bit((lcd_dev.width-1)>>8);lcd_write_data_16bit((lcd_dev.width-1)&0XFF);
			lcd_write_reg_16bit(lcd_dev.sety_cmd); 
			lcd_write_data_16bit(0);lcd_write_data_16bit(0);
			lcd_write_data_16bit((lcd_dev.height-1)>>8);lcd_write_data_16bit((lcd_dev.height-1)&0XFF);  
		}
  	} 
}     
//画点
//x,y:坐标
//point_color:此点的颜色
void lcd_draw_point(u16 x,u16 y)
{ 
	lcd_set_cursor(x,y);		//设置光标位置 
	lcd_write_ram_prepare();	//开始写入GRAM
	MOWER_LCD->lcd_ram=point_color;  
}
//快速画点
//x,y:坐标
//color:颜色
void lcd_fast_draw_point(u16 x,u16 y,u32 color)
{	   
	if(lcd_dev.id == 0X5510)
	{
		lcd_write_reg_16bit(lcd_dev.setx_cmd);
		lcd_write_data_16bit(x >> 8);  
		lcd_write_reg_16bit(lcd_dev.setx_cmd+1);
		lcd_write_data_16bit(x & 0XFF);	  
		lcd_write_reg_16bit(lcd_dev.sety_cmd);
		lcd_write_data_16bit(y >> 8);  
		lcd_write_reg_16bit(lcd_dev.sety_cmd+1);
		lcd_write_data_16bit(y & 0XFF); 
	}
		 
	MOWER_LCD->lcd_reg=lcd_dev.wram_cmd; 
	MOWER_LCD->lcd_ram=color; 
}	 
//SSD1963 背光设置
//pwm:背光等级,0~100.越大越亮.
void lcd_ssd_back_light_set(u8 pwm)
{	
	lcd_write_reg_16bit(0xBE);	//配置PWM输出
	lcd_write_data_16bit(0x05);	//1设置PWM频率
	lcd_write_data_16bit(pwm*2.55);//2设置PWM占空比
	lcd_write_data_16bit(0x01);	//3设置C
	lcd_write_data_16bit(0xFF);	//4设置D
	lcd_write_data_16bit(0x00);	//5设置E
	lcd_write_data_16bit(0x00);	//6设置F
}

//设置LCD显示方向
//dir:0,竖屏；1,横屏
void lcd_display_dir(u8 dir)
{
	lcd_dev.dir=dir;		//竖屏/横屏 
	if(dir==0)			//竖屏
	{
		lcd_dev.width=240;
		lcd_dev.height=320;
		if(lcd_dev.id==0X9341||lcd_dev.id==0X5310)
		{
			lcd_dev.wram_cmd=0X2C;
	 		lcd_dev.setx_cmd=0X2A;
			lcd_dev.sety_cmd=0X2B;  	 
			if(lcd_dev.id==0X5310)
			{
				lcd_dev.width=320;
				lcd_dev.height=480;
			}
		}else if(lcd_dev.id==0x5510)
		{
			lcd_dev.wram_cmd=0X2C00;
	 		lcd_dev.setx_cmd=0X2A00;
			lcd_dev.sety_cmd=0X2B00; 
			lcd_dev.width=480;
			lcd_dev.height=800;
		}else if(lcd_dev.id==0X1963)
		{
			lcd_dev.wram_cmd=0X2C;	//设置写入GRAM的指令 
			lcd_dev.setx_cmd=0X2B;	//设置写X坐标指令
			lcd_dev.sety_cmd=0X2A;	//设置写Y坐标指令
			lcd_dev.width=480;		//设置宽度480
			lcd_dev.height=800;		//设置高度800  
		}
	}else 				//横屏
	{	  				 
		lcd_dev.width=320;
		lcd_dev.height=240;
		if(lcd_dev.id==0X9341||lcd_dev.id==0X5310)
		{
			lcd_dev.wram_cmd=0X2C;
	 		lcd_dev.setx_cmd=0X2A;
			lcd_dev.sety_cmd=0X2B;  	 
		}
		else if(lcd_dev.id==0x5510)
		{
			lcd_dev.wram_cmd=0X2C00;
	 		lcd_dev.setx_cmd=0X2A00;
			lcd_dev.sety_cmd=0X2B00; 
			lcd_dev.width=800;
			lcd_dev.height=480;
		}
		else if(lcd_dev.id==0X1963)
		{
			lcd_dev.wram_cmd=0X2C;	//设置写入GRAM的指令 
			lcd_dev.setx_cmd=0X2A;	//设置写X坐标指令
			lcd_dev.sety_cmd=0X2B;	//设置写Y坐标指令
			lcd_dev.width=800;		//设置宽度800
			lcd_dev.height=480;		//设置高度480  
		}
		
		if(lcd_dev.id==0X5310)
		{ 	 
			lcd_dev.width=480;
			lcd_dev.height=320; 			
		}
	} 
	lcd_scan_dir(DFT_SCAN_DIR);	//默认扫描方向
}	 
//设置窗口(对RGB屏无效),并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height. 
void lcd_set_window(u16 sx,u16 sy,u16 width,u16 height)
{     
	u16 twidth,theight;
	twidth=sx+width-1;
	theight=sy+height-1;

	if(lcd_dev.id==0X5510)
	{
		lcd_write_reg_16bit(lcd_dev.setx_cmd);lcd_write_data_16bit(sx>>8);  
		lcd_write_reg_16bit(lcd_dev.setx_cmd+1);lcd_write_data_16bit(sx&0XFF);	  
		lcd_write_reg_16bit(lcd_dev.setx_cmd+2);lcd_write_data_16bit(twidth>>8);   
		lcd_write_reg_16bit(lcd_dev.setx_cmd+3);lcd_write_data_16bit(twidth&0XFF);   
		lcd_write_reg_16bit(lcd_dev.sety_cmd);lcd_write_data_16bit(sy>>8);   
		lcd_write_reg_16bit(lcd_dev.sety_cmd+1);lcd_write_data_16bit(sy&0XFF);  
		lcd_write_reg_16bit(lcd_dev.sety_cmd+2);lcd_write_data_16bit(theight>>8);   
		lcd_write_reg_16bit(lcd_dev.sety_cmd+3);lcd_write_data_16bit(theight&0XFF);  
	} 
}
//初始化lcd
//该初始化函数可以初始化各种型号的LCD(详见本.c文件最前面的描述)
void lcd_init(void)
{ 	 
	GPIO_InitTypeDef GPIO_InitData;
#if 0	
	RCC->AHB1ENR|=1<<1;			//使能PORTB时钟 
	RCC->AHB1ENR|=3<<3;    		//使能PD,PE   
	RCC->AHB3ENR|=1<<0;     	//使能FMC时钟  

	GPIO_InitData.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitData.GPIO_OType=GPIO_OType_PP;
	GPIO_InitData.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitData.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitData.GPIO_Pin=GPIO_Pin_5;
	GPIO_Init(GPIOB,&GPIO_InitData);
	
	//GPIO_Set(GPIOB,PIN5,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);							//PB5 推挽输出,控制背光

	GPIO_InitData.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitData.GPIO_OType=GPIO_OType_PP;
	GPIO_InitData.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitData.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitData.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOD,&GPIO_InitData);
	//GPIO_Set(GPIOD,(3<<0)|(3<<4)|(15<<7)|(7<<13),GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);	//PD0,1,4,5,7,8,9,10,13,14,15 AF OUT
	GPIO_PinAFConfig(GPIOD,0,12);	//PD0,AF12
	GPIO_PinAFConfig(GPIOD,1,12);	//PD1,AF12
	GPIO_PinAFConfig(GPIOD,4,12);	//PD4,AF12
	GPIO_PinAFConfig(GPIOD,5,12);	//PD5,AF12 
	GPIO_PinAFConfig(GPIOD,7,12);	//PD7,AF12
	GPIO_PinAFConfig(GPIOD,8,12);	//PD8,AF12
	GPIO_PinAFConfig(GPIOD,9,12);	//PD9,AF12
	GPIO_PinAFConfig(GPIOD,10,12);	//PD10,AF12 
	GPIO_PinAFConfig(GPIOD,13,12);	//PD13,AF12 
	GPIO_PinAFConfig(GPIOD,14,12);	//PD14,AF12
	GPIO_PinAFConfig(GPIOD,15,12);	//PD15,AF12


	GPIO_InitData.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitData.GPIO_OType=GPIO_OType_PP;
	GPIO_InitData.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitData.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitData.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOE,&GPIO_InitData);
	//GPIO_Set(GPIOE,(0X1FF<<7),GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);						//PE7~15,AF OUT
	GPIO_PinAFConfig(GPIOE,7,12);	//PE7,AF12
	GPIO_PinAFConfig(GPIOE,8,12);	//PE8,AF12
	GPIO_PinAFConfig(GPIOE,9,12);	//PE9,AF12
	GPIO_PinAFConfig(GPIOE,10,12);	//PE10,AF12
	GPIO_PinAFConfig(GPIOE,11,12);	//PE11,AF12
	GPIO_PinAFConfig(GPIOE,12,12);	//PE12,AF12
	GPIO_PinAFConfig(GPIOE,13,12);	//PE13,AF12
	GPIO_PinAFConfig(GPIOE,14,12);	//PE14,AF12
	GPIO_PinAFConfig(GPIOE,15,12);	//PE15,AF12
#endif	 
	 
	//寄存器清零
	//bank1有NE1~4,每一个有一个BCR+TCR，所以总共八个寄存器。
	//这里我们使用NE1 ，也就对应BTCR[0],[1]。				    
	FMC_Bank1->BTCR[0]=0X00000000;
	FMC_Bank1->BTCR[1]=0X00000000;
	FMC_Bank1E->BWTR[0]=0X00000000;
	//操作BCR寄存器	使用异步模式
	FMC_Bank1->BTCR[0]|=1<<12;		//存储器写使能
	FMC_Bank1->BTCR[0]|=1<<14;		//读写使用不同的时序
	FMC_Bank1->BTCR[0]|=1<<4; 		//存储器数据宽度为16bit 	    
	//操作BTR寄存器	
	//读时序控制寄存器 							    
	FMC_Bank1->BTCR[1]|=0<<28;		//模式A 	 						  	 
	FMC_Bank1->BTCR[1]|=0XF<<0; 	//地址建立时间(ADDSET)为15个HCLK 1/192M=5.2ns*15=78ns	
	//因为液晶驱动IC的读数据的时候，速度不能太快,尤其是个别奇葩芯片。
	FMC_Bank1->BTCR[1]|=70<<8;  	//数据保存时间(DATAST)为60个HCLK	=5.2*70=360ns
	//写时序控制寄存器  
	FMC_Bank1E->BWTR[0]|=0<<28; 	//模式A 	 							    
	FMC_Bank1E->BWTR[0]|=15<<0;		//地址建立时间(ADDSET)为15个HCLK=78ns
	//10个HCLK（HCLK=192M）,某些液晶驱动IC的写信号脉宽，最少也得50ns。  	 
	FMC_Bank1E->BWTR[0]|=15<<8; 	//数据保存时间(DATAST)为5.2ns*15个HCLK=78ns
	//使能BANK1,区域1
	FMC_Bank1->BTCR[0]|=1<<0;		//使能BANK1，区域1	    
			 
	//delay_ms(50); // delay 50 ms 
	hw_ms_delay(5);//rt_thread_delay(5);
	
	//尝试9341 ID的读取		
	lcd_write_reg_16bit(0XD3);				   
	lcd_dev.id=lcd_read_data_16bit();	//dummy read 	
	lcd_dev.id=lcd_read_data_16bit();	//读到0X00
	lcd_dev.id=lcd_read_data_16bit();   	//读取93								   
	lcd_dev.id<<=8;
	lcd_dev.id|=lcd_read_data_16bit();  	//读取41 	   			   
	if(lcd_dev.id!=0X9341)		//非9341,尝试看看是不是NT35310
	{	 
		lcd_write_reg_16bit(0XD4);				   
		lcd_dev.id = lcd_read_data_16bit();//dummy read  
		lcd_dev.id = lcd_read_data_16bit();//读回0X01	 
		lcd_dev.id = lcd_read_data_16bit();//读回0X53	
		lcd_dev.id <<= 8;	 
		lcd_dev.id |= lcd_read_data_16bit();	//这里读回0X10	
		
		if(lcd_dev.id != 0X5310)		//也不是NT35310,尝试看看是不是NT35510
		{
			lcd_write_reg_16bit(0XDA00);	
			lcd_dev.id=lcd_read_data_16bit();		//读回0X00	 
			lcd_write_reg_16bit(0XDB00);	
			lcd_dev.id=lcd_read_data_16bit();		//读回0X80
			lcd_dev.id<<=8;	
			lcd_write_reg_16bit(0XDC00);	
			lcd_dev.id|=lcd_read_data_16bit();		//读回0X00	
			
			if(lcd_dev.id == 0x8000)
			{
				/* NT35510读回的ID是8000H,为方便区分,我们强制设置为5510 */
				lcd_dev.id=0x5510;
			}

			if(lcd_dev.id!=0X5510)			//也不是NT5510,尝试看看是不是SSD1963
			{
				lcd_write_reg_16bit(0XA1);
				lcd_dev.id=lcd_read_data_16bit();
				lcd_dev.id=lcd_read_data_16bit();	//读回0X57
				lcd_dev.id<<=8;	 
				lcd_dev.id|=lcd_read_data_16bit();	//读回0X61	
				if(lcd_dev.id==0X5761)lcd_dev.id=0X1963;//SSD1963读回的ID是5761H,为方便区分,我们强制设置为1963
			} 
		}
	}   
	
	rt_kprintf(" MOWER_LCD ID:%x\r\n",lcd_dev.id); //打印LCD ID
	
	if(lcd_dev.id == 0x5510)
	{
		lcd_write_reg_data(0xF000,0x55);
		lcd_write_reg_data(0xF001,0xAA);
		lcd_write_reg_data(0xF002,0x52);
		lcd_write_reg_data(0xF003,0x08);
		lcd_write_reg_data(0xF004,0x01);
		//AVDD Set AVDD 5.2V
		lcd_write_reg_data(0xB000,0x0D);
		lcd_write_reg_data(0xB001,0x0D);
		lcd_write_reg_data(0xB002,0x0D);
		//AVDD ratio
		lcd_write_reg_data(0xB600,0x34);
		lcd_write_reg_data(0xB601,0x34);
		lcd_write_reg_data(0xB602,0x34);
		//AVEE -5.2V
		lcd_write_reg_data(0xB100,0x0D);
		lcd_write_reg_data(0xB101,0x0D);
		lcd_write_reg_data(0xB102,0x0D);
		//AVEE ratio
		lcd_write_reg_data(0xB700,0x34);
		lcd_write_reg_data(0xB701,0x34);
		lcd_write_reg_data(0xB702,0x34);
		//VCL -2.5V
		lcd_write_reg_data(0xB200,0x00);
		lcd_write_reg_data(0xB201,0x00);
		lcd_write_reg_data(0xB202,0x00);
		//VCL ratio
		lcd_write_reg_data(0xB800,0x24);
		lcd_write_reg_data(0xB801,0x24);
		lcd_write_reg_data(0xB802,0x24);
		//VGH 15V (Free pump)
		lcd_write_reg_data(0xBF00,0x01);
		lcd_write_reg_data(0xB300,0x0F);
		lcd_write_reg_data(0xB301,0x0F);
		lcd_write_reg_data(0xB302,0x0F);
		//VGH ratio
		lcd_write_reg_data(0xB900,0x34);
		lcd_write_reg_data(0xB901,0x34);
		lcd_write_reg_data(0xB902,0x34);
		//VGL_REG -10V
		lcd_write_reg_data(0xB500,0x08);
		lcd_write_reg_data(0xB501,0x08);
		lcd_write_reg_data(0xB502,0x08);
		lcd_write_reg_data(0xC200,0x03);
		//VGLX ratio
		lcd_write_reg_data(0xBA00,0x24);
		lcd_write_reg_data(0xBA01,0x24);
		lcd_write_reg_data(0xBA02,0x24);
		//VGMP/VGSP 4.5V/0V
		lcd_write_reg_data(0xBC00,0x00);
		lcd_write_reg_data(0xBC01,0x78);
		lcd_write_reg_data(0xBC02,0x00);
		//VGMN/VGSN -4.5V/0V
		lcd_write_reg_data(0xBD00,0x00);
		lcd_write_reg_data(0xBD01,0x78);
		lcd_write_reg_data(0xBD02,0x00);
		//VCOM
		lcd_write_reg_data(0xBE00,0x00);
		lcd_write_reg_data(0xBE01,0x64);
		//Gamma Setting
		lcd_write_reg_data(0xD100,0x00);
		lcd_write_reg_data(0xD101,0x33);
		lcd_write_reg_data(0xD102,0x00);
		lcd_write_reg_data(0xD103,0x34);
		lcd_write_reg_data(0xD104,0x00);
		lcd_write_reg_data(0xD105,0x3A);
		lcd_write_reg_data(0xD106,0x00);
		lcd_write_reg_data(0xD107,0x4A);
		lcd_write_reg_data(0xD108,0x00);
		lcd_write_reg_data(0xD109,0x5C);
		lcd_write_reg_data(0xD10A,0x00);
		lcd_write_reg_data(0xD10B,0x81);
		lcd_write_reg_data(0xD10C,0x00);
		lcd_write_reg_data(0xD10D,0xA6);
		lcd_write_reg_data(0xD10E,0x00);
		lcd_write_reg_data(0xD10F,0xE5);
		lcd_write_reg_data(0xD110,0x01);
		lcd_write_reg_data(0xD111,0x13);
		lcd_write_reg_data(0xD112,0x01);
		lcd_write_reg_data(0xD113,0x54);
		lcd_write_reg_data(0xD114,0x01);
		lcd_write_reg_data(0xD115,0x82);
		lcd_write_reg_data(0xD116,0x01);
		lcd_write_reg_data(0xD117,0xCA);
		lcd_write_reg_data(0xD118,0x02);
		lcd_write_reg_data(0xD119,0x00);
		lcd_write_reg_data(0xD11A,0x02);
		lcd_write_reg_data(0xD11B,0x01);
		lcd_write_reg_data(0xD11C,0x02);
		lcd_write_reg_data(0xD11D,0x34);
		lcd_write_reg_data(0xD11E,0x02);
		lcd_write_reg_data(0xD11F,0x67);
		lcd_write_reg_data(0xD120,0x02);
		lcd_write_reg_data(0xD121,0x84);
		lcd_write_reg_data(0xD122,0x02);
		lcd_write_reg_data(0xD123,0xA4);
		lcd_write_reg_data(0xD124,0x02);
		lcd_write_reg_data(0xD125,0xB7);
		lcd_write_reg_data(0xD126,0x02);
		lcd_write_reg_data(0xD127,0xCF);
		lcd_write_reg_data(0xD128,0x02);
		lcd_write_reg_data(0xD129,0xDE);
		lcd_write_reg_data(0xD12A,0x02);
		lcd_write_reg_data(0xD12B,0xF2);
		lcd_write_reg_data(0xD12C,0x02);
		lcd_write_reg_data(0xD12D,0xFE);
		lcd_write_reg_data(0xD12E,0x03);
		lcd_write_reg_data(0xD12F,0x10);
		lcd_write_reg_data(0xD130,0x03);
		lcd_write_reg_data(0xD131,0x33);
		lcd_write_reg_data(0xD132,0x03);
		lcd_write_reg_data(0xD133,0x6D);
		lcd_write_reg_data(0xD200,0x00);
		lcd_write_reg_data(0xD201,0x33);
		lcd_write_reg_data(0xD202,0x00);
		lcd_write_reg_data(0xD203,0x34);
		lcd_write_reg_data(0xD204,0x00);
		lcd_write_reg_data(0xD205,0x3A);
		lcd_write_reg_data(0xD206,0x00);
		lcd_write_reg_data(0xD207,0x4A);
		lcd_write_reg_data(0xD208,0x00);
		lcd_write_reg_data(0xD209,0x5C);
		lcd_write_reg_data(0xD20A,0x00);

		lcd_write_reg_data(0xD20B,0x81);
		lcd_write_reg_data(0xD20C,0x00);
		lcd_write_reg_data(0xD20D,0xA6);
		lcd_write_reg_data(0xD20E,0x00);
		lcd_write_reg_data(0xD20F,0xE5);
		lcd_write_reg_data(0xD210,0x01);
		lcd_write_reg_data(0xD211,0x13);
		lcd_write_reg_data(0xD212,0x01);
		lcd_write_reg_data(0xD213,0x54);
		lcd_write_reg_data(0xD214,0x01);
		lcd_write_reg_data(0xD215,0x82);
		lcd_write_reg_data(0xD216,0x01);
		lcd_write_reg_data(0xD217,0xCA);
		lcd_write_reg_data(0xD218,0x02);
		lcd_write_reg_data(0xD219,0x00);
		lcd_write_reg_data(0xD21A,0x02);
		lcd_write_reg_data(0xD21B,0x01);
		lcd_write_reg_data(0xD21C,0x02);
		lcd_write_reg_data(0xD21D,0x34);
		lcd_write_reg_data(0xD21E,0x02);
		lcd_write_reg_data(0xD21F,0x67);
		lcd_write_reg_data(0xD220,0x02);
		lcd_write_reg_data(0xD221,0x84);
		lcd_write_reg_data(0xD222,0x02);
		lcd_write_reg_data(0xD223,0xA4);
		lcd_write_reg_data(0xD224,0x02);
		lcd_write_reg_data(0xD225,0xB7);
		lcd_write_reg_data(0xD226,0x02);
		lcd_write_reg_data(0xD227,0xCF);
		lcd_write_reg_data(0xD228,0x02);
		lcd_write_reg_data(0xD229,0xDE);
		lcd_write_reg_data(0xD22A,0x02);
		lcd_write_reg_data(0xD22B,0xF2);
		lcd_write_reg_data(0xD22C,0x02);
		lcd_write_reg_data(0xD22D,0xFE);
		lcd_write_reg_data(0xD22E,0x03);
		lcd_write_reg_data(0xD22F,0x10);
		lcd_write_reg_data(0xD230,0x03);
		lcd_write_reg_data(0xD231,0x33);
		lcd_write_reg_data(0xD232,0x03);
		lcd_write_reg_data(0xD233,0x6D);
		lcd_write_reg_data(0xD300,0x00);
		lcd_write_reg_data(0xD301,0x33);
		lcd_write_reg_data(0xD302,0x00);
		lcd_write_reg_data(0xD303,0x34);
		lcd_write_reg_data(0xD304,0x00);
		lcd_write_reg_data(0xD305,0x3A);
		lcd_write_reg_data(0xD306,0x00);
		lcd_write_reg_data(0xD307,0x4A);
		lcd_write_reg_data(0xD308,0x00);
		lcd_write_reg_data(0xD309,0x5C);
		lcd_write_reg_data(0xD30A,0x00);

		lcd_write_reg_data(0xD30B,0x81);
		lcd_write_reg_data(0xD30C,0x00);
		lcd_write_reg_data(0xD30D,0xA6);
		lcd_write_reg_data(0xD30E,0x00);
		lcd_write_reg_data(0xD30F,0xE5);
		lcd_write_reg_data(0xD310,0x01);
		lcd_write_reg_data(0xD311,0x13);
		lcd_write_reg_data(0xD312,0x01);
		lcd_write_reg_data(0xD313,0x54);
		lcd_write_reg_data(0xD314,0x01);
		lcd_write_reg_data(0xD315,0x82);
		lcd_write_reg_data(0xD316,0x01);
		lcd_write_reg_data(0xD317,0xCA);
		lcd_write_reg_data(0xD318,0x02);
		lcd_write_reg_data(0xD319,0x00);
		lcd_write_reg_data(0xD31A,0x02);
		lcd_write_reg_data(0xD31B,0x01);
		lcd_write_reg_data(0xD31C,0x02);
		lcd_write_reg_data(0xD31D,0x34);
		lcd_write_reg_data(0xD31E,0x02);
		lcd_write_reg_data(0xD31F,0x67);
		lcd_write_reg_data(0xD320,0x02);
		lcd_write_reg_data(0xD321,0x84);
		lcd_write_reg_data(0xD322,0x02);
		lcd_write_reg_data(0xD323,0xA4);
		lcd_write_reg_data(0xD324,0x02);
		lcd_write_reg_data(0xD325,0xB7);
		lcd_write_reg_data(0xD326,0x02);
		lcd_write_reg_data(0xD327,0xCF);
		lcd_write_reg_data(0xD328,0x02);
		lcd_write_reg_data(0xD329,0xDE);
		lcd_write_reg_data(0xD32A,0x02);
		lcd_write_reg_data(0xD32B,0xF2);
		lcd_write_reg_data(0xD32C,0x02);
		lcd_write_reg_data(0xD32D,0xFE);
		lcd_write_reg_data(0xD32E,0x03);
		lcd_write_reg_data(0xD32F,0x10);
		lcd_write_reg_data(0xD330,0x03);
		lcd_write_reg_data(0xD331,0x33);
		lcd_write_reg_data(0xD332,0x03);
		lcd_write_reg_data(0xD333,0x6D);
		lcd_write_reg_data(0xD400,0x00);
		lcd_write_reg_data(0xD401,0x33);
		lcd_write_reg_data(0xD402,0x00);
		lcd_write_reg_data(0xD403,0x34);
		lcd_write_reg_data(0xD404,0x00);
		lcd_write_reg_data(0xD405,0x3A);
		lcd_write_reg_data(0xD406,0x00);
		lcd_write_reg_data(0xD407,0x4A);
		lcd_write_reg_data(0xD408,0x00);
		lcd_write_reg_data(0xD409,0x5C);
		lcd_write_reg_data(0xD40A,0x00);
		lcd_write_reg_data(0xD40B,0x81);

		lcd_write_reg_data(0xD40C,0x00);
		lcd_write_reg_data(0xD40D,0xA6);
		lcd_write_reg_data(0xD40E,0x00);
		lcd_write_reg_data(0xD40F,0xE5);
		lcd_write_reg_data(0xD410,0x01);
		lcd_write_reg_data(0xD411,0x13);
		lcd_write_reg_data(0xD412,0x01);
		lcd_write_reg_data(0xD413,0x54);
		lcd_write_reg_data(0xD414,0x01);
		lcd_write_reg_data(0xD415,0x82);
		lcd_write_reg_data(0xD416,0x01);
		lcd_write_reg_data(0xD417,0xCA);
		lcd_write_reg_data(0xD418,0x02);
		lcd_write_reg_data(0xD419,0x00);
		lcd_write_reg_data(0xD41A,0x02);
		lcd_write_reg_data(0xD41B,0x01);
		lcd_write_reg_data(0xD41C,0x02);
		lcd_write_reg_data(0xD41D,0x34);
		lcd_write_reg_data(0xD41E,0x02);
		lcd_write_reg_data(0xD41F,0x67);
		lcd_write_reg_data(0xD420,0x02);
		lcd_write_reg_data(0xD421,0x84);
		lcd_write_reg_data(0xD422,0x02);
		lcd_write_reg_data(0xD423,0xA4);
		lcd_write_reg_data(0xD424,0x02);
		lcd_write_reg_data(0xD425,0xB7);
		lcd_write_reg_data(0xD426,0x02);
		lcd_write_reg_data(0xD427,0xCF);
		lcd_write_reg_data(0xD428,0x02);
		lcd_write_reg_data(0xD429,0xDE);
		lcd_write_reg_data(0xD42A,0x02);
		lcd_write_reg_data(0xD42B,0xF2);
		lcd_write_reg_data(0xD42C,0x02);
		lcd_write_reg_data(0xD42D,0xFE);
		lcd_write_reg_data(0xD42E,0x03);
		lcd_write_reg_data(0xD42F,0x10);
		lcd_write_reg_data(0xD430,0x03);
		lcd_write_reg_data(0xD431,0x33);
		lcd_write_reg_data(0xD432,0x03);
		lcd_write_reg_data(0xD433,0x6D);
		lcd_write_reg_data(0xD500,0x00);
		lcd_write_reg_data(0xD501,0x33);
		lcd_write_reg_data(0xD502,0x00);
		lcd_write_reg_data(0xD503,0x34);
		lcd_write_reg_data(0xD504,0x00);
		lcd_write_reg_data(0xD505,0x3A);
		lcd_write_reg_data(0xD506,0x00);
		lcd_write_reg_data(0xD507,0x4A);
		lcd_write_reg_data(0xD508,0x00);
		lcd_write_reg_data(0xD509,0x5C);
		lcd_write_reg_data(0xD50A,0x00);
		lcd_write_reg_data(0xD50B,0x81);

		lcd_write_reg_data(0xD50C,0x00);
		lcd_write_reg_data(0xD50D,0xA6);
		lcd_write_reg_data(0xD50E,0x00);
		lcd_write_reg_data(0xD50F,0xE5);
		lcd_write_reg_data(0xD510,0x01);
		lcd_write_reg_data(0xD511,0x13);
		lcd_write_reg_data(0xD512,0x01);
		lcd_write_reg_data(0xD513,0x54);
		lcd_write_reg_data(0xD514,0x01);
		lcd_write_reg_data(0xD515,0x82);
		lcd_write_reg_data(0xD516,0x01);
		lcd_write_reg_data(0xD517,0xCA);
		lcd_write_reg_data(0xD518,0x02);
		lcd_write_reg_data(0xD519,0x00);
		lcd_write_reg_data(0xD51A,0x02);
		lcd_write_reg_data(0xD51B,0x01);
		lcd_write_reg_data(0xD51C,0x02);
		lcd_write_reg_data(0xD51D,0x34);
		lcd_write_reg_data(0xD51E,0x02);
		lcd_write_reg_data(0xD51F,0x67);
		lcd_write_reg_data(0xD520,0x02);
		lcd_write_reg_data(0xD521,0x84);
		lcd_write_reg_data(0xD522,0x02);
		lcd_write_reg_data(0xD523,0xA4);
		lcd_write_reg_data(0xD524,0x02);
		lcd_write_reg_data(0xD525,0xB7);
		lcd_write_reg_data(0xD526,0x02);
		lcd_write_reg_data(0xD527,0xCF);
		lcd_write_reg_data(0xD528,0x02);
		lcd_write_reg_data(0xD529,0xDE);
		lcd_write_reg_data(0xD52A,0x02);
		lcd_write_reg_data(0xD52B,0xF2);
		lcd_write_reg_data(0xD52C,0x02);
		lcd_write_reg_data(0xD52D,0xFE);
		lcd_write_reg_data(0xD52E,0x03);
		lcd_write_reg_data(0xD52F,0x10);
		lcd_write_reg_data(0xD530,0x03);
		lcd_write_reg_data(0xD531,0x33);
		lcd_write_reg_data(0xD532,0x03);
		lcd_write_reg_data(0xD533,0x6D);
		lcd_write_reg_data(0xD600,0x00);
		lcd_write_reg_data(0xD601,0x33);
		lcd_write_reg_data(0xD602,0x00);
		lcd_write_reg_data(0xD603,0x34);
		lcd_write_reg_data(0xD604,0x00);
		lcd_write_reg_data(0xD605,0x3A);
		lcd_write_reg_data(0xD606,0x00);
		lcd_write_reg_data(0xD607,0x4A);
		lcd_write_reg_data(0xD608,0x00);
		lcd_write_reg_data(0xD609,0x5C);
		lcd_write_reg_data(0xD60A,0x00);
		lcd_write_reg_data(0xD60B,0x81);

		lcd_write_reg_data(0xD60C,0x00);
		lcd_write_reg_data(0xD60D,0xA6);
		lcd_write_reg_data(0xD60E,0x00);
		lcd_write_reg_data(0xD60F,0xE5);
		lcd_write_reg_data(0xD610,0x01);
		lcd_write_reg_data(0xD611,0x13);
		lcd_write_reg_data(0xD612,0x01);
		lcd_write_reg_data(0xD613,0x54);
		lcd_write_reg_data(0xD614,0x01);
		lcd_write_reg_data(0xD615,0x82);
		lcd_write_reg_data(0xD616,0x01);
		lcd_write_reg_data(0xD617,0xCA);
		lcd_write_reg_data(0xD618,0x02);
		lcd_write_reg_data(0xD619,0x00);
		lcd_write_reg_data(0xD61A,0x02);
		lcd_write_reg_data(0xD61B,0x01);
		lcd_write_reg_data(0xD61C,0x02);
		lcd_write_reg_data(0xD61D,0x34);
		lcd_write_reg_data(0xD61E,0x02);
		lcd_write_reg_data(0xD61F,0x67);
		lcd_write_reg_data(0xD620,0x02);
		lcd_write_reg_data(0xD621,0x84);
		lcd_write_reg_data(0xD622,0x02);
		lcd_write_reg_data(0xD623,0xA4);
		lcd_write_reg_data(0xD624,0x02);
		lcd_write_reg_data(0xD625,0xB7);
		lcd_write_reg_data(0xD626,0x02);
		lcd_write_reg_data(0xD627,0xCF);
		lcd_write_reg_data(0xD628,0x02);
		lcd_write_reg_data(0xD629,0xDE);
		lcd_write_reg_data(0xD62A,0x02);
		lcd_write_reg_data(0xD62B,0xF2);
		lcd_write_reg_data(0xD62C,0x02);
		lcd_write_reg_data(0xD62D,0xFE);
		lcd_write_reg_data(0xD62E,0x03);
		lcd_write_reg_data(0xD62F,0x10);
		lcd_write_reg_data(0xD630,0x03);
		lcd_write_reg_data(0xD631,0x33);
		lcd_write_reg_data(0xD632,0x03);
		lcd_write_reg_data(0xD633,0x6D);
		//LV2 Page 0 enable
		lcd_write_reg_data(0xF000,0x55);
		lcd_write_reg_data(0xF001,0xAA);
		lcd_write_reg_data(0xF002,0x52);
		lcd_write_reg_data(0xF003,0x08);
		lcd_write_reg_data(0xF004,0x00);
		//Display control
		lcd_write_reg_data(0xB100, 0xCC);
		lcd_write_reg_data(0xB101, 0x00);
		//Source hold time
		lcd_write_reg_data(0xB600,0x05);
		//Gate EQ control
		lcd_write_reg_data(0xB700,0x70);
		lcd_write_reg_data(0xB701,0x70);
		//Source EQ control (Mode 2)
		lcd_write_reg_data(0xB800,0x01);
		lcd_write_reg_data(0xB801,0x03);
		lcd_write_reg_data(0xB802,0x03);
		lcd_write_reg_data(0xB803,0x03);
		//Inversion mode (2-dot)
		lcd_write_reg_data(0xBC00,0x02);
		lcd_write_reg_data(0xBC01,0x00);
		lcd_write_reg_data(0xBC02,0x00);
		//Timing control 4H w/ 4-delay
		lcd_write_reg_data(0xC900,0xD0);
		lcd_write_reg_data(0xC901,0x02);
		lcd_write_reg_data(0xC902,0x50);
		lcd_write_reg_data(0xC903,0x50);
		lcd_write_reg_data(0xC904,0x50);
		lcd_write_reg_data(0x3500,0x00);
		lcd_write_reg_data(0x3A00,0x55);  //16-bit/pixel
		lcd_write_reg_16bit(0x1100);
		//delay_us(120);
		systick_us_delay(120);
		lcd_write_reg_16bit(0x2900);  
	}
	
		 
	//初始化完成以后,提速
	if(lcd_dev.id==0X9341||lcd_dev.id==0X5310||lcd_dev.id==0X5510||lcd_dev.id==0X1963)//如果是这几个IC,则设置WR时序为最快
	{
		//重新配置写时序控制寄存器的时序   	 							    
		FMC_Bank1E->BWTR[0]&=~(0XF<<0);	//地址建立时间(ADDSET)清零 	 
		FMC_Bank1E->BWTR[0]&=~(0XF<<8);	//数据保存时间清零
		FMC_Bank1E->BWTR[0]|=4<<0;	 	//地址建立时间(ADDSET)为4个HCLK =21ns  	 
		FMC_Bank1E->BWTR[0]|=4<<8; 		//数据保存时间(DATAST)为5.2ns*4个HCLK=21ns
	} 
	lcd_display_dir(0);		//默认为竖屏
	LCD_BACK_LIGHT;				//点亮背光
	/* 屏幕背景色 */	
	lcd_clear(BLUE);
}  


//清屏函数
//color:要清屏的填充色
void lcd_clear(u32 color)
{
	u32 index=0;      
	u32 totalpoint=lcd_dev.width; 
	totalpoint*=lcd_dev.height; 			//得到总点数
	lcd_set_cursor(0x00,0x0000);			//设置光标位置 
	lcd_write_ram_prepare();     		//开始写入GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		MOWER_LCD->lcd_ram=color;	
	} 
}  
//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void lcd_fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
{          
	u16 i,j;
	u16 xlen=0; 
	xlen=ex-sx+1;	 
	for(i=sy;i<=ey;i++)
	{
		lcd_set_cursor(sx,i);      				//设置光标位置 
		lcd_write_ram_prepare();     			//开始写入GRAM	  
		for(j=0;j<xlen;j++)MOWER_LCD->lcd_ram=color;	//显示颜色 	    
	}  
}  
//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void lcd_color_fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j; 
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
	for(i=0;i<height;i++)
	{
		lcd_set_cursor(sx,sy+i);   	//设置光标位置 
		lcd_write_ram_prepare();     //开始写入GRAM
		for(j=0;j<width;j++)MOWER_LCD->lcd_ram=color[i*width+j];//写入数据 
	} 
}  
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void lcd_draw_line(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		lcd_draw_point(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void lcd_draw_rectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	lcd_draw_line(x1,y1,x2,y1);
	lcd_draw_line(x1,y1,x1,y2);
	lcd_draw_line(x1,y2,x2,y2);
	lcd_draw_line(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void lcd_draw_circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		lcd_draw_point(x0+a,y0-b);             //5
 		lcd_draw_point(x0+b,y0-a);             //0           
		lcd_draw_point(x0+b,y0+a);             //4               
		lcd_draw_point(x0+a,y0+b);             //6 
		lcd_draw_point(x0-a,y0+b);             //1       
 		lcd_draw_point(x0-b,y0+a);             
		lcd_draw_point(x0-a,y0-b);             //2             
  		lcd_draw_point(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24/32
//mode:叠加方式(1)还是非叠加方式(0)
void lcd_show_char(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else if(size==32)temp=asc2_3216[num][t];	//调用3216字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)lcd_fast_draw_point(x,y,point_color);
			else if(mode==0)lcd_fast_draw_point(x,y,back_color);
			temp<<=1;
			y++;
			if(y>=lcd_dev.height)return;	//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcd_dev.width)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void lcd_show_num(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				lcd_show_char(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	lcd_show_char(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void lcd_show_string_num(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)lcd_show_char(x+(size/2)*t,y,'0',size,mode&0X01);  
				else lcd_show_char(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	lcd_show_char(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小 12/16/24/32
//*p:字符串起始地址		  
void lcd_show_string(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0;
	
	x0 = x;
	width += x;
	height += y;
	
    while((*p <= '~') && (*p >= ' '))//判断是不是非法字符!
    {       
        if(x >= width)
		{
			x = x0;
			y += size;
		}
		
        if(y >= height)
			break;//退出
			
        lcd_show_char(x,y,*p,size,0);
        x += size/2;
        p++;
    }  
}








/* LCD 操作函数机构体 */
struct fmc_lcd_ops
{
	rt_err_t (*show_string)(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);
	void (*show_string_num)(void);
};


struct fmc_lcd_device
{
	T_lcd_typedef*		lcd_map_addr;
	struct rt_device	parent;
	rt_uint16_t			reg_addr;
	rt_uint16_t			reg_data;
	const struct fmc_lcd_ops 	*operations;
};

struct rt_device lcd_dev_driver;

struct fmc_lcd_device mover_lcd;
struct fmc_lcd_device mower_lcd_data = 
{
	MOWER_LCD,
	RT_NULL,
	RT_NULL,
	RT_NULL,
	RT_NULL,
};

rt_err_t rt_hw_lcd_register(struct fmc_lcd_device *lcd,const char *name,rt_uint32_t flag,struct fmc_lcd_device *data)
{
	struct rt_device  *device;
	
	RT_ASSERT(lcd != RT_NULL);
	
	device = &(lcd->parent);
	device->type = RT_Device_Class_Char;
	device->device_id = 5;
	device->init = lcd_nt35510_init;
	device->open = RT_NULL;
	device->close = RT_NULL;
	device->read = RT_NULL;
	device->write = RT_NULL;
	device->control = RT_NULL;
	device->user_data = data;

	return rt_device_register(device,name,flag);

	
}


void lcd_nt35510_config(void)
{	
	GPIO_InitTypeDef GPIO_InitData;
#if 0	
	RCC->AHB1ENR |= 1 << 1;			//使能PORTB时钟 
	RCC->AHB1ENR |= 3 << 3;    		//使能PD,PE   
	RCC->AHB3ENR |= 1 << 0;     	//使能FMC时钟  

	GPIO_InitData.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitData.GPIO_OType = GPIO_OType_PP;
	GPIO_InitData.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitData.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitData.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOB,&GPIO_InitData);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	
	
	GPIO_InitData.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitData.GPIO_OType = GPIO_OType_PP;
	GPIO_InitData.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitData.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitData.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5\
						| GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10\
						| GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOD,&GPIO_InitData);

	GPIO_PinAFConfig(GPIOD,0,12);	//PD0,AF12
	GPIO_PinAFConfig(GPIOD,1,12);	//PD1,AF12
	GPIO_PinAFConfig(GPIOD,4,12);	//PD4,AF12
	GPIO_PinAFConfig(GPIOD,5,12);	//PD5,AF12 
	GPIO_PinAFConfig(GPIOD,7,12);	//PD7,AF12
	GPIO_PinAFConfig(GPIOD,8,12);	//PD8,AF12
	GPIO_PinAFConfig(GPIOD,9,12);	//PD9,AF12
	GPIO_PinAFConfig(GPIOD,10,12);	//PD10,AF12 
	GPIO_PinAFConfig(GPIOD,13,12);	//PD13,AF12 
	GPIO_PinAFConfig(GPIOD,14,12);	//PD14,AF12
	GPIO_PinAFConfig(GPIOD,15,12);	//PD15,AF12

	GPIO_InitData.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitData.GPIO_OType = GPIO_OType_PP;
	GPIO_InitData.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitData.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitData.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10\
						| GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14\
						| GPIO_Pin_15;
	GPIO_Init(GPIOE,&GPIO_InitData);

	GPIO_PinAFConfig(GPIOE,7,12);	//PE7,AF12
	GPIO_PinAFConfig(GPIOE,8,12);	//PE8,AF12
	GPIO_PinAFConfig(GPIOE,9,12);	//PE9,AF12
	GPIO_PinAFConfig(GPIOE,10,12);	//PE10,AF12
	GPIO_PinAFConfig(GPIOE,11,12);	//PE11,AF12
	GPIO_PinAFConfig(GPIOE,12,12);	//PE12,AF12
	GPIO_PinAFConfig(GPIOE,13,12);	//PE13,AF12
	GPIO_PinAFConfig(GPIOE,14,12);	//PE14,AF12
	GPIO_PinAFConfig(GPIOE,15,12);	//PE15,AF12
#endif	 

#if 0
	 /*
	//寄存器清零
	//bank1有NE1~4,每一个有一个BCR+TCR，所以总共八个寄存器。
	//这里我们使用NE1 ，也就对应BTCR[0],[1]。
	*/
	FMC_Bank1->BTCR[0]=0X00000000;
	FMC_Bank1->BTCR[1]=0X00000000;
	FMC_Bank1E->BWTR[0]=0X00000000;
	/*
	//操作BCR寄存器	使用异步模式
	*/
	FMC_Bank1->BTCR[0]|=1<<12;		//存储器写使能
	FMC_Bank1->BTCR[0]|=1<<14;		//读写使用不同的时序
	FMC_Bank1->BTCR[0]|=1<<4; 		//存储器数据宽度为16bit 	 
	/*
	//操作BTR寄存器	
	//读时序控制寄存器 			
	*/
	FMC_Bank1->BTCR[1]|=0<<28;		//模式A 	 						  	 
	FMC_Bank1->BTCR[1]|=0XF<<0; 	//地址建立时间(ADDSET)为15个HCLK 1/192M=5.2ns*15=78ns	
	/*
	//因为液晶驱动IC的读数据的时候，速度不能太快,尤其是个别奇葩芯片。
	*/
	FMC_Bank1->BTCR[1]|=70<<8;  	//数据保存时间(DATAST)为60个HCLK	=5.2*70=360ns
	/*
	//写时序控制寄存器  
	*/
	FMC_Bank1E->BWTR[0]|=0<<28; 	//模式A 	 							    
	FMC_Bank1E->BWTR[0]|=15<<0;		//地址建立时间(ADDSET)为15个HCLK=78ns
	/*
	//10个HCLK（HCLK=192M）,某些液晶驱动IC的写信号脉宽，最少也得50ns。  	 
	*/
	FMC_Bank1E->BWTR[0]|=15<<8; 	//数据保存时间(DATAST)为5.2ns*15个HCLK=78ns
	/*
	//使能BANK1,区域1
	*/
	FMC_Bank1->BTCR[0]|=1<<0;		//使能BANK1，区域1	 

	/* 把lcd设备注册到设备队列里 */
	rt_hw_lcd_register(&mover_lcd,"Tft",\
						RT_DEVICE_FLAG_WRONLY | RT_DEVICE_FLAG_STREAM,&mower_lcd_data);

#endif
}


rt_err_t lcd_nt35510_init(rt_device_t lcd)
{
#if 1	
	 /*
	//寄存器清零
	//bank1有NE1~4,每一个有一个BCR+TCR，所以总共八个寄存器。
	//这里我们使用NE1 ，也就对应BTCR[0],[1]。
	*/
	FMC_Bank1->BTCR[0]=0X00000000;
	FMC_Bank1->BTCR[1]=0X00000000;
	FMC_Bank1E->BWTR[0]=0X00000000;
	/*
	//操作BCR寄存器	使用异步模式
	*/
	FMC_Bank1->BTCR[0]|=1<<12;		//存储器写使能
	FMC_Bank1->BTCR[0]|=1<<14;		//读写使用不同的时序
	FMC_Bank1->BTCR[0]|=1<<4; 		//存储器数据宽度为16bit 	 
	/*
	//操作BTR寄存器	
	//读时序控制寄存器 			
	*/
	FMC_Bank1->BTCR[1]|=0<<28;		//模式A 	 						  	 
	FMC_Bank1->BTCR[1]|=0XF<<0; 	//地址建立时间(ADDSET)为15个HCLK 1/192M=5.2ns*15=78ns	
	/*
	//因为液晶驱动IC的读数据的时候，速度不能太快,尤其是个别奇葩芯片。
	*/
	FMC_Bank1->BTCR[1]|=70<<8;  	//数据保存时间(DATAST)为60个HCLK	=5.2*70=360ns
	/*
	//写时序控制寄存器  
	*/
	FMC_Bank1E->BWTR[0]|=0<<28; 	//模式A 	 							    
	FMC_Bank1E->BWTR[0]|=15<<0;		//地址建立时间(ADDSET)为15个HCLK=78ns
	/*
	//10个HCLK（HCLK=192M）,某些液晶驱动IC的写信号脉宽，最少也得50ns。  	 
	*/
	FMC_Bank1E->BWTR[0]|=15<<8; 	//数据保存时间(DATAST)为5.2ns*15个HCLK=78ns
	/*
	//使能BANK1,区域1
	*/
	FMC_Bank1->BTCR[0]|=1<<0;		//使能BANK1，区域1	 

	/* 把lcd设备注册到设备队列里 */
	rt_hw_lcd_register(&mover_lcd,"Tft",\
						RT_DEVICE_FLAG_WRONLY | RT_DEVICE_FLAG_STREAM,&mower_lcd_data);

	systick_ms_delay(1);
#endif
	
	lcd_write_reg_16bit(0XDA00);	
	lcd_dev.id=lcd_read_data_16bit();		//读回0X00	 
	lcd_write_reg_16bit(0XDB00);	
	lcd_dev.id=lcd_read_data_16bit();		//读回0X80
	lcd_dev.id<<=8;	
	lcd_write_reg_16bit(0XDC00);	
	lcd_dev.id|=lcd_read_data_16bit();		//读回0X00	
			
	if(lcd_dev.id == 0x8000)
	{
		/* NT35510读回的ID是8000H,为方便区分,我们强制设置为5510 */
		lcd_dev.id=0x5510;
	}
	
	rt_kprintf(" MOWER_LCD ID:%x\r\n",lcd_dev.id); //打印LCD ID
	
	if(lcd_dev.id == 0x5510)
	{
		lcd_write_reg_data(0xF000,0x55);
		lcd_write_reg_data(0xF001,0xAA);
		lcd_write_reg_data(0xF002,0x52);
		lcd_write_reg_data(0xF003,0x08);
		lcd_write_reg_data(0xF004,0x01);
		//AVDD Set AVDD 5.2V
		lcd_write_reg_data(0xB000,0x0D);
		lcd_write_reg_data(0xB001,0x0D);
		lcd_write_reg_data(0xB002,0x0D);
		//AVDD ratio
		lcd_write_reg_data(0xB600,0x34);
		lcd_write_reg_data(0xB601,0x34);
		lcd_write_reg_data(0xB602,0x34);
		//AVEE -5.2V
		lcd_write_reg_data(0xB100,0x0D);
		lcd_write_reg_data(0xB101,0x0D);
		lcd_write_reg_data(0xB102,0x0D);
		//AVEE ratio
		lcd_write_reg_data(0xB700,0x34);
		lcd_write_reg_data(0xB701,0x34);
		lcd_write_reg_data(0xB702,0x34);
		//VCL -2.5V
		lcd_write_reg_data(0xB200,0x00);
		lcd_write_reg_data(0xB201,0x00);
		lcd_write_reg_data(0xB202,0x00);
		//VCL ratio
		lcd_write_reg_data(0xB800,0x24);
		lcd_write_reg_data(0xB801,0x24);
		lcd_write_reg_data(0xB802,0x24);
		//VGH 15V (Free pump)
		lcd_write_reg_data(0xBF00,0x01);
		lcd_write_reg_data(0xB300,0x0F);
		lcd_write_reg_data(0xB301,0x0F);
		lcd_write_reg_data(0xB302,0x0F);
		//VGH ratio
		lcd_write_reg_data(0xB900,0x34);
		lcd_write_reg_data(0xB901,0x34);
		lcd_write_reg_data(0xB902,0x34);
		//VGL_REG -10V
		lcd_write_reg_data(0xB500,0x08);
		lcd_write_reg_data(0xB501,0x08);
		lcd_write_reg_data(0xB502,0x08);
		lcd_write_reg_data(0xC200,0x03);
		//VGLX ratio
		lcd_write_reg_data(0xBA00,0x24);
		lcd_write_reg_data(0xBA01,0x24);
		lcd_write_reg_data(0xBA02,0x24);
		//VGMP/VGSP 4.5V/0V
		lcd_write_reg_data(0xBC00,0x00);
		lcd_write_reg_data(0xBC01,0x78);
		lcd_write_reg_data(0xBC02,0x00);
		//VGMN/VGSN -4.5V/0V
		lcd_write_reg_data(0xBD00,0x00);
		lcd_write_reg_data(0xBD01,0x78);
		lcd_write_reg_data(0xBD02,0x00);
		//VCOM
		lcd_write_reg_data(0xBE00,0x00);
		lcd_write_reg_data(0xBE01,0x64);
		//Gamma Setting
		lcd_write_reg_data(0xD100,0x00);
		lcd_write_reg_data(0xD101,0x33);
		lcd_write_reg_data(0xD102,0x00);
		lcd_write_reg_data(0xD103,0x34);
		lcd_write_reg_data(0xD104,0x00);
		lcd_write_reg_data(0xD105,0x3A);
		lcd_write_reg_data(0xD106,0x00);
		lcd_write_reg_data(0xD107,0x4A);
		lcd_write_reg_data(0xD108,0x00);
		lcd_write_reg_data(0xD109,0x5C);
		lcd_write_reg_data(0xD10A,0x00);
		lcd_write_reg_data(0xD10B,0x81);
		lcd_write_reg_data(0xD10C,0x00);
		lcd_write_reg_data(0xD10D,0xA6);
		lcd_write_reg_data(0xD10E,0x00);
		lcd_write_reg_data(0xD10F,0xE5);
		lcd_write_reg_data(0xD110,0x01);
		lcd_write_reg_data(0xD111,0x13);
		lcd_write_reg_data(0xD112,0x01);
		lcd_write_reg_data(0xD113,0x54);
		lcd_write_reg_data(0xD114,0x01);
		lcd_write_reg_data(0xD115,0x82);
		lcd_write_reg_data(0xD116,0x01);
		lcd_write_reg_data(0xD117,0xCA);
		lcd_write_reg_data(0xD118,0x02);
		lcd_write_reg_data(0xD119,0x00);
		lcd_write_reg_data(0xD11A,0x02);
		lcd_write_reg_data(0xD11B,0x01);
		lcd_write_reg_data(0xD11C,0x02);
		lcd_write_reg_data(0xD11D,0x34);
		lcd_write_reg_data(0xD11E,0x02);
		lcd_write_reg_data(0xD11F,0x67);
		lcd_write_reg_data(0xD120,0x02);
		lcd_write_reg_data(0xD121,0x84);
		lcd_write_reg_data(0xD122,0x02);
		lcd_write_reg_data(0xD123,0xA4);
		lcd_write_reg_data(0xD124,0x02);
		lcd_write_reg_data(0xD125,0xB7);
		lcd_write_reg_data(0xD126,0x02);
		lcd_write_reg_data(0xD127,0xCF);
		lcd_write_reg_data(0xD128,0x02);
		lcd_write_reg_data(0xD129,0xDE);
		lcd_write_reg_data(0xD12A,0x02);
		lcd_write_reg_data(0xD12B,0xF2);
		lcd_write_reg_data(0xD12C,0x02);
		lcd_write_reg_data(0xD12D,0xFE);
		lcd_write_reg_data(0xD12E,0x03);
		lcd_write_reg_data(0xD12F,0x10);
		lcd_write_reg_data(0xD130,0x03);
		lcd_write_reg_data(0xD131,0x33);
		lcd_write_reg_data(0xD132,0x03);
		lcd_write_reg_data(0xD133,0x6D);
		lcd_write_reg_data(0xD200,0x00);
		lcd_write_reg_data(0xD201,0x33);
		lcd_write_reg_data(0xD202,0x00);
		lcd_write_reg_data(0xD203,0x34);
		lcd_write_reg_data(0xD204,0x00);
		lcd_write_reg_data(0xD205,0x3A);
		lcd_write_reg_data(0xD206,0x00);
		lcd_write_reg_data(0xD207,0x4A);
		lcd_write_reg_data(0xD208,0x00);
		lcd_write_reg_data(0xD209,0x5C);
		lcd_write_reg_data(0xD20A,0x00);

		lcd_write_reg_data(0xD20B,0x81);
		lcd_write_reg_data(0xD20C,0x00);
		lcd_write_reg_data(0xD20D,0xA6);
		lcd_write_reg_data(0xD20E,0x00);
		lcd_write_reg_data(0xD20F,0xE5);
		lcd_write_reg_data(0xD210,0x01);
		lcd_write_reg_data(0xD211,0x13);
		lcd_write_reg_data(0xD212,0x01);
		lcd_write_reg_data(0xD213,0x54);
		lcd_write_reg_data(0xD214,0x01);
		lcd_write_reg_data(0xD215,0x82);
		lcd_write_reg_data(0xD216,0x01);
		lcd_write_reg_data(0xD217,0xCA);
		lcd_write_reg_data(0xD218,0x02);
		lcd_write_reg_data(0xD219,0x00);
		lcd_write_reg_data(0xD21A,0x02);
		lcd_write_reg_data(0xD21B,0x01);
		lcd_write_reg_data(0xD21C,0x02);
		lcd_write_reg_data(0xD21D,0x34);
		lcd_write_reg_data(0xD21E,0x02);
		lcd_write_reg_data(0xD21F,0x67);
		lcd_write_reg_data(0xD220,0x02);
		lcd_write_reg_data(0xD221,0x84);
		lcd_write_reg_data(0xD222,0x02);
		lcd_write_reg_data(0xD223,0xA4);
		lcd_write_reg_data(0xD224,0x02);
		lcd_write_reg_data(0xD225,0xB7);
		lcd_write_reg_data(0xD226,0x02);
		lcd_write_reg_data(0xD227,0xCF);
		lcd_write_reg_data(0xD228,0x02);
		lcd_write_reg_data(0xD229,0xDE);
		lcd_write_reg_data(0xD22A,0x02);
		lcd_write_reg_data(0xD22B,0xF2);
		lcd_write_reg_data(0xD22C,0x02);
		lcd_write_reg_data(0xD22D,0xFE);
		lcd_write_reg_data(0xD22E,0x03);
		lcd_write_reg_data(0xD22F,0x10);
		lcd_write_reg_data(0xD230,0x03);
		lcd_write_reg_data(0xD231,0x33);
		lcd_write_reg_data(0xD232,0x03);
		lcd_write_reg_data(0xD233,0x6D);
		lcd_write_reg_data(0xD300,0x00);
		lcd_write_reg_data(0xD301,0x33);
		lcd_write_reg_data(0xD302,0x00);
		lcd_write_reg_data(0xD303,0x34);
		lcd_write_reg_data(0xD304,0x00);
		lcd_write_reg_data(0xD305,0x3A);
		lcd_write_reg_data(0xD306,0x00);
		lcd_write_reg_data(0xD307,0x4A);
		lcd_write_reg_data(0xD308,0x00);
		lcd_write_reg_data(0xD309,0x5C);
		lcd_write_reg_data(0xD30A,0x00);

		lcd_write_reg_data(0xD30B,0x81);
		lcd_write_reg_data(0xD30C,0x00);
		lcd_write_reg_data(0xD30D,0xA6);
		lcd_write_reg_data(0xD30E,0x00);
		lcd_write_reg_data(0xD30F,0xE5);
		lcd_write_reg_data(0xD310,0x01);
		lcd_write_reg_data(0xD311,0x13);
		lcd_write_reg_data(0xD312,0x01);
		lcd_write_reg_data(0xD313,0x54);
		lcd_write_reg_data(0xD314,0x01);
		lcd_write_reg_data(0xD315,0x82);
		lcd_write_reg_data(0xD316,0x01);
		lcd_write_reg_data(0xD317,0xCA);
		lcd_write_reg_data(0xD318,0x02);
		lcd_write_reg_data(0xD319,0x00);
		lcd_write_reg_data(0xD31A,0x02);
		lcd_write_reg_data(0xD31B,0x01);
		lcd_write_reg_data(0xD31C,0x02);
		lcd_write_reg_data(0xD31D,0x34);
		lcd_write_reg_data(0xD31E,0x02);
		lcd_write_reg_data(0xD31F,0x67);
		lcd_write_reg_data(0xD320,0x02);
		lcd_write_reg_data(0xD321,0x84);
		lcd_write_reg_data(0xD322,0x02);
		lcd_write_reg_data(0xD323,0xA4);
		lcd_write_reg_data(0xD324,0x02);
		lcd_write_reg_data(0xD325,0xB7);
		lcd_write_reg_data(0xD326,0x02);
		lcd_write_reg_data(0xD327,0xCF);
		lcd_write_reg_data(0xD328,0x02);
		lcd_write_reg_data(0xD329,0xDE);
		lcd_write_reg_data(0xD32A,0x02);
		lcd_write_reg_data(0xD32B,0xF2);
		lcd_write_reg_data(0xD32C,0x02);
		lcd_write_reg_data(0xD32D,0xFE);
		lcd_write_reg_data(0xD32E,0x03);
		lcd_write_reg_data(0xD32F,0x10);
		lcd_write_reg_data(0xD330,0x03);
		lcd_write_reg_data(0xD331,0x33);
		lcd_write_reg_data(0xD332,0x03);
		lcd_write_reg_data(0xD333,0x6D);
		lcd_write_reg_data(0xD400,0x00);
		lcd_write_reg_data(0xD401,0x33);
		lcd_write_reg_data(0xD402,0x00);
		lcd_write_reg_data(0xD403,0x34);
		lcd_write_reg_data(0xD404,0x00);
		lcd_write_reg_data(0xD405,0x3A);
		lcd_write_reg_data(0xD406,0x00);
		lcd_write_reg_data(0xD407,0x4A);
		lcd_write_reg_data(0xD408,0x00);
		lcd_write_reg_data(0xD409,0x5C);
		lcd_write_reg_data(0xD40A,0x00);
		lcd_write_reg_data(0xD40B,0x81);

		lcd_write_reg_data(0xD40C,0x00);
		lcd_write_reg_data(0xD40D,0xA6);
		lcd_write_reg_data(0xD40E,0x00);
		lcd_write_reg_data(0xD40F,0xE5);
		lcd_write_reg_data(0xD410,0x01);
		lcd_write_reg_data(0xD411,0x13);
		lcd_write_reg_data(0xD412,0x01);
		lcd_write_reg_data(0xD413,0x54);
		lcd_write_reg_data(0xD414,0x01);
		lcd_write_reg_data(0xD415,0x82);
		lcd_write_reg_data(0xD416,0x01);
		lcd_write_reg_data(0xD417,0xCA);
		lcd_write_reg_data(0xD418,0x02);
		lcd_write_reg_data(0xD419,0x00);
		lcd_write_reg_data(0xD41A,0x02);
		lcd_write_reg_data(0xD41B,0x01);
		lcd_write_reg_data(0xD41C,0x02);
		lcd_write_reg_data(0xD41D,0x34);
		lcd_write_reg_data(0xD41E,0x02);
		lcd_write_reg_data(0xD41F,0x67);
		lcd_write_reg_data(0xD420,0x02);
		lcd_write_reg_data(0xD421,0x84);
		lcd_write_reg_data(0xD422,0x02);
		lcd_write_reg_data(0xD423,0xA4);
		lcd_write_reg_data(0xD424,0x02);
		lcd_write_reg_data(0xD425,0xB7);
		lcd_write_reg_data(0xD426,0x02);
		lcd_write_reg_data(0xD427,0xCF);
		lcd_write_reg_data(0xD428,0x02);
		lcd_write_reg_data(0xD429,0xDE);
		lcd_write_reg_data(0xD42A,0x02);
		lcd_write_reg_data(0xD42B,0xF2);
		lcd_write_reg_data(0xD42C,0x02);
		lcd_write_reg_data(0xD42D,0xFE);
		lcd_write_reg_data(0xD42E,0x03);
		lcd_write_reg_data(0xD42F,0x10);
		lcd_write_reg_data(0xD430,0x03);
		lcd_write_reg_data(0xD431,0x33);
		lcd_write_reg_data(0xD432,0x03);
		lcd_write_reg_data(0xD433,0x6D);
		lcd_write_reg_data(0xD500,0x00);
		lcd_write_reg_data(0xD501,0x33);
		lcd_write_reg_data(0xD502,0x00);
		lcd_write_reg_data(0xD503,0x34);
		lcd_write_reg_data(0xD504,0x00);
		lcd_write_reg_data(0xD505,0x3A);
		lcd_write_reg_data(0xD506,0x00);
		lcd_write_reg_data(0xD507,0x4A);
		lcd_write_reg_data(0xD508,0x00);
		lcd_write_reg_data(0xD509,0x5C);
		lcd_write_reg_data(0xD50A,0x00);
		lcd_write_reg_data(0xD50B,0x81);

		lcd_write_reg_data(0xD50C,0x00);
		lcd_write_reg_data(0xD50D,0xA6);
		lcd_write_reg_data(0xD50E,0x00);
		lcd_write_reg_data(0xD50F,0xE5);
		lcd_write_reg_data(0xD510,0x01);
		lcd_write_reg_data(0xD511,0x13);
		lcd_write_reg_data(0xD512,0x01);
		lcd_write_reg_data(0xD513,0x54);
		lcd_write_reg_data(0xD514,0x01);
		lcd_write_reg_data(0xD515,0x82);
		lcd_write_reg_data(0xD516,0x01);
		lcd_write_reg_data(0xD517,0xCA);
		lcd_write_reg_data(0xD518,0x02);
		lcd_write_reg_data(0xD519,0x00);
		lcd_write_reg_data(0xD51A,0x02);
		lcd_write_reg_data(0xD51B,0x01);
		lcd_write_reg_data(0xD51C,0x02);
		lcd_write_reg_data(0xD51D,0x34);
		lcd_write_reg_data(0xD51E,0x02);
		lcd_write_reg_data(0xD51F,0x67);
		lcd_write_reg_data(0xD520,0x02);
		lcd_write_reg_data(0xD521,0x84);
		lcd_write_reg_data(0xD522,0x02);
		lcd_write_reg_data(0xD523,0xA4);
		lcd_write_reg_data(0xD524,0x02);
		lcd_write_reg_data(0xD525,0xB7);
		lcd_write_reg_data(0xD526,0x02);
		lcd_write_reg_data(0xD527,0xCF);
		lcd_write_reg_data(0xD528,0x02);
		lcd_write_reg_data(0xD529,0xDE);
		lcd_write_reg_data(0xD52A,0x02);
		lcd_write_reg_data(0xD52B,0xF2);
		lcd_write_reg_data(0xD52C,0x02);
		lcd_write_reg_data(0xD52D,0xFE);
		lcd_write_reg_data(0xD52E,0x03);
		lcd_write_reg_data(0xD52F,0x10);
		lcd_write_reg_data(0xD530,0x03);
		lcd_write_reg_data(0xD531,0x33);
		lcd_write_reg_data(0xD532,0x03);
		lcd_write_reg_data(0xD533,0x6D);
		lcd_write_reg_data(0xD600,0x00);
		lcd_write_reg_data(0xD601,0x33);
		lcd_write_reg_data(0xD602,0x00);
		lcd_write_reg_data(0xD603,0x34);
		lcd_write_reg_data(0xD604,0x00);
		lcd_write_reg_data(0xD605,0x3A);
		lcd_write_reg_data(0xD606,0x00);
		lcd_write_reg_data(0xD607,0x4A);
		lcd_write_reg_data(0xD608,0x00);
		lcd_write_reg_data(0xD609,0x5C);
		lcd_write_reg_data(0xD60A,0x00);
		lcd_write_reg_data(0xD60B,0x81);

		lcd_write_reg_data(0xD60C,0x00);
		lcd_write_reg_data(0xD60D,0xA6);
		lcd_write_reg_data(0xD60E,0x00);
		lcd_write_reg_data(0xD60F,0xE5);
		lcd_write_reg_data(0xD610,0x01);
		lcd_write_reg_data(0xD611,0x13);
		lcd_write_reg_data(0xD612,0x01);
		lcd_write_reg_data(0xD613,0x54);
		lcd_write_reg_data(0xD614,0x01);
		lcd_write_reg_data(0xD615,0x82);
		lcd_write_reg_data(0xD616,0x01);
		lcd_write_reg_data(0xD617,0xCA);
		lcd_write_reg_data(0xD618,0x02);
		lcd_write_reg_data(0xD619,0x00);
		lcd_write_reg_data(0xD61A,0x02);
		lcd_write_reg_data(0xD61B,0x01);
		lcd_write_reg_data(0xD61C,0x02);
		lcd_write_reg_data(0xD61D,0x34);
		lcd_write_reg_data(0xD61E,0x02);
		lcd_write_reg_data(0xD61F,0x67);
		lcd_write_reg_data(0xD620,0x02);
		lcd_write_reg_data(0xD621,0x84);
		lcd_write_reg_data(0xD622,0x02);
		lcd_write_reg_data(0xD623,0xA4);
		lcd_write_reg_data(0xD624,0x02);
		lcd_write_reg_data(0xD625,0xB7);
		lcd_write_reg_data(0xD626,0x02);
		lcd_write_reg_data(0xD627,0xCF);
		lcd_write_reg_data(0xD628,0x02);
		lcd_write_reg_data(0xD629,0xDE);
		lcd_write_reg_data(0xD62A,0x02);
		lcd_write_reg_data(0xD62B,0xF2);
		lcd_write_reg_data(0xD62C,0x02);
		lcd_write_reg_data(0xD62D,0xFE);
		lcd_write_reg_data(0xD62E,0x03);
		lcd_write_reg_data(0xD62F,0x10);
		lcd_write_reg_data(0xD630,0x03);
		lcd_write_reg_data(0xD631,0x33);
		lcd_write_reg_data(0xD632,0x03);
		lcd_write_reg_data(0xD633,0x6D);
		//LV2 Page 0 enable
		lcd_write_reg_data(0xF000,0x55);
		lcd_write_reg_data(0xF001,0xAA);
		lcd_write_reg_data(0xF002,0x52);
		lcd_write_reg_data(0xF003,0x08);
		lcd_write_reg_data(0xF004,0x00);
		//Display control
		lcd_write_reg_data(0xB100, 0xCC);
		lcd_write_reg_data(0xB101, 0x00);
		//Source hold time
		lcd_write_reg_data(0xB600,0x05);
		//Gate EQ control
		lcd_write_reg_data(0xB700,0x70);
		lcd_write_reg_data(0xB701,0x70);
		//Source EQ control (Mode 2)
		lcd_write_reg_data(0xB800,0x01);
		lcd_write_reg_data(0xB801,0x03);
		lcd_write_reg_data(0xB802,0x03);
		lcd_write_reg_data(0xB803,0x03);
		//Inversion mode (2-dot)
		lcd_write_reg_data(0xBC00,0x02);
		lcd_write_reg_data(0xBC01,0x00);
		lcd_write_reg_data(0xBC02,0x00);
		//Timing control 4H w/ 4-delay
		lcd_write_reg_data(0xC900,0xD0);
		lcd_write_reg_data(0xC901,0x02);
		lcd_write_reg_data(0xC902,0x50);
		lcd_write_reg_data(0xC903,0x50);
		lcd_write_reg_data(0xC904,0x50);
		lcd_write_reg_data(0x3500,0x00);
		lcd_write_reg_data(0x3A00,0x55);  //16-bit/pixel
		lcd_write_reg_16bit(0x1100);
		//delay_us(120);
		systick_us_delay(120);
		lcd_write_reg_16bit(0x2900);  
	}
	
		 
	//初始化完成以后,提速
	if(lcd_dev.id==0X9341||lcd_dev.id==0X5310||lcd_dev.id==0X5510||lcd_dev.id==0X1963)//如果是这几个IC,则设置WR时序为最快
	{
		//重新配置写时序控制寄存器的时序   	 							    
		FMC_Bank1E->BWTR[0]&=~(0XF<<0);	//地址建立时间(ADDSET)清零 	 
		FMC_Bank1E->BWTR[0]&=~(0XF<<8);	//数据保存时间清零
		FMC_Bank1E->BWTR[0]|=4<<0;	 	//地址建立时间(ADDSET)为4个HCLK =21ns  	 
		FMC_Bank1E->BWTR[0]|=4<<8; 		//数据保存时间(DATAST)为5.2ns*4个HCLK=21ns
	} 

	/* 显示区域背景及字体颜色 */
	point_color = WHITE; 
	back_color = BLUE;
	/* 默认为竖屏 */
	lcd_display_dir(0);	
	/* 屏幕背景色 */	
	lcd_clear(BLUE);
	/* 点亮背 */
	systick_ms_delay(50);
	LCD_BACK_LIGHT;				
	return RT_EOK;
}



