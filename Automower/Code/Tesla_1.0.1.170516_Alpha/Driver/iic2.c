/*******************************************************************************
Copyright (C), 2017, TOPBAND Robot Team
File name		:	ist8310.c
Author			:	klaus
Version			:	V1.0.0	
Date			:	2017/05/23
Description		:	ist8310 driiver
*******************************************************************************/

/* includes *******************************************************************/
#include "iic2.h"
#include "time_control.h"

/* macros *********************************************************************/
typedef enum
{
	LOW = 0,
	HIGH
}T_level;

/* funcitons ******************************************************************/
static u8 get_iic2_sda(void);
static void set_iic2_sda(T_level level);
static void set_iic2_scl(T_level level);
static void iic2_sda_in(void);
static void iic2_sda_out(void);

/*******************************************************************************
  Function		: get_iic2_sda    
  Description	: get iic2 sda level
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static u8 get_iic2_sda(void)
{
	return GPIO_ReadInputDataBit(I2C2_SDA_PORT, I2C2_SDA_PIN);
}

/*******************************************************************************
  Function		: set_iic2_sda     
  Description	: set iic2 sda level
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void set_iic2_sda(T_level level)
{
	if(level == LOW)
	{
		GPIO_ResetBits(I2C2_SDA_PORT, I2C2_SDA_PIN);
	}
	else
	{
		GPIO_SetBits(I2C2_SDA_PORT, I2C2_SDA_PIN);
	}
}

/*******************************************************************************
  Function		: set_iic2_scl     
  Description	: set iic2 scl level
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void set_iic2_scl(T_level level)
{
	if(level == LOW)
	{
		GPIO_ResetBits(I2C2_SCL_PORT, I2C2_SCL_PIN);
	}
	else
	{
		GPIO_SetBits(I2C2_SCL_PORT, I2C2_SCL_PIN);
	}
}

/*******************************************************************************
  Function		: iic2_sda_in     
  Description	: set sda as in mode
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void iic2_sda_in(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = I2C2_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C2_SDA_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
  Function		: iic2_sda_out     
  Description	: set sda as out mode
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void iic2_sda_out(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = I2C2_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C2_SDA_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
  Function		: iic2_init     
  Description	: iic2 init
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void iic2_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_AHB1PeriphClockCmd(I2C2_SCL_CLK |  I2C2_SDA_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = I2C2_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C2_SCL_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = I2C2_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C2_SDA_PORT, &GPIO_InitStructure);
	

    set_iic2_sda(HIGH);	
    set_iic2_scl(HIGH);	
}

/*******************************************************************************
  Function		: iic2_start     
  Description	: iic2 start
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void iic2_start(void)
{
	iic2_sda_out() ;     
	set_iic2_sda(HIGH);	  	  
	set_iic2_scl(HIGH);
	delay_us(4);
 	set_iic2_sda(LOW);	
	delay_us(4);
	set_iic2_scl(LOW);
}	  

/*******************************************************************************
  Function		: iic2_stop     
  Description	: iic2 stop
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void iic2_stop(void)
{
	iic2_sda_out() ;
	set_iic2_scl(LOW);
	set_iic2_sda(LOW);	
 	delay_us(4);
	set_iic2_scl(HIGH);
	set_iic2_sda(HIGH);	
	delay_us(4);							   	
}

/*******************************************************************************
  Function		: iic2_wait_ack     
  Description	: iic2 wait ack
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
u8 iic2_wait_ack(void)
{
	u8 ucErrTime=0;
	iic2_sda_in() ;      
	set_iic2_sda(HIGH);	
	delay_us(1);	   
	set_iic2_scl(HIGH);
	delay_us(1);	 
	while(get_iic2_sda())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			iic2_stop();
			return 1;
		}
	}
	set_iic2_scl(LOW);
	return 0;  
} 

/*******************************************************************************
  Function		: iic2_ack     
  Description	: iic2 ack
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void iic2_ack(void)
{
	set_iic2_scl(LOW);
	iic2_sda_out() ;
	set_iic2_sda(LOW);	
	delay_us(2);
	set_iic2_scl(HIGH);
	delay_us(2);
	set_iic2_scl(LOW);
}

/*******************************************************************************
  Function		: iic2_nack     
  Description	: iic2 nack
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/	    
void iic2_nack(void)
{
	set_iic2_scl(LOW);
	iic2_sda_out() ;
	set_iic2_sda(HIGH);	
	delay_us(2);
	set_iic2_scl(HIGH);
	delay_us(2);
	set_iic2_scl(LOW);
}		

/*******************************************************************************
  Function		: iic2_send_byte     
  Description	: iic2 send byte
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/	  
void iic2_send_byte(u8 txd)
{                        
    u8 t;   
	iic2_sda_out() ; 	    
    set_iic2_scl(LOW);
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
		{
			set_iic2_sda(HIGH);	
		}
		else
		{
			set_iic2_sda(LOW);	
		}
			
        txd<<=1; 	  
		delay_us(2);   
		set_iic2_scl(HIGH);
		delay_us(2); 
		set_iic2_scl(LOW);
		delay_us(2);
    }	 
} 	   

/*******************************************************************************
  Function		: iic2_read_byte    
  Description	: iic2 read byte
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
u8 iic2_read_byte(u8 ack)
{
	u8 i,receive=0;
	iic2_sda_in() ;
    for(i=0;i<8;i++ )
	{
        set_iic2_scl(LOW);
        delay_us(2);
		set_iic2_scl(HIGH);
        receive<<=1;
        if(get_iic2_sda())receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        iic2_nack();
    else
        iic2_ack(); 
    return receive;
}

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/

