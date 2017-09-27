/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			lcd12864_io_spi.h
  Author:				Raymond
  Date:				2017.6.15
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

#ifndef __LCD12864_IO_SPI_H__
#define __LCD12864_IO_SPI_H__

#include "stm32f4xx.h"

#ifdef SUPPORT_BOARD_VERSION_2
#define RS			PH_OUT(6)
#define RSTB		PD_OUT(7)
#define CSB			PD_OUT(11)
#define SID			PB_OUT(15)
#define SCK			PB_OUT(13)
#define LCD_PWM	    PD_OUT(12)

#else
#define RS			PG_OUT(12)
#define RSTB		PG_OUT(14)
#define CSB			PG_OUT(13)
#define SID			PG_OUT(8)
#define SCK			PF_OUT(13)
#define LCD_PWM	PD_OUT(12)
#endif

void clear_screen(void);
void test_display(u8 data1,u8 data2);
void display_string_8x16(u8 fb,u16 page,u16 column,u8 *text);
void display_string_8x16_t(u8 fb,u16 page,u16 column,u8 text);
void display_string_5x8(u16 page,u16 column,u8 *text);
void display_graphic_16x16(u8 fb,u8 page,u8 column,u8 *dp);
void display_bk(void);
void lcd12864_st7565r_init(void);
void lcd12864_st7565r_config(void);
void led_test(void);
void all_dispaly(u8 string[][130]);

extern u8 const ascii_table_8x16[95][16];
extern u8 const ascii_table_6x8[95][6]; 
extern u8 const changer_8x16[][16];
extern u8 const gps_icon_8x8[][8];
void display_string_5x8_gundong(u16 page,u16 column,u8 *text);

#endif

