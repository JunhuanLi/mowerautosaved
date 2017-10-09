/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

/**
 * @addtogroup STM32
 */
/*@{*/

#include <stdio.h>
#include "stm32f4xx.h"
#include <board.h>
#include <rtthread.h>

#ifdef RT_USING_LWIP
#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#include "stm32f4xx_eth.h"
#endif

#ifdef RT_USING_GDB
#include <gdb_stub.h>
#endif

/* 使用12864屏 */
#ifdef SUPPORT_LCD12864_ST7920
#include "lcd12864_parallel.h"
#endif

/* 使用4.3寸tft屏 */
#ifdef SUPPORT_TFT43
#include "lcd.h"
#endif
#include "rainbow_led.h"
#include "lcd12864_io_spi.h"
#include "mower_common.h"
#include "time_control.h"
#include "thread_app.h"
#include "encoder.h"
#include "mpu6500_spi.h"
#include "math.h"
#include "imu_processing.h"

u8 is_imu_valid;
u8 is_odo_valid;

void rt_init_thread_entry(void* parameter)
{
/***********************************************/
//本线程的开头不能立刻调用打印函数打印消息
//否则在波特率低（9600）的情况下会出现hardfault
//2017.8.30还没查明原因，不知道是不是系统的bug
/***********************************************/
    rt_enter_critical();// 初始化阶段不允许调度

    rt_event_init(&sys_event,"sys_event",RT_IPC_FLAG_PRIO);
    rt_event_init(&exception_event,"exp_event",RT_IPC_FLAG_PRIO);
    

/***LED***/
	rainbow_led_hw_config();
	rainbow_led_variable_init();

/***KEY***/
	key_hw_config();
	key_variable_init();
	key_thread_variable_init();
	
    /*imu and odometer valid*/
    //is_imu_valid = imu_validation();
	
	/*sth. wrong in this function, need to be modify*/
	//is_odo_valid = odo_validation();


/***END***/
	#if 0//def CUSTOMER_SERIAL_DEBUG
	rt_kprintf("\r\n>>>>system peripheral device initialization END ........  \r\n ");

    #endif
    //rt_drtc_config();
    
    
    front_work_pattern_tran_req(WORK_PATTERN_DEF);//请求进入模式

    rt_exit_critical();  



	
    /* GDB STUB */
#ifdef RT_USING_GDB
    gdb_set_device("uart6");
    gdb_start();
#endif

    /* LwIP Initialization */
#ifdef RT_USING_LWIP
    {
        extern void lwip_sys_init(void);

        /* register ethernetif device */
        eth_system_device_init();

        rt_hw_stm32_eth_init();

        /* init lwip system */
        lwip_sys_init();
        rt_kprintf("TCP/IP initialized!\n");
    }
#endif	  
}


int rt_application_init()
{
    rt_thread_t init_thread;
	  rt_err_t init_thread_state;

    rt_timer_t timer_result;

	/* 初始化线程的优先级设为最高 保证初始化代码得到运行 */
#if (RT_THREAD_PRIORITY_MAX == 32)
    init_thread = rt_thread_create("init",
                                   rt_init_thread_entry, RT_NULL,
                                   2048, 1, 20);
#else
    init_thread = rt_thread_create("init",
                                   rt_init_thread_entry, RT_NULL,
                                   2048, 80, 20);
#endif
    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);

    /* 10ms */
    rt_timer_init(&timer_10ms,
                    "t10ms",
                    soft_timeout_10ms,
                    RT_NULL,
                    10,
                    RT_TIMER_FLAG_PERIODIC);
    rt_timer_start(&timer_10ms);
    
    /* 100ms */
    rt_timer_init(&timer_100ms,
                    "t100ms",
                    soft_timeout_100ms,
                    RT_NULL,
                    100,
                    RT_TIMER_FLAG_PERIODIC);
    rt_timer_start(&timer_100ms);
    
    /* 1s */
    timer_result = rt_timer_create("t1000ms",
                                    soft_timeout_1000ms,
                                    RT_NULL,
                                    1000,
                                    RT_TIMER_FLAG_PERIODIC);
    if(timer_result != RT_NULL)
    {
        rt_timer_start(timer_result);
    }

#ifdef GPS_THREAD_ENABLE
	// thread gps
    init_thread_state=rt_thread_init(&thread_gps,
                   "Gps",
                   mower_gps_thread,
                   RT_NULL,
                   &thread_gps_stack[0],
                   sizeof(thread_gps_stack),
                   PERIORI_GPS,
                   50);// pre is 10
	if(init_thread_state==RT_EOK)
	{
    	rt_thread_startup(&thread_gps);
	}
#endif

#ifdef IMU_THREAD_ENABLE
        // thread imu
        init_thread_state=rt_thread_init(&thread_imu,
                       "Imu",
                       mower_imu_thread,
                       RT_NULL,
                       &thread_imu_stack[0],
                       sizeof(thread_imu_stack),
                       PERIORI_IMU,
                       5);
        if(init_thread_state==RT_EOK)
        {
            rt_thread_startup(&thread_imu);
        }
#endif

#ifdef KEY_THREAD_ENABLE
	// thread key 
    init_thread_state=rt_thread_init(&thread_key,
                   "Key",
                   mower_key_thread,
                   RT_NULL,
                   &thread_key_stack[0],
                   sizeof(thread_key_stack),
                   PERIORI_KEY,
                   5);
	if(init_thread_state==RT_EOK)
	{
    	rt_thread_startup(&thread_key);
	}
#endif

#ifdef GUI_THREAD_ENABLE
	// thread ui
    init_thread_state=rt_thread_init(&thread_ui,
                   "Gui",
                   mower_ui_thread,
                   RT_NULL,
                   &thread_ui_stack[0],
                   sizeof(thread_ui_stack),
                   PERIORI_GUI,
                   10);
	if(init_thread_state==RT_EOK)
	{
    	rt_thread_startup(&thread_ui);
	}
#endif 


#ifdef UPDATA_THREAD_ENABLE
        // thread updata 
        init_thread_state=rt_thread_init(&thread_updata,
                       "Updata",
                       mower_updata_thread,
                       RT_NULL,
                       &thread_updata_stack[0],
                       sizeof(thread_updata_stack),
                       PERIORI_UPDATA,
                       5);
        if(init_thread_state==RT_EOK)
        {
            rt_thread_startup(&thread_updata);
        }
#endif

#ifdef MONITOR_THREAD_ENABLE
        // thread monitor 
        init_thread_state=rt_thread_init(&thread_monitor,
                       "Monitor",
                       mower_monitor_thread,
                       RT_NULL,
                       &thread_monitor_stack[0],
                       sizeof(thread_monitor_stack),
                       PERIORI_MONITOR,
                       5);
        if(init_thread_state==RT_EOK)
        {
            rt_thread_startup(&thread_monitor);
        }
#endif

#ifdef MOTION_THREAD_ENABLE
        // thread motion
        init_thread_state=rt_thread_init(&thread_motion,
                       "Motion",
                       mower_motion_thread,
                       RT_NULL,
                       &thread_motion_stack[0],
                       sizeof(thread_motion_stack),
                       PERIORI_MOTION,
                       5);
        if(init_thread_state==RT_EOK)
        {
            rt_thread_startup(&thread_motion);
        }
#endif


#ifdef WIRELESS_THREAD_ENABLE
        // thread wireless 
        init_thread_state=rt_thread_init(&thread_wireless,
                       "Wireless",
                       mower_wireless_thread,
                       RT_NULL,
                       &thread_wireless_stack[0],
                       sizeof(thread_wireless_stack),
                       PERIORI_WIRELESS,
                       5);
        if(init_thread_state==RT_EOK)
        {
            rt_thread_startup(&thread_wireless);
        }
#endif



// these threads unused yet
#if 0

#ifdef MOTOR_THREAD_ENABLE
                        // thread motor control 
                        init_thread_state=rt_thread_init(&thread_motor,
                                       "Motor",
                                       mower_motor_thread,
                                       RT_NULL,
                                       &thread_motor_stack[0],
                                       sizeof(thread_motor_stack),
                                       PERIORI_MOTOR,
                                       5);
                        if(init_thread_state==RT_EOK)
                        {
                            rt_thread_startup(&thread_motor);
                        }
#endif


                // thread data rx 
                init_thread_state=rt_thread_init(&thread_datarx,
                               "_data_rx",
                               mower_data_rx_thread,
                               RT_NULL,
                               &thread_datarx_stack[0],
                               sizeof(thread_datarx_stack),10,5);
                if(init_thread_state==RT_EOK)
                {
                    rt_thread_startup(&thread_datarx);
                }

        

                // thread data tx 
                init_thread_state=rt_thread_init(&thread_datatx,
                               "_data_tx",
                               mower_data_tx_thread,
                               RT_NULL,
                               &thread_datatx_stack[0],
                               sizeof(thread_datatx_stack),10,5);
                if(init_thread_state==RT_EOK)
                {
                    rt_thread_startup(&thread_datatx);
                }
#endif

    return 0;
}

/*@}*/
