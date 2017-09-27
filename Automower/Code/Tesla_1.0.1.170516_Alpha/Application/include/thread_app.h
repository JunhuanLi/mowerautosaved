/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:	    thread_app.h
  Author:			Raymond
  Date:				2017.7.31
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

#ifndef __THREAD_APP_H__
#define __THREAD_APP_H__


/*************线程使能宏定义**************/
#define GPS_THREAD_ENABLE     // gps线程     
//#define GUI_THREAD_ENABLE       // gui线程    
//#define KEY_THREAD_ENABLE       // 按键扫描线程    
#define IMU_THREAD_ENABLE       // imu线程       
#define UPDATA_THREAD_ENABLE    // 数据采集更新线程   
//#define MONITOR_THREAD_ENABLE   // 系统监控线程  
#define MOTION_THREAD_ENABLE    // 运动控制线程  
#define WIRELESS_THREAD_ENABLE  // 无线传输数据线程 


//#define MOTOR_THREAD_ENABLE     // 电机控制线程 

/******线程优先级控制宏******/ 
#define PERIORI_WIRELESS 5  // 1000ms

#define PERIORI_MONITOR 6   // 30ms

#define PERIORI_MOTION  7   // 20ms

#define PERIORI_KEY     4   // 10ms

#define PERIORI_GUI     5   // 200ms

#define PERIORI_UPDATA  7   // 20ms

#define PERIORI_IMU     5   // 20ms

#define PERIORI_GPS     5   // 200ms






extern char thread_datarx_stack[1024];
extern struct rt_thread thread_datarx;
void mower_data_rx_thread(void* parameter);

extern char thread_updata_stack[1024];
extern struct rt_thread thread_updata;
void mower_updata_thread(void* parameter);

extern char thread_datatx_stack[1024];
extern struct rt_thread thread_datatx;
void mower_data_tx_thread(void* parameter);

extern char thread_gps_stack[3072];
extern struct rt_thread thread_gps;
void mower_gps_thread(void* parameter);

extern char thread_imu_stack[2048];
extern struct rt_thread thread_imu;
void mower_imu_thread(void* parameter);

#include "key.h"
extern char thread_key_stack[512];
extern struct rt_thread thread_key;
extern u16 key_press_10ms_cnt;
void mower_key_thread(void* parameter);

extern char thread_manager_stack[1024];
extern struct rt_thread thread_manager;
void mower_manager_thread(void* parameter);

extern char thread_monitor_stack[1024];
extern struct rt_thread thread_monitor;
void mower_monitor_thread(void* parameter);

extern char thread_motion_stack[1024];
extern struct rt_thread thread_motion;
void mower_motion_thread(void* parameter);

extern char thread_motor_stack[1024];
extern struct rt_thread thread_motor;
void mower_motor_thread(void* parameter);

// THREAD MPU
extern char thread_mpu_stack[1024];
extern struct rt_thread thread_mpu;
void mower_mpu_thread(void* parameter);

// THREAD RAINBOWLED
extern char thread_rainbowled_stack[512];
extern struct rt_thread thread_rainbowled;
void mower_rainbowled_thread(void* parameter);

// THREAD_SLAM
extern char thread_slam_stack[512];
extern struct rt_thread thread_slam;
void mower_slam_thread(void* parameter);

// THREAD_SYSTEM_CONTRL
extern char thread_sysctrl_stack[1024];
extern struct rt_thread thread_sysctrl;
void mower_sysctrl_thread(void* parameter);

// THREAD_UI
extern char thread_ui_stack[512];
extern struct rt_thread thread_ui;
void mower_ui_thread(void* parameter);
void gui_init(void);
void text_box_data_modify(key_enum key_value);

// THREAD_UPDATA
extern char thread_updata_stack[1024];
extern struct rt_thread thread_updata;
void mower_updata_thread(void* parameter);

extern char thread_border_stack[512];
extern struct rt_thread thread_border;
void mower_border_detect_thread(void* parameter);

extern char thread_wireless_stack[512];
extern struct rt_thread thread_wireless;
void mower_wireless_thread(void* parameter);

#endif



