/**
  ******************************************************************************
  * @file    IO_Toggle/stm32f4xx_it.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <rtthread.h>
#include "board.h"
#include "hardware.h"
#include "usart_driver.h"

/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup IO_Toggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
//void HardFault_Handler(void)
//{
//    // definition in libcpu/arm/cortex-m4/context_*.S
//}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//    // definition in libcpu/arm/cortex-m4/context_*.S
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
//    // definition in board.c

//}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/



/*------------------------------------------------------------------------------
Function		: USART_GPS_HANDLER
Description		: USART_GPS_HANDLER
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/

extern u8 USART_GPS_RX_BUFFER[200]; 
/*
void USART_GPS_HANDLER(void)
{
	u8 res;
	u8 i = 0;
	if(USART_GetITStatus(USART_GPS,USART_IT_RXNE)!=RESET)
	{
		res = USART_ReceiveData(USART_GPS);	 
		for(i = 0; i < 100; i++)
		{
			USART_RX_BUFFER[i] = USART_RX_BUFFER[i+1];
		}
		USART_RX_BUFFER[99] = res;
	}  				 									
	//USART_ClearITPendingBit(USART_COM, USART_IT_RXNE|USART_IT_TC);
}   
*/
/*------------------------------------------------------------------------------
Function		: USART_BMS_HANDLER
Description		: USART BMS HANDLER
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void USART_BMS_HANDLER(void)
{
	u8 res;	
	if(USART_GetITStatus(USART_BMS,USART_IT_RXNE)!=RESET)
	{
		res = USART_ReceiveData(USART_BMS);		
	}  				 											 
}  
/*------------------------------------------------------------------------------
Function		: USART_COM_HANDLER
Description		: USART_COM_HANDLER
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
u8 res_gps;
const unsigned char GPSDATA_LEN = 100;
void USART_GPS_HANDLER(void)
{
	
#ifdef RT_USING_UART3
        extern struct rt_device uart3_device;
        extern void rt_hw_serial_isr(struct rt_device *device);
    
        /* enter interrupt */
        rt_interrupt_enter();
    
        rt_hw_serial_isr(&uart3_device);
    
        /* leave interrupt */
        rt_interrupt_leave();
#else
	/*
	if(USART_GetITStatus(USART_GPS,USART_IT_RXNE)!=RESET)
	{
		res_gps = USART_ReceiveData(USART_GPS);		
	} 
	*/
	u8 res = 0;
	u8 i = 0;
	if(USART_GetITStatus(USART_GPS,USART_IT_RXNE)!=RESET)
	{
		res = USART_ReceiveData(USART_GPS);	 
		for(i = 0; i < GPSDATA_LEN; i++)
		{
			USART_GPS_RX_BUFFER[i] = USART_GPS_RX_BUFFER[i+1];
		}
		USART_GPS_RX_BUFFER[GPSDATA_LEN - 1] = res;
	}  		 	
	
	//USART_ClearITPendingBit(USART_GPS, USART_IT_RXNE|USART_IT_TC);
	

#endif
}


/*******************************************************************************
  Function		: inverse_level      
  Description	: inverse_level
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void inverse_level(void)
{
	static u8 flag = 0;
	
	flag = ~flag;
	
	if(flag)
	{
		GPIO_WriteBit(GPIOH, GPIO_Pin_12, Bit_RESET);
	}
	else
	{
		GPIO_WriteBit(GPIOH, GPIO_Pin_12, Bit_SET);
	}
}
/*******************************************************************************
  Function		: EXTI1_IRQHandler      
  Description	: EXTI1_IRQHandler
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
float g_left = 0.0,  g_right = 0.0; 
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		inverse_level();
		update_left_encoder();
		g_left = get_left_encoder();
		g_right = get_right_encoder();
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}
/*******************************************************************************
  Function		: EXTI9_5_IRQHandler      
  Description	: EXTI9_5_IRQHandler
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		update_right_encoder();
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	
	if(EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		process_left_sonar();
		EXTI_ClearITPendingBit(EXTI_Line8);
	}

	if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		process_right_sonar();
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
}
/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
