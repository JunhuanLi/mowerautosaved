/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			thread_key.c
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


#include <rtthread.h>
#include "thread_app.h"
#include "key.h"
#include "menu.h"
#include "mower_common.h"
#include "beep.h"

u8 key_index;
u8 key_index_pre;
u8 key_continue_step;
u16 key_press_10ms_cnt;


#ifdef SUPPORT_KEY_REGISTER
static rt_device_t key_p = RT_NULL;
#endif

void key_thread_variable_init(void)
{
    key_index = 0xff;
	key_index_pre = 0xff;
	key_continue_step = 0x00;
	key_press_10ms_cnt = 0x00;
}

void key_function_filt(key_enum *key)
{
    // disable key
    if(!sys_power_state)
    {
        if(*key != K_POWER)*key = K_NULL;
    }

    if(*key != K_NULL)
        #ifdef SUPPORT_BOARD_VERSION_2
        beep_ctrl(&beep,ENABLE,BEEP_ATTR_SINGLE_PASS,30);
        #else
        beep_ctrl(ENABLE,BEEP_ATTR_SINGLE_PASS,30);
        #endif
}


#ifndef SUPPORT_KEY_REGISTER
void key_process(void)
{
    key_enum key_value;

    key_index = key_scan();
    key_value = (key_enum)key_check(key_table,&key_index_pre,key_index,&key_continue_step,&key_press_10ms_cnt);

    key_function_filt(&key_value);
    
    switch (key_value)
    {
        case K_UP:
        case K_DOWN:
            text_box_data_modify(key_value);
            break;
        default:

            break;
    }
}
#else
void key_process(rt_device_t p)
{
    key_enum key_value;

    static e_work_pattern pattern_cnt;

    mower_key_dev * key_dev = RT_NULL;

    key_dev = (mower_key_dev *)p->user_data;

    key_dev->key_index = key_dev->mower_key_scan();
    key_value = (key_enum)key_dev->mower_key_check(key_table,key_dev);

    key_function_filt(&key_value);

    switch (key_value)
    {
        case K_NUM_1: // #1
            rt_event_send(&exception_event,EVENT_EXC_LOWER_POWER);
            break;
        case K_NUM_2: // #2
            set_date(2017,9,14);
            set_time(10,12,30);
            break;
        case K_NUM_3: // #3

            break;
        case K_NUM_4: // #4

            break;
        case K_NUM_5: // #5

            break;
        case K_NUM_6: // #6

            break;
        case K_NUM_7: // #7

            break;
        case K_NUM_8: // #8

            break;
        case K_NUM_9: // #9

            break;
        case K_NUM_0: // #0

            break;
        case K_UP: // up

            break;
        case K_DOWN: // down
            text_box_data_modify(key_value);

            pattern_cnt++;
            if(pattern_cnt==WORK_MODE_TOTAL)pattern_cnt=WORK_MODE_STANDBY;
            front_work_pattern_tran_req(pattern_cnt);
            rt_event_send(&sys_event,SYS_EVN_PATTERN_REQ);
            break;
        case K_HOME: // home
            
            break;
        case K_RETURN: // return

            break;
        case K_SETUP: // 

            break;
        case K_OK: // OK

            break;
        case K_STOP: // stop  急停按键
            text_box_data_modify(key_value);
            
            break;
        case K_POWER:  // POWER
        
            sys_power_state ^= 1;
            
            #ifdef CUSTOMER_SERIAL_DEBUG
            rt_kprintf("\r\n...sys_power_state = %d...\r\n",sys_power_state);
            #endif            

            if(sys_power_state)
            {
                lcd12864_st7565r_init();
                clear_screen(); //clear all dots
                LCD_PWM = 1;
            }
            else
            {
                clear_screen(); //clear all dots
                LCD_PWM = 0;               
            }

            break;
        case K_START: //START

            break;
        default:

            break;
    }
}
#endif





ALIGN(RT_ALIGN_SIZE)
char thread_key_stack[512];
struct rt_thread thread_key;

void mower_key_thread(void* parameter)
{
    rt_uint32_t event_rec;
	rt_err_t err;
    
    #ifdef SUPPORT_KEY_REGISTER
    rt_key_device_set("d_key");
    key_p = rt_device_find("d_key");
    #endif

	while(1)
	{
#if 1
        /* scan once per 20ms */
        //err = rt_event_recv(&sys_event,SYS_EVN_KEY_SCAN,RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR,RT_WAITING_FOREVER,&event_rec);
		#ifdef CUSTOMER_SERIAL_DEBUG
		//rt_kprintf("\r\n>>>Enter key thread...");
		#endif

        #ifndef SUPPORT_KEY_REGISTER
        key_process();
        #else
        key_process(key_p);
        #endif
#endif	
        //rt_kprintf("\r\n>>>key_thread");
        //LCD_PWM = ~LCD_PWM;
		rt_thread_delay(10);// 10 ms
    }
}

