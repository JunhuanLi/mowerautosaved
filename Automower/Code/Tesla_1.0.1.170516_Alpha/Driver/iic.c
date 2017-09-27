/*******************************************************************************
Copyright (C), 2017, TOPBAND Robot Team
File name		:	ist8310.c
Author			:	klaus
Version			:	V1.0.0	
Date			:	2017/05/23
Description		:	ist8310 driiver
*******************************************************************************/

/* includes *******************************************************************/
#include "iic.h"
#include "time_control.h"

/* macros *********************************************************************/
typedef enum
{
	LOW = 0,
	HIGH
}T_level;


/* funcitons ******************************************************************/
static u8 get_iic_sda(void);
static void set_iic_sda(T_level level);
static void set_iic_scl(T_level level);
static void iic_sda_in(void);
static void iic_sda_out(void);

/*******************************************************************************
  Function		: get_iic_sda    
  Description	: get iic sda level
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void iic_delay(void)
{
	delay_systick(20);
}

static u8 get_iic_sda(void)
{
	return GPIO_ReadInputDataBit(I2C_SDA_PORT, I2C_SDA_PIN);
}

/*******************************************************************************
  Function		: set_iic_sda     
  Description	: set iic sda level
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void set_iic_sda(T_level level)
{
	if(level == LOW)
	{
		GPIO_ResetBits(I2C_SDA_PORT, I2C_SDA_PIN);
	}
	else
	{
		GPIO_SetBits(I2C_SDA_PORT, I2C_SDA_PIN);
	}
}

/*******************************************************************************
  Function		: set_iic_scl     
  Description	: set iic scl level
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void set_iic_scl(T_level level)
{
	if(level == LOW)
	{
		GPIO_ResetBits(I2C_SCL_PORT, I2C_SCL_PIN);
	}
	else
	{
		GPIO_SetBits(I2C_SCL_PORT, I2C_SCL_PIN);
	}
}

/*******************************************************************************
  Function		: iic_sda_in     
  Description	: set sda as in mode
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void iic_sda_in(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
  Function		: iic_sda_out     
  Description	: set sda as out mode
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void iic_sda_out(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
  Function		: iic_init     
  Description	: iic init
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void iic_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_AHB1PeriphClockCmd(I2C_SCL_CLK |  I2C_SDA_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_SCL_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);
	
    set_iic_sda(HIGH);	
    set_iic_scl(HIGH);	
}

/*******************************************************************************
  Function		: iic_start     
  Description	: iic start
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void iic_start(void)
{
	iic_sda_out() ;     
	set_iic_sda(HIGH);	  	  
	set_iic_scl(HIGH);
	iic_delay();
 	set_iic_sda(LOW);	
	iic_delay();
	set_iic_scl(LOW);
}	  

/*******************************************************************************
  Function		: iic_stop     
  Description	: iic stop
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void iic_stop(void)
{
	iic_sda_out() ;
	set_iic_scl(LOW);
	set_iic_sda(LOW);	
 	iic_delay();
	set_iic_scl(HIGH);
	set_iic_sda(HIGH);	
	iic_delay();							   	
}

/*******************************************************************************
  Function		: iic_wait_ack     
  Description	: iic wait ack
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
u8 iic_wait_ack(void)
{
	u8 ucErrTime=0;
	iic_sda_in() ;      
	set_iic_sda(HIGH);	
	iic_delay();	   
	set_iic_scl(HIGH);
	iic_delay();	 
	while(get_iic_sda())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			iic_stop();
			return 1;
		}
	}
	set_iic_scl(LOW);
	return 0;  
} 

/*******************************************************************************
  Function		: iic_ack     
  Description	: iic ack
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void iic_ack(void)
{
	set_iic_scl(LOW);
	iic_sda_out() ;
	set_iic_sda(LOW);	
	iic_delay();
	set_iic_scl(HIGH);
	iic_delay();
	set_iic_scl(LOW);
}

/*******************************************************************************
  Function		: iic_nack     
  Description	: iic nack
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/	    
void iic_nack(void)
{
	set_iic_scl(LOW);
	iic_sda_out() ;
	set_iic_sda(HIGH);	
	iic_delay();
	set_iic_scl(HIGH);
	iic_delay();
	set_iic_scl(LOW);
}		

/*******************************************************************************
  Function		: iic_send_byte     
  Description	: iic send byte
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/	  
void iic_send_byte(u8 txd)
{                        
    u8 t;   
	iic_sda_out() ; 	    
    set_iic_scl(LOW);
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
		{
			set_iic_sda(HIGH);	
		}
		else
		{
			set_iic_sda(LOW);	
		}
			
        txd<<=1; 	  
		iic_delay();   
		set_iic_scl(HIGH);
		iic_delay(); 
		set_iic_scl(LOW);
		iic_delay();
    }	 
} 	   

/*******************************************************************************
  Function		: iic_read_byte    
  Description	: iic read byte
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
u8 iic_read_byte(u8 ack)
{
	u8 i,receive=0;
	iic_sda_in() ;
  
	for(i=0;i<8;i++ )
	{
    set_iic_scl(LOW);
    iic_delay();
		set_iic_scl(HIGH);
    receive<<=1;
    if(get_iic_sda())
		{
			receive++;   
		}
		iic_delay(); 
  }					 
    if (!ack)
        iic_nack();
    else
        iic_ack(); 
    return receive;
}

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/

