/*******************************************************************************
Copyright (C), 2017, TOPBAND Robot Team
File name		:	usart_driver.c
Author			:	klaus
Version			:	V1.0.0	
Date			:	2017/05/27
Description		:	usart driver
*******************************************************************************/

/* includes *******************************************************************/
#include "usart_driver.h"

/* macros *********************************************************************/

/* static variables ***********************************************************/

/* funcitons ******************************************************************/

/*------------------------------------------------------------------------------
Function		: usart_bms_init
Description		: usart_bms_init
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void usart_bms_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART_BMS_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(USART_BMS_CLK,ENABLE);
	RCC_AHB1PeriphClockCmd(USART_BMS_TX_CLK | USART_BMS_RX_CLK, ENABLE );
	

	GPIO_InitStructure.GPIO_Pin = USART_BMS_TX_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_BMS_TX_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART_BMS_RX_PIN;
	GPIO_Init(USART_BMS_RX_PORT, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(USART_BMS_TX_PORT,USART_BMS_TX_SOURCE,USART_BMS_TX_AF);
	GPIO_PinAFConfig(USART_BMS_RX_PORT,USART_BMS_RX_SOURCE,USART_BMS_RX_AF);

	USART_InitStructure.USART_BaudRate = USART_BMS_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART_BMS, &USART_InitStructure); 

	USART_ITConfig(USART_BMS, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART_BMS, ENABLE); 
}

/*------------------------------------------------------------------------------
Function		: usart_bms_init
Description		: usart_bms_init
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void usart_debug_init(void)
{
  #if 0
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART_DEBUG_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(USART_DEBUG_CLK,ENABLE);
	RCC_AHB1PeriphClockCmd(USART_DEBUG_TX_CLK | USART_DEBUG_RX_CLK, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = USART_DEBUG_TX_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_DEBUG_TX_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART_DEBUG_RX_PIN;
	GPIO_Init(USART_DEBUG_RX_PORT, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(USART_DEBUG_TX_PORT,USART_DEBUG_TX_SOURCE,USART_DEBUG_TX_AF);
	GPIO_PinAFConfig(USART_DEBUG_RX_PORT,USART_DEBUG_RX_SOURCE,USART_DEBUG_RX_AF);

	USART_InitStructure.USART_BaudRate = USART_DEBUG_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART_DEBUG, &USART_InitStructure); 

	USART_ITConfig(USART_DEBUG, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART_DEBUG, ENABLE); 
#endif
}

/*------------------------------------------------------------------------------
Function		: usart_bms_init
Description		: usart_bms_init
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void usart_com_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART_COM_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(USART_COM_CLK,ENABLE);
	RCC_AHB1PeriphClockCmd(USART_COM_TX_CLK | USART_COM_RX_CLK, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = USART_COM_TX_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_COM_TX_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART_COM_RX_PIN;
	GPIO_Init(USART_COM_RX_PORT, &GPIO_InitStructure);
	

	GPIO_PinAFConfig(USART_COM_RX_PORT,USART_COM_RX_SOURCE,USART_COM_RX_AF);
	GPIO_PinAFConfig(USART_COM_TX_PORT,USART_COM_TX_SOURCE,USART_COM_TX_AF);

	USART_InitStructure.USART_BaudRate = USART_COM_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART_COM, &USART_InitStructure); 

	USART_ITConfig(USART_COM, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART_COM, ENABLE); 
}

/*------------------------------------------------------------------------------
Function		: usart_gps_init
Description	: usart3, usart3_irqhandler function in 4xx_it.c,
							usart3 baudrate is 115200
Input				: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void usart_gps_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel 										= USART_GPS_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 				= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 								= ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(USART_GPS_CLK,ENABLE);
	RCC_AHB1PeriphClockCmd(USART_GPS_TX_CLK | USART_GPS_RX_CLK, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin 	= USART_GPS_TX_PIN;	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(USART_GPS_TX_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART_GPS_RX_PIN;
	GPIO_Init(USART_GPS_RX_PORT, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(USART_GPS_TX_PORT, USART_GPS_TX_SOURCE,USART_GPS_TX_AF);
	GPIO_PinAFConfig(USART_GPS_RX_PORT, USART_GPS_RX_SOURCE,USART_GPS_RX_AF);
	
	//GPS_EN
	GPIO_InitStructure.GPIO_Pin 	= USART_GPS_EN_PIN;	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_GPS_EN_PORT, &GPIO_InitStructure);
	GPIO_SetBits(USART_GPS_EN_PORT, USART_GPS_EN_PIN);
	
	USART_InitStructure.USART_BaudRate 		= USART_GPS_BAUDRATE;
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;
	USART_InitStructure.USART_Parity 			= USART_Parity_No;
	USART_InitStructure.USART_Mode 				= USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART_GPS, &USART_InitStructure); 

	USART_ITConfig(USART_GPS, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART_GPS, ENABLE); 
}

void USART_GPS_ENABLE(void)
{
	USART_ITConfig(USART_GPS, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART_GPS, ENABLE); 
}

void USART_GPS_DISABLE(void)
{
	USART_ITConfig(USART_GPS, USART_IT_RXNE, DISABLE);
	USART_Cmd(USART_GPS, DISABLE); 
}

/*------------------------------------------------------------------------------
Function		: usart_broder_init
Description		: usart_broder_init
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void usart_broder_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART_BRODER_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB2PeriphClockCmd(USART_BRODER_CLK,ENABLE);
	RCC_AHB1PeriphClockCmd(USART_BRODER_TX_CLK | USART_BRODER_RX_CLK, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = USART_BRODER_TX_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_BRODER_TX_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART_BRODER_RX_PIN;
	GPIO_Init(USART_BRODER_RX_PORT, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(USART_BRODER_TX_PORT,USART_BRODER_TX_SOURCE,USART_BRODER_TX_AF);
	GPIO_PinAFConfig(USART_BRODER_RX_PORT,USART_BRODER_RX_SOURCE,USART_BRODER_RX_AF);

	USART_InitStructure.USART_BaudRate = USART_BRODER_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART_BRODER, &USART_InitStructure); 

	USART_ITConfig(USART_BRODER, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART_BRODER, ENABLE); 
}

/*------------------------------------------------------------------------------
Function		: usart_send_byte
Description		: usart send byte
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void usart_send_byte(USART_TypeDef * USARTx, u8 ch)
{
	USART_SendData(USARTx, ch);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

/*------------------------------------------------------------------------------
Function		: usarts_init
Description		: usarts_init
Input			: None
Output			: None
Return			: None
-------------------------------------------------------------------------------*/
void usarts_init(void)
{	
	usart_bms_init();
	usart_debug_init();
    #ifdef SUPPORT_BOARD_VERSION_2
	//usart_broder_init();
	#else
    usart_broder_init();
    #endif
	usart_gps_init();
	//usart_com_init(); 
	uart_emsensor_init();	
}  



void uart_emsensor_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE );
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	

	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure); 

	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);

	USART_Cmd(UART5, ENABLE); 
	
	usart_send_byte(UART5, 5);
	usart_send_byte(UART5, 6);
}


u8 u5_res = 0;
u8 UART_RX_BUFFER[200];
int32_t leftsensor_data = 0;
int32_t rightsensor_data = 0;

void UART5_IRQHandler(void)
{
	u8 i = 0;
	u8 add_temp = 0;
	if(USART_GetITStatus(UART5, USART_IT_RXNE)!=RESET)
	{
		u5_res = USART_ReceiveData(UART5);	 
		for(i = 0; i < 13; i++)
		{
			UART_RX_BUFFER[i] = UART_RX_BUFFER[i+1];
			//usart_send_byte(UART5, UART_RX_BUFFER[i]);
			add_temp = add_temp + UART_RX_BUFFER[i];
		}
		UART_RX_BUFFER[12] = u5_res;
		
		
		if((UART_RX_BUFFER[0] == 0xAA) && (UART_RX_BUFFER[1] == 0xBB) && (add_temp == UART_RX_BUFFER[12]))
		{
			leftsensor_data = (UART_RX_BUFFER[9] << 8) + UART_RX_BUFFER[8];
			rightsensor_data = (UART_RX_BUFFER[11] << 8) + UART_RX_BUFFER[10];
		
			if(UART_RX_BUFFER[4] == 0x01)//V00 is 0x02, V01 is 0x01
			{
				leftsensor_data = -leftsensor_data;
			}
			if(UART_RX_BUFFER[5] == 0x01)
			{
				rightsensor_data = -rightsensor_data;
			}
		}
	}  				 									
	//USART_ClearITPendingBit(USART_COM, USART_IT_RXNE|USART_IT_TC);

}
/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
