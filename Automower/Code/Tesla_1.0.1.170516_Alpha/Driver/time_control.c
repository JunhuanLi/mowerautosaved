 /*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: time_control.c
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/05/17
  Description	: time control  
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

/* includes *******************************************************************/
#include "stm32f4xx_gpio.h"
#include "time_control.h"
#include "mower_common.h"
#include "key.h"
//#include "thread_key.h"

#include "posix_types.h"

#include "rtc.h"

#include "menu.h"
#include "beep.h"

/* macros *********************************************************************/

struct rt_timer timer_10ms;
struct rt_timer timer_100ms;

/* static variables ***********************************************************/
static u64 g_time_control_clock_overflow = 0;
static u64 g_time_delay = 0;

/* funcitons ******************************************************************/
static void time_decrease(void);

/*------------------------------------------------------------------------------
Function		:	time_control_clock_init
Description		: 	time_control_clock_init
Input			:	None
Output			:	None
Return			:	None
-------------------------------------------------------------------------------*/
void time_control_clock_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(TIME_CONTROL_CLOCK_TIMER_CLK, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = TIME_CONTROL_CLOCK_PERIOD - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = TIME_CONTROL_CLOCK_PRESCALE - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseInit(TIME_CONTROL_CLOCK_TIMER, &TIM_TimeBaseStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIME_CONTROL_CLOCK_TIMER->CNT = 0;
	
	TIM_ITConfig(TIME_CONTROL_CLOCK_TIMER, TIM_IT_Update, ENABLE);	
	TIM_ClearFlag(TIME_CONTROL_CLOCK_TIMER, TIM_FLAG_Update);
	TIM_Cmd(TIME_CONTROL_CLOCK_TIMER, ENABLE);
}

/*------------------------------------------------------------------------------
Function		:	time_delay_us
Description		: 	time delay us
Input			:	None
Output			:	None
Return			:	None
-------------------------------------------------------------------------------*/
void time_delay_10us(u32 delay_time)
{
	g_time_delay = delay_time;
	while(g_time_delay != 0)
	{
	}
}

/*------------------------------------------------------------------------------
Function		:	time_delay_ms
Description		: 	time_delay_ms
Input			:	None
Output			:	None
Return			:	None
-------------------------------------------------------------------------------*/
void time_delay_ms(u32 delay_time)
{
	g_time_delay = delay_time * TIME_CONTROL_CLOCK_FREQUENCY;
	while(g_time_delay != 0)
	{
	}
}

/*------------------------------------------------------------------------------
Function		:	TIM6_IRQHandler
Description		: 	TIM6_IRQHandler
Input			:	None
Output			:	None
Return			:	None
-------------------------------------------------------------------------------*/
static void time_decrease(void)
{
	if(g_time_delay != 0)
	{
		g_time_delay--;
	}
}


/***************************************
 **Function		:	time_event
 **Description		: 	only call by timer6 isr,  period is 10us 
 **Input			:	None
 **Output			:	None
 **Return			:	None
 ***************************************
 **/
static void time_event(void)
{
    static rt_uint32_t base_time_cnt;
    static rt_uint16_t  beep_pwm_cnt;
    static FlagStatus beep_onoff = RESET;
		static u8 fg;

    base_time_cnt++;
    if(base_time_cnt%50) // 500us
    {
     #ifdef SUPPORT_BOARD_VERSION_2
      
     #else
        if(beep.state == ENABLE)
        {
            //beep_cnt++;
           // if()
           // PH_OUT
           GPIO_ToggleBits(GPIOH,GPIO_Pin_2);
        }
        else
        {
            PH_OUT(2) = 0;
        }
     #endif   
    }
    if(base_time_cnt%100 == 0)  // 1ms
    {
      
    }
    if(base_time_cnt%1000 == 0) // 10ms
    {
        //启动 按键扫描
        rt_event_send(&sys_event, SYS_EVN_KEY_SCAN);
			
			//  imu 30ms
			fg++;
			if(fg%3==0)
			{
				//rt_event_send(&sys_event, SYS_EVN_IMU);
			}
			// gps 50ms
			if(fg%5==0)
			{
				rt_event_send(&sys_event, SYS_EVN_GPS);			
			}

        /* beep moniter */
        beep_moniter(&beep);
			
    }
    if(base_time_cnt%10000 == 0) //  100ms
    {

    }
    if(base_time_cnt%20000 == 0) // 200ms
    {
        rt_event_send(&sys_event, SYS_EVN_UI_UPDATA);
			
			
				// GPS 200ms
				//rt_event_send(&sys_event, SYS_EVN_GPS);
    }
    if(base_time_cnt%50000 == 0) // 500ms
    {
        base_time_cnt = 0;
#ifdef SUPPORT_BOARD_VERSION_2
         
#else    
        //GPIO_ToggleBits(GPIOA,GPIO_Pin_0);
        if(beep_onoff == RESET)
        {
            beep_onoff = SET;
            //beep_flag = ENABLE;
        }
        else
        {
            beep_onoff = RESET;
            beep_flag = DISABLE;            
        }
#endif
    }
}
    
/*------------------------------------------------------------------------------
Function		:	TIM6_IRQHandler
Description		: 	10us generate a interrupt
Input			:	None
Output			:	None
Return			:	None
-------------------------------------------------------------------------------*/
void TIM6_DAC_IRQHandler(void)
{
    
	if(TIM_GetITStatus( TIME_CONTROL_CLOCK_TIMER, TIM_IT_Update) != RESET)
	{
		g_time_control_clock_overflow++;
		time_decrease();
        time_event();
		TIM_ClearITPendingBit(TIME_CONTROL_CLOCK_TIMER , TIM_IT_Update);
	}
}

/*------------------------------------------------------------------------------
Function		:	get_time_stamp
Description		: 	get_time_stamp
Input			:	None
Output			:	None
Return			:	None
-------------------------------------------------------------------------------*/
u64 get_time_stamp(void)
{
		u64 time_stamp = ((u64)(g_time_control_clock_overflow * (u64)10)\
	+ (u64)TIM_GetCounter(TIME_CONTROL_CLOCK_TIMER) / 90);

	return time_stamp;		// us
}
/*------------------------------------------------------------------------------
Function		:	delay_us
Description		: 	delay_us
Input			:	None
Output			:	None
Return			:	None
-------------------------------------------------------------------------------*/
void delay_us(u32 nus)
{
	u32 i;
	for(i=0;i<nus;i++) 
	{
		delay_1us();
	}
}
/*------------------------------------------------------------------------------
Function		:	delay_1us
Description		: 	delay_1us
Input			:	None
Output			:	None
Return			:	None
-------------------------------------------------------------------------------*/
void delay_1us(void)
{		
	u32 start_cnt = 0;
	u32 end_cnt = 0;
	u32 ticks = 90;
	 
	start_cnt = TIM6->CNT;
	
	while(1)
	{
		end_cnt = TIM6->CNT;
		if(end_cnt > start_cnt)
		{
			if(end_cnt - start_cnt > ticks)
			{
				break;
			}
		}
		else
		{
			if(end_cnt - start_cnt + TIM6->ARR > ticks)
			{
				break;
			}
		}
	}
}

void delay_systick(unsigned char my_ticks)
{
	u32 start_cnt = 0;
	u32 end_cnt = 0;
	u32 ticks = my_ticks;
	 
	start_cnt = TIM6->CNT;
	
	while(1)
	{
		end_cnt = TIM6->CNT;
		if(end_cnt > start_cnt)
		{
			if(end_cnt - start_cnt > ticks)
			{
				break;
			}
		}
		else
		{
			if(end_cnt - start_cnt + TIM6->ARR > ticks)
			{
				break;
			}
		}
	}
}
/*------------------------------------------------------------------------------
Function		:	delay_ms
Description		: 	delay_ms
Input			:	None
Output			:	None
Return			:	None
-------------------------------------------------------------------------------*/
void delay_ms(u16 nms)
{
	u32 i;
	for(i=0;i<nms;i++) 
	{
		delay_us(1000);
	}
}



/***************************************
 **Function		:	timeout_10ms
 **Description		: 	soft timer callback function. use for counting 
 **Input			:	None
 **Output			:	None
 **Return			:	None
 ***************************************
 **/
void soft_timeout_10ms(void * parameter)
{
    static u32 send_updata_cnt;
    
    #if 0//def CUSTOMER_SERIAL_DEBUG
    rt_kprintf("\r\n...soft_timeout_10ms...");
    #endif 

    send_updata_cnt ++;

    /* scan key timer */
    #ifndef SUPPORT_KEY_REGISTER
    if( key_press_10ms_cnt != KEY_PRESS_TIME_LOCK )key_press_10ms_cnt++;
    #else
    if(t_key_m.press_10ms_cnt != KEY_PRESS_TIME_LOCK)t_key_m.press_10ms_cnt++;
    #endif  

    /* updata sensor : 20ms */
    if(send_updata_cnt%2 == 0)
    {
        send_updata_cnt = 0;
        rt_event_send(&sys_event, SYS_EVN_SENSOR_UPDATA);
        rt_event_send(&sys_event,SYS_EVN_MOTION);
    }
    
    // 模式切换
    work_pattern_switch();
}


/***************************************
 **Function		:	soft_timeout_100ms
 **Description		: 	soft timer callback function. use for counting  
 **Input			:	None
 **Output			:	None
 **Return			:	None
 ***************************************
 **/
void soft_timeout_100ms(void * parameter)
{
    static u8 cnt;

    cnt++;
    if(cnt%2 == 0)
    {
        rt_event_send(&sys_event, SYS_EVN_BORDER);
    }
}


/***************************************
 **Function		:	soft_timeout_1000ms
 **Description		: 	soft timer callback function. use for counting  
 **Input			:	None
 **Output			:	None
 **Return			:	None
 ***************************************
 **/
void soft_timeout_1000ms(void * parameter)
{
    static u32 i;

    struct tm * now_time;
    time_t temp;
    
#ifdef SUPPORT_BOARD_VERSION_2
    static bool lcd_flag;
#endif
    
#ifdef SUPPORT_BOARD_VERSION_2
    lcd_flag^=1;
    PI_OUT(8) = lcd_flag;/* 指示灯 */

    //PH_OUT(2) = lcd_flag;
#endif
    
    i++;
    #if 0//def CUSTOMER_SERIAL_DEBUG
    rt_kprintf("\r\n...->%d\r\n",i);
    rt_kprintf("\r\n...mode_req.%d...mode_gol.%d...mode_curr.%d...mode_last.%d...mode_save.%d",
                         get_m_req(),get_m_goal(),get_m_curr(),get_m_last(),get_m_save());
    #endif

    rt_event_send(&sys_event, SYS_EVN_WIRELESS);
}

/*------------------------------------------------------------------------------
Function		:	get_time_diff
Description	:	get_time_diff
Input		  	:	None
Output			:	None
Return			:	None
-------------------------------------------------------------------------------*/
u64 get_time_diff1(void)
{
	static u64 current_cnt = 0;
	static u64 last_cnt = 0;
	u64 cnt_diff = 0;
	current_cnt = get_time_stamp();
	
	cnt_diff = current_cnt - last_cnt;
	last_cnt = current_cnt;
	return cnt_diff;
}

u64 get_time_diff2(void)
{
	static u64 current_cnt = 0;
	static u64 last_cnt = 0;
	u64 cnt_diff = 0;
	current_cnt = get_time_stamp();
	
	cnt_diff = current_cnt - last_cnt;
	last_cnt = current_cnt;
	return cnt_diff;
}

u64 get_time_diff_trapping(void)
{
	static u64 current_cnt = 0;
	static u64 last_cnt = 0;
	u64 cnt_diff = 0;
	current_cnt = get_time_stamp();
	
	cnt_diff = current_cnt - last_cnt;
	last_cnt = current_cnt;
	return cnt_diff;
}

/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
