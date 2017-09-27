/*******************************************************************************
Copyright (C), 2017, TOPBAND Robot Team
File name		:	ist8310.c
Author			:	klaus
Version			:	V1.0.0	
Date			:	2017/05/23
Description		:	ist8310 driiver
*******************************************************************************/

/* includes *******************************************************************/
#include "ist83102.h"
#include "iic.h"
#include "time_control.h"
#include <rtthread.h>

/* macros *********************************************************************/
//#define MAG_SLAVE_ADDRESS		0x18  

/* slave address */
/*	0  0  0x0C   */	
/*	0  1  0x0D   */	
/*	1  0  0x0E   */	
/*  1  1  0x0F   */ 	

/*	read only	*/
#define WHO_AM_I			0x00
#define STATUS_REGISTER_1	0x02
#define STATUS_REGISTER_2	0x09

#define MAG_X_L				0x03
#define MAG_X_H				0x04
#define MAG_Y_L				0x05
#define MAG_Y_H				0x06
#define MAG_Z_L				0x07
#define MAG_Z_H				0x08

#define TEMPERATURE_L		0x1C
#define TEMPERATURE_H		0x1D

/*	R/W	*/
#define CONTROL_REGISTER_1	0x0A
#define CONTROL_REGISTER_2	0x0B

/* static variables ***********************************************************/
static T_mag g_mag = {0};

/* funcitons ******************************************************************/
static u8 ist8310_iic2_write_register(u8 slave_address, u8 register_address, u8 value);
static u8 ist8310_iic2_read_register(u8 slave_address, u8 register_address, u8 *ret_value);

static void update_mag(void);

/*******************************************************************************
  Function		: ist8310_iic_write_register     
  Description	: write register
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static u8 ist8310_iic2_write_register(u8 slave_address, u8 register_address, u8 value)
{
	iic_start(); 
	iic_send_byte(slave_address); 		
    if(iic_wait_ack())          	    
    {
        iic_stop();
        return 1;
    }
    iic_send_byte(register_address);     
    iic_wait_ack();            			
    iic_send_byte(value);        		
    if(iic_wait_ack())          		
    {
        iic_stop();
        return 1;
    }
    iic_stop();
    return 0;
}

/*******************************************************************************
  Function		: ist8310_iic2_read_register     
  Description	: read register
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static u8 ist8310_iic2_read_register(u8 slave_address, u8 register_address, u8 *ret_value)
{
    u8 res;
    iic_start();
	iic_send_byte(slave_address);			
    iic_wait_ack();            			
    iic_send_byte(register_address);      
    iic_wait_ack();            			
	iic_start();                
	iic_send_byte(slave_address+1); 		 
    iic_wait_ack();             			
    res=iic_read_byte(0);					
    iic_stop();               		
    *ret_value = res;
	return 0;
}

/*******************************************************************************
  Function		: mag_init     
  Description	: mag init
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
T_bool mag_init2(void)
{
	u8 id = 0xFF;
	
	iic_init();	
    delay_ms(50);
	ist8310_iic2_write_register(MAG2_SLAVE_ADDRESS, 0x41, 0x24);
	ist8310_iic2_write_register(MAG2_SLAVE_ADDRESS, 0x42, 0xC0);
	
//	ist8310_iic2_read_register(LEFT_MAG_I2C, MAG2_SLAVE_ADDRESS, WHO_AM_I, &id);

//	if(id != 10)
//	{
//		return FALSE;
//	}
//	else
//	{
		return TRUE;
//	}
}

/*******************************************************************************
  Function		: get_left_mgx    
  Description	: get left mgx
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void update_mag2(void)
{
	u8 data[2] = {0};

	ist8310_iic2_write_register(MAG2_SLAVE_ADDRESS, 0x0A, 0x01);

    delay_ms(6);
	ist8310_iic2_read_register(MAG2_SLAVE_ADDRESS, MAG_X_L, &data[0]);
	ist8310_iic2_read_register(MAG2_SLAVE_ADDRESS, MAG_X_H, &data[1]);
	g_mag.mag_x = (s16)(data[1] << 8 | data[0]);

	ist8310_iic2_read_register(MAG2_SLAVE_ADDRESS, MAG_Y_L, &data[0]);
	ist8310_iic2_read_register(MAG2_SLAVE_ADDRESS, MAG_Y_H, &data[1]);
	g_mag.mag_y = (s16)(data[1] << 8 | data[0]);

	ist8310_iic2_read_register(MAG2_SLAVE_ADDRESS, MAG_Z_L, &data[0]);
	ist8310_iic2_read_register(MAG2_SLAVE_ADDRESS, MAG_Z_H, &data[1]);
	g_mag.mag_z = (s16)(data[1] << 8 | data[0]);

	ist8310_iic2_read_register(MAG2_SLAVE_ADDRESS, TEMPERATURE_L, &data[0]);
	ist8310_iic2_read_register(MAG2_SLAVE_ADDRESS, TEMPERATURE_H, &data[1]);
	g_mag.temperature = (s16)(data[1] << 8 | data[0]);
}

/*------------------------------------------------------------------------------
Function		:	getBatteryInfo
Description		:	reset Battery
Input			:	None
Output			: 	None
Return			:	None
-------------------------------------------------------------------------------*/
void get_mag_info2(T_mag *mag)
{
	update_mag2();

//	mag->mag_x = g_mag.mag_x;
//	mag->mag_y = g_mag.mag_y;
//	mag->mag_z = g_mag.mag_z;
	mag->mag_x = g_mag.mag_y;
	mag->mag_y = -g_mag.mag_x;
	mag->mag_z = -g_mag.mag_z;
	mag->temperature = g_mag.temperature;
}

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
