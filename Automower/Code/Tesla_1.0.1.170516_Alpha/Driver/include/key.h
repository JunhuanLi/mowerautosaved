/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			key.h
  Author:				Raymond
  Date:				2017.6.8
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

#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f4xx.h"

#define SUPPORT_KEY_REGISTER // 使用注册到系统的方式调用按键代码

/* 按键结构体 */
typedef struct
{
    u8 short_value;
    u8 attribute;
    u8 long_value;
} mower_key_struct;

/* 按键属性枚举 */
enum
{
    KEY_ATTR_SHORT,
    KEY_ATTR_CONTINUE,
    KEY_ATTR_LONG1,
    KEY_ATTR_LONG2,
};

/* 宏定义按键虚拟值 */
typedef enum
{
/* 宏定义按键虚拟值 */
/*01*/	K_NUM_0 	= 0x00,
/*02*/	K_NUM_1 	= 0x01,
/*03*/	K_NUM_2 	= 0x02,
/*04*/	K_NUM_3 	= 0x03,
/*05*/	K_NUM_4 	= 0x04,
/*06*/	K_NUM_5		= 0x05,
/*07*/	K_NUM_6		= 0x06,
/*08*/	K_NUM_7		= 0x07,
/*09*/	K_NUM_8		= 0x08,
/*10*/	K_NUM_9		= 0x09,
/*11*/	K_JIN		= 0x0a,
/*12*/	K_XING		= 0x0b,
/*13*/	K_HOME		= 0x0c,
/*14*/	K_RETURN	= 0x0d,
/*15*/	K_SETUP		= 0x0e,
/*16*/	K_UP 		= 0x0f,
/*17*/	K_DOWN		= 0x10,
/*18*/	K_LEFT		= 0x11,
/*19*/	K_RIGHT		= 0x12,
/*20*/	K_OK		= 0x13,
/*21*/	K_STOP		= 0x14,
/*22*/	K_POWER		= 0x15,
/*23*/  K_START     = 0x16,


/*  */	K_NULL		= 0xff,		// 空按键

}key_enum;

/*************************************
 * 长按键判断的时间限定值
 *************************************/
#define KEY_PRESS_TIME_LOCK 	0xffff
#define INDEX_CURRENT_INVALID 	0xff
	
#define KEY_PRESS_TIME_SHORT				3  // 3*10ms = 30ms
#define KEY_PRESS_TIME_CONTINUE 			50 // 50*10ms = 500ms
#define KEY_PRESS_TIME_CONTINUE_SECOND 		10 // 10*10ms = 100ms
#define KEY_PRESS_TIME_LONG1				(3*100) // 3*100*10ms =3s
#define KEY_PRESS_TIME_LONG2				(5*100) // 5*100*10ms =3s



/*****最大按键数量********************/
#define KEY_NUM_MAX     25  // 按键数量，根据按键的个数改变

/************************************************************************
 * 按键线的相关宏定义
 ************************************************************************/
#define ROW_MAX     4 // 矩阵按键横线数量
#define COLUMN_MAX  5 // 矩阵按键竖线数量


#ifdef SUPPORT_BOARD_VERSION_2
#define ROW_1   PG_OUT(5)
#define ROW_2   PG_OUT(11)
#define ROW_3   PG_OUT(2)
#define ROW_4   PG_OUT(1)
//#define row_5   PB_OUT(10)

#define COLUMN_1    PA_IN(0)
#define COLUMN_2    PG_IN(9)
#define COLUMN_3    PG_IN(10)
#define COLUMN_4    PG_IN(7)
#define COLUMN_5    PG_IN(6)

#else
#define ROW_1   PG_OUT(0)
#define ROW_2   PG_OUT(1)
#define ROW_3   PG_OUT(2)
#define ROW_4   PG_OUT(4)
//#define row_5   PB_OUT(10)

#define COLUMN_1    PG_IN(5)
#define COLUMN_2    PG_IN(6)
#define COLUMN_3    PG_IN(7)
#define COLUMN_4    PG_IN(10)
#define COLUMN_5    PG_IN(9)
#endif

// 按键数据结构体
typedef struct mower_key_dev_tag
{
    rt_uint8_t key_index;
    rt_uint8_t key_index_pre;
    rt_uint8_t key_value;
    rt_uint8_t key_step;
    rt_uint16_t press_10ms_cnt;
    rt_uint8_t key_long_flag;
    
    rt_uint8_t (*mower_key_scan)(void);
    key_enum (*mower_key_check)(const mower_key_struct *key_table,struct mower_key_dev_tag * key_m);
} mower_key_dev;


#ifdef SUPPORT_KEY_REGISTER
extern mower_key_dev t_key_m;
extern rt_device_t _key_device;
#endif
extern const mower_key_struct key_table[KEY_NUM_MAX];




void key_hw_config(void);
void key_variable_init(void);
u8 key_scan(void);

#ifndef SUPPORT_KEY_REGISTER
u8 key_check(const mower_key_struct *key_table,u8 *pre_index,u8 index,u8 *key_step,volatile u16 *press_10ms_cnt);
#else
u8 key_check_d(const mower_key_struct *key_table,mower_key_dev * key_m);
rt_err_t rt_hw_key_register(rt_device_t device, const char* name, rt_uint32_t flag, mower_key_dev *key_m);
rt_device_t rt_key_device_set(const char *name);
#endif

#endif 

