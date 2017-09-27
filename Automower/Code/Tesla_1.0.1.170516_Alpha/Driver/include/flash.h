/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: bumper_front.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/05/17
  Description	: bumper front   
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/


#ifndef _FLASH_H_
#define _FLASH_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"



/* macros *********************************************************************/
typedef struct
{
	s32 start_address;
	s32 end_address;
	s32 max_length;
}T_address_flash;

/* funcitons ******************************************************************/
T_bool write_flash_word(T_address_flash flash_addr, u32 write_address, u32 data);
T_bool read_flash(T_address_flash flash_addr, u32 read_address, u32 *data);

T_bool write_flash_buf(T_address_flash flash_addr, u32 write_address, u32 * data);
T_bool read_flash_buf(T_address_flash flash_addr, u32 read_address, u32 * data);

T_bool write_flash_buf_s32(T_address_flash flash_addr, u32 write_address, s32 * data);
T_bool read_flash_buf_s32(T_address_flash flash_addr, u32 read_address, s32 * data);

#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
