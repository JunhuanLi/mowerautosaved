/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			mower_common.h
  Author:				Raymond
  Date:				2017.6.7
  Version:        
  Description:		��Ź������� 
  						
  						
  History:        // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                  // �߼��޸����ݼ���  
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/


#ifndef __MOWER_COMMON_H__
#define __MOWER_COMMON_H__

#include "stm32f4xx.h"
#include "rtthread.h"



// sys event 
#define SYS_EVN_MSAK            (rt_uint32_t)0
#define SYS_EVN_KEY_SCAN        (1<<0)  // ��������ɨ���¼�
#define SYS_EVN_KEY             (1<<1)  // ���������¼�
#define SYS_EVN_UI_UPDATA       (1<<2)  // ui���� 200msһ��
#define SYS_EVN_SENSOR_UPDATA   (1<<3)  // �����������¼�
#define SYS_EVN_GPS             (1<<4)  // gps�߳������¼�
#define SYS_EVN_IMU             (1<<5)  // imu�߳������¼�
#define SYS_EVN_MOTOR           (1<<6)  // motor�߳������¼�
#define SYS_EVN_WIRELESS        (1<<7)  // wireless�߳������¼�
#define SYS_EVN_BORDER          (1<<8)  // border�߳������¼�
#define SYS_EVN_MOTION          (1<<9)  // motion�߳������¼� 20ms
#define SYS_EVN_MANAGER         (1<<11) // manager�߳������¼�
#define SYS_EVN_PATTERN_REQ     (1<<12) // ģʽ�л������¼�


// exception event
#define EVENT_EXC_LOWER_POWER       ((rt_uint32_t)(1<<0)) // ������
#define EVENT_EXC_OUTSIDE_AREA      ((rt_uint32_t)(1<<1)) // �ڱ߽���
#define EVENT_EXC_NO_LOOP_SINGNAL        ((rt_uint32_t)(1<<2)) // û��Ȧ�ź�
//#define EVENT_EXC_OVER_CURRENT  (rt_uint32_t)(1<<3)
#define EVENT_EXC_LIFT              ((rt_uint32_t)(1<<4)) // lift up
#define EVENT_EXC_TRAPPED           ((rt_uint32_t)(1<<5)) // trap
#define EVENT_EXC_TOO_STEEP         ((rt_uint32_t)(1<<6))
#define EVENT_EXC_BM_OVERCURRENT    ((rt_uint32_t)(1<<7))
#define EVENT_EXC_BM_OVERLOAD       ((rt_uint32_t)(1<<8))
#define EVENT_EXC_WM_OVERCURRENT    ((rt_uint32_t)(1<<9))
#define EVENT_EXC_WM_OVERLOAD       ((rt_uint32_t)(1<<10))



typedef enum WORK_TAG
{
 /*00*/ WORK_MODE_NULL, 
 /*01*/ WORK_MODE_STANDBY,               // ����ģʽ
 /*02*/ WORK_MODE_BACK_TO_STATION,       // ����ģʽ
 /*03*/ WORK_MODE_RAIN_ESCAPE,           // ����ģʽ   
 /*04*/ WORK_MODE_ERROR,                 // ����ģʽ
 /*05*/ WORK_MODE_SPOT_CUT,              // ����ģʽ 
 /*06*/ WORK_MODE_MEMERY_CUT,            // �ϵ�ģʽ 
 /*07*/ WORK_MODE_AUTOMATIC_CUT,         // ȫ��ģʽ 
 /*08*/ WORK_MODE_IDLE,                  // ����ģʽ 
 
 /*09*/ WORK_MODE_TOTAL, 

}e_work_pattern;

extern struct rt_event sys_event;
extern struct rt_event exception_event;

extern rt_mq_t sys_messagequeue;
//extern e_work_pattern _sys_work_pattern;

extern T_u32_to_bit _sys_flag;
#define sys_power_state _sys_flag.bitfield.bit0

#define WORK_PATTERN_DEF WORK_MODE_STANDBY

/****����ģʽ�ṹ��****/
typedef struct sys_work_tag
{
    e_work_pattern work_mode_last;
    
    e_work_pattern work_mode_curr;
    
    e_work_pattern work_mode_goal;
    
    e_work_pattern work_mode_save;
    
    e_work_pattern work_mode_request;
    
}t_sys_work_mode;

typedef struct sys_work_tag * p_mode;

/****ϵͳ����ṹ****/
typedef struct sys_tag
{
    u8 power_mode;// �ϵ�ģʽ
    
    u8 pannel_mode;// �˻�����ģʽ
    
    t_sys_work_mode front_mode;// work in front
    
    t_sys_work_mode background_mode;// work in background
    
    u8 f_bak_mode;// front backup
    
    u8 b_bak_mode;// background backup
    
    u8 ui_mode;// UI show something
    
}t_sys_total; 


typedef struct work_pattern_save_tag
{
	e_work_pattern mode;
	void * data; 
}t_work_save;



typedef struct test_tag
{
    t_work_save de;
    u8 dje;
}t_test_s;


void work_pattern_switch(void);
void front_work_pattern_tran_req(e_work_pattern mode);
e_work_pattern get_m_req(void);
e_work_pattern get_m_goal(void);
e_work_pattern get_m_curr(void);
e_work_pattern get_m_last(void);
e_work_pattern get_m_save(void);




void led_idle_indicate(void);



#if 0
////////////////////////////////////////////////////////////////////////////////// 
//Ex_NVIC_Configר�ö���
#define GPIO_A 				0
#define GPIO_B 				1
#define GPIO_C				2
#define GPIO_D 				3
#define GPIO_E 				4
#define GPIO_F 				5
#define GPIO_G 				6 
#define GPIO_H 				7 
#define GPIO_I 				8 

#define FTIR   				1  		//�½��ش���
#define RTIR   				2  		//�����ش���

#if 0
//GPIO����ר�ú궨��
#define GPIO_MODE_IN    	0		//��ͨ����ģʽ
#define GPIO_MODE_OUT		1		//��ͨ���ģʽ
#define GPIO_MODE_AF		2		//AF����ģʽ
#define GPIO_MODE_AIN		3		//ģ������ģʽ

#define GPIO_SPEED_2M		0		//GPIO�ٶ�2Mhz
#define GPIO_SPEED_25M		1		//GPIO�ٶ�25Mhz
#define GPIO_SPEED_50M		2		//GPIO�ٶ�50Mhz
#define GPIO_SPEED_100M		3		//GPIO�ٶ�100Mhz

#define GPIO_PUPD_NONE		0		//����������
#define GPIO_PUPD_PU		1		//����
#define GPIO_PUPD_PD		2		//����
#define GPIO_PUPD_RES		3		//���� 

#define GPIO_OTYPE_PP		0		//�������
#define GPIO_OTYPE_OD		1		//��©��� 
#endif
//GPIO���ű�Ŷ���
#define PIN0				1<<0
#define PIN1				1<<1
#define PIN2				1<<2
#define PIN3				1<<3
#define PIN4				1<<4
#define PIN5				1<<5
#define PIN6				1<<6
#define PIN7				1<<7
#define PIN8				1<<8
#define PIN9				1<<9
#define PIN10				1<<10
#define PIN11				1<<11
#define PIN12				1<<12
#define PIN13				1<<13
#define PIN14				1<<14
#define PIN15				1<<15 
////////////////////////////////////////////////////////////////////////////////// 
u8 Sys_Clock_Set(u32 plln,u32 pllm,u32 pllp,u32 pllq);		//ϵͳʱ������
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq); //ʱ�ӳ�ʼ��  
void Sys_Soft_Reset(void);      							//ϵͳ��λ
void Sys_Standby(void);         							//����ģʽ 	
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset);	//����ƫ�Ƶ�ַ
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);			//����NVIC����
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);//�����ж�
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM);				//�ⲿ�ж����ú���(ֻ��GPIOA~I)
void GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx);		//GPIO���ù�������
void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD);//GPIO���ú���  

void sys_power_control(void);


/* ���»�ຯ�� */
void WFI_SET(void);		//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(u32 addr);	//���ö�ջ��ַ 
#endif

#endif
/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
