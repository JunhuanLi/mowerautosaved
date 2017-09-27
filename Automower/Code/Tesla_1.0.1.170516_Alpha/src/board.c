/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009 RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      first implementation
 */

#include <rthw.h>
#include <rtthread.h>
#include "stm32f4xx.h"
#include "board.h"
#include "stm32f4_rtc.h"

#include "mower_hw_config.h"
#include "delay.h" 
#include "usart.h" 
#include "lcd.h"
#include "usart_driver.h"
#include "lcd12864_io_spi.h"

/**
 * @addtogroup STM32
 */

/*@{*/



/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
	/* 4*4 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}



/*******************************************************************************
 * Function Name  : SysTick_Configuration
 * Description    : Configures the SysTick for OS tick.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void  SysTick_Configuration(void)
{
	RCC_ClocksTypeDef  rcc_clocks;
	rt_uint32_t         cnts;
    
	RCC_GetClocksFreq(&rcc_clocks);
	/*每秒100次心跳 10ms周期*/
	cnts = (rt_uint32_t)rcc_clocks.HCLK_Frequency / RT_TICK_PER_SECOND;  
	cnts = cnts / 8;

	SysTick_Config(cnts);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);// 外部时钟
}




/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();

	rt_tick_increase();

	/* leave interrupt */
	rt_interrupt_leave();
}




/**
 * This function will initial STM32 board.
 */
void rt_hw_board_init()
{
	// SYSCLK = 180M 
	// 读取系统变量值
    SystemCoreClockUpdate();

	/* NVIC Configuration */
	NVIC_Configuration();

	/* Configure the SysTick */
	SysTick_Configuration();

	/* configure test usart */
	stm32_hw_usart_init();

#ifdef RT_USING_CONSOLE
	rt_console_set_device(CONSOLE_DEVICE);
#endif

    /* rtc config */
    rt_hw_rtc_init();
    
	/* system hw config ---add by Raymond 20170613 */
	peripheral_hw_config();

		//uart_emsensor_init();
    gui_init();

   //cpp_test();

}

/*@}*/
