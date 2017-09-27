/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			key.h
  Author:				Raymond
  Date:				2017.6.8
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

#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f4xx.h"

#define SUPPORT_KEY_REGISTER // ʹ��ע�ᵽϵͳ�ķ�ʽ���ð�������

/* �����ṹ�� */
typedef struct
{
    u8 short_value;
    u8 attribute;
    u8 long_value;
} mower_key_struct;

/* ��������ö�� */
enum
{
    KEY_ATTR_SHORT,
    KEY_ATTR_CONTINUE,
    KEY_ATTR_LONG1,
    KEY_ATTR_LONG2,
};

/* �궨�尴������ֵ */
typedef enum
{
/* �궨�尴������ֵ */
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


/*  */	K_NULL		= 0xff,		// �հ���

}key_enum;

/*************************************
 * �������жϵ�ʱ���޶�ֵ
 *************************************/
#define KEY_PRESS_TIME_LOCK 	0xffff
#define INDEX_CURRENT_INVALID 	0xff
	
#define KEY_PRESS_TIME_SHORT				3  // 3*10ms = 30ms
#define KEY_PRESS_TIME_CONTINUE 			50 // 50*10ms = 500ms
#define KEY_PRESS_TIME_CONTINUE_SECOND 		10 // 10*10ms = 100ms
#define KEY_PRESS_TIME_LONG1				(3*100) // 3*100*10ms =3s
#define KEY_PRESS_TIME_LONG2				(5*100) // 5*100*10ms =3s



/*****��󰴼�����********************/
#define KEY_NUM_MAX     25  // �������������ݰ����ĸ����ı�

/************************************************************************
 * �����ߵ���غ궨��
 ************************************************************************/
#define ROW_MAX     4 // ���󰴼���������
#define COLUMN_MAX  5 // ���󰴼���������


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

// �������ݽṹ��
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

