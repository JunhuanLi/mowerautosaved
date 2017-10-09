/*
 * File      : usart.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

#ifndef __USART_H__
#define __USART_H__

#include <rthw.h>
#include <rtthread.h>
#include "stm32f4xx.h"

/* UART GPIO define. */
#define UART1_GPIO_TX       GPIO_Pin_9
#define UART1_TX_PIN_SOURCE GPIO_PinSource9
#define UART1_GPIO_RX       GPIO_Pin_10
#define UART1_RX_PIN_SOURCE GPIO_PinSource10
#define UART1_GPIO          GPIOA
#define UART1_GPIO_RCC      RCC_AHB1Periph_GPIOA
#define RCC_APBPeriph_UART1 RCC_APB2Periph_USART1
#define UART1_TX_DMA        DMA1_Channel4
#define UART1_RX_DMA        DMA1_Channel5

#define UART2_GPIO_TX       GPIO_Pin_2
#define UART2_TX_PIN_SOURCE GPIO_PinSource2
#define UART2_GPIO_RX       GPIO_Pin_3
#define UART2_RX_PIN_SOURCE GPIO_PinSource3
#define UART2_GPIO	    		GPIOA
#define UART2_GPIO_RCC   		RCC_AHB1Periph_GPIOA
#define RCC_APBPeriph_UART2	RCC_APB1Periph_USART2

/* USART3_REMAP[1:0] = 00 */
#define UART3_GPIO_TX				GPIO_Pin_10
#define UART3_TX_PIN_SOURCE GPIO_PinSource10
#define UART3_GPIO_RX				GPIO_Pin_11
#define UART3_RX_PIN_SOURCE GPIO_PinSource11
#define UART3_GPIO					GPIOB
#define UART3_GPIO_RCC   		RCC_AHB1Periph_GPIOB
#define RCC_APBPeriph_UART3	RCC_APB1Periph_USART3
#define UART3_TX_DMA				DMA1_Stream1
#define UART3_RX_DMA				DMA1_Stream3

#define UART8_GPIO_TX 			GPIO_Pin_1
#define UART8_TX_PIN_SOURCE GPIO_PinSource1
#define UART8_GPIO_RX				GPIO_Pin_0
#define UART8_RX_PIN_SOURCE GPIO_PinSource0
#define UART8_GPIO					GPIOE
#define UART8_GPIO_RCC   		RCC_AHB1Periph_GPIOE
#define RCC_APBPeriph_UART8	RCC_APB1Periph_UART8



#define RT_SERIAL_CONFIG_USER1           \
{                                          \
    BAUD_RATE_115200, /* 115200 bits/s */  \
    DATA_BITS_8,      /* 8 databits */     \
    STOP_BITS_1,      /* 1 stopbit */      \
    PARITY_NONE,      /* No parity  */     \
    BIT_ORDER_LSB,    /* LSB first sent */ \
    NRZ_NORMAL,       /* Normal mode */    \
    200, /* Buffer size */  \
    0                                      \
}



extern struct rt_device uart2_device;
extern struct rt_device uart8_device;

#define UART_ENABLE_IRQ(n)            NVIC_EnableIRQ((n))
#define UART_DISABLE_IRQ(n)           NVIC_DisableIRQ((n))

int stm32_hw_usart_init(void);

#endif
