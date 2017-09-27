/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:	    thread_app.h
  Author:			Raymond
  Date:				2017.7.31
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

#ifndef __THREAD_APP_H__
#define __THREAD_APP_H__


/*************�߳�ʹ�ܺ궨��**************/
#define GPS_THREAD_ENABLE     // gps�߳�     
//#define GUI_THREAD_ENABLE       // gui�߳�    
//#define KEY_THREAD_ENABLE       // ����ɨ���߳�    
#define IMU_THREAD_ENABLE       // imu�߳�       
#define UPDATA_THREAD_ENABLE    // ���ݲɼ������߳�   
//#define MONITOR_THREAD_ENABLE   // ϵͳ����߳�  
#define MOTION_THREAD_ENABLE    // �˶������߳�  
#define WIRELESS_THREAD_ENABLE  // ���ߴ��������߳� 


//#define MOTOR_THREAD_ENABLE     // ��������߳� 

/******�߳����ȼ����ƺ�******/ 
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



