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


/* includes *******************************************************************/
#include "flash.h"
#include "stm32f4xx_flash.h"

/* funcitons ******************************************************************/
static u32 get_sector(uint32_t address);

/* macros *********************************************************************/
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base address of Sector 0, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base address of Sector 1, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base address of Sector 2, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base address of Sector 3, 16 Kbytes   */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base address of Sector 4, 64 Kbytes   */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base address of Sector 5, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base address of Sector 6, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base address of Sector 7, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base address of Sector 8, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base address of Sector 9, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base address of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base address of Sector 11, 128 Kbytes */

#define ADDR_FLASH_SECTOR_12     ((uint32_t)0x08100000) /* Base address of Sector 12, 16 Kbytes  */
#define ADDR_FLASH_SECTOR_13     ((uint32_t)0x08104000) /* Base address of Sector 13, 16 Kbytes  */
#define ADDR_FLASH_SECTOR_14     ((uint32_t)0x08108000) /* Base address of Sector 14, 16 Kbytes  */
#define ADDR_FLASH_SECTOR_15     ((uint32_t)0x0810C000) /* Base address of Sector 15, 16 Kbytes  */
#define ADDR_FLASH_SECTOR_16     ((uint32_t)0x08110000) /* Base address of Sector 16, 64 Kbytes  */
#define ADDR_FLASH_SECTOR_17     ((uint32_t)0x08120000) /* Base address of Sector 17, 128 Kbytes */
#define ADDR_FLASH_SECTOR_18     ((uint32_t)0x08140000) /* Base address of Sector 18, 128 Kbytes */
#define ADDR_FLASH_SECTOR_19     ((uint32_t)0x08160000) /* Base address of Sector 19, 128 Kbytes */
#define ADDR_FLASH_SECTOR_20     ((uint32_t)0x08180000) /* Base address of Sector 20, 128 Kbytes */
#define ADDR_FLASH_SECTOR_21     ((uint32_t)0x081A0000) /* Base address of Sector 21, 128 Kbytes */
#define ADDR_FLASH_SECTOR_22     ((uint32_t)0x081C0000) /* Base address of Sector 22, 128 Kbytes */
#define ADDR_FLASH_SECTOR_23     ((uint32_t)0x081E0000) /* Base address of Sector 23, 128 Kbytes */

/*******************************************************************************
  Function		: write_flash_word      
  Description	: write_flash_word
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
T_bool write_flash_word(T_address_flash flash_addr, u32 write_address, u32 data)
{
	u32 flash_sector = 0;
	volatile FLASH_Status status = FLASH_COMPLETE;
		
	if(write_address > flash_addr.max_length)
	{
		return FALSE;
	}

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
					FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	
	flash_sector = get_sector(flash_addr.start_address);

	status = FLASH_EraseSector(flash_sector, VoltageRange_2);

	if(status != FLASH_COMPLETE)
	{
		return FALSE;
	}

	status = FLASH_ProgramWord((u32)(flash_addr.start_address + write_address), data);
	
	if(status != FLASH_COMPLETE)
	{
		return FALSE;
	}
	
	FLASH_Lock();
	return TRUE;
}
/*******************************************************************************
  Function		: read_flash_word      
  Description	: read_flash_word
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
T_bool read_flash_word(T_address_flash flash_addr, u32 read_address, u32 *data)
{
	u32 address = flash_addr.start_address + read_address;
	if(read_address > flash_addr.max_length)
	{
		return FALSE;
	}

	*data = *((__IO u32*)(address));
	return TRUE;
}
/*******************************************************************************
  Function		: write_flash_buf      
  Description	: write_flash_buf
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
T_bool write_flash_buf(T_address_flash flash_addr, u32 write_address, u32 * data)
{
	volatile FLASH_Status status = FLASH_COMPLETE;
	u32 flash_sector = 0;
	u32 i = 0;
	u32 size = sizeof(data) / sizeof(data[0]) ;
 
	if((write_address +size) > flash_addr.max_length)
	{
		return FALSE;
	}

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
					FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	
	flash_sector = get_sector(flash_addr.start_address);

	status = FLASH_EraseSector(flash_sector, VoltageRange_2);

	if(status != FLASH_COMPLETE)
	{
		return FALSE;
	}

	for(i = 0; i < size; i++)
	{
		if(status == FLASH_COMPLETE)
		{
			u32 address =flash_addr.start_address + write_address + 4 * i;
			status = FLASH_ProgramWord(address, (u32)(*(data + i)));
		}
	}
		
	if(status != FLASH_COMPLETE)
	{
		return FALSE;
	}
	
	FLASH_Lock();
	return TRUE;
}

/*******************************************************************************
  Function		: read_flash_buf      
  Description	: read_flash_buf
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
T_bool read_flash_buf(T_address_flash flash_addr, u32 read_address, u32 *data)
{
	u32 i = 0;
	u32 size = sizeof(data) / sizeof(data[0]) ;

	if((read_address +size) > flash_addr.max_length)
	{
		return FALSE;
	}

	for(i = 0; i < size; i++)
	{
		u32 address = read_address + 4 * i;
		read_flash_word(flash_addr, address, (u32 *)(data + i));
	}
	return TRUE;
}

/*******************************************************************************
  Function		: write_flash_buf_s32      
  Description	: write_flash_buf_s32
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
T_bool write_flash_buf_s32(T_address_flash flash_addr, u32 write_address, s32 * data)
{
	volatile FLASH_Status status = FLASH_COMPLETE;
	u32 flash_sector = 0;
	u32 i = 0;
	u32 size = sizeof(data) / sizeof(data[0]) ;
 
	if((write_address +size) > flash_addr.max_length)
	{
		return FALSE;
	}

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
					FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
	
	flash_sector = get_sector(flash_addr.start_address);

	status = FLASH_EraseSector(flash_sector, VoltageRange_2);

	if(status != FLASH_COMPLETE)
	{
		return FALSE;
	}

	for(i = 0; i < size; i++)
	{
		if(status == FLASH_COMPLETE)
		{
			u32 value  = (s32)(*(data + i));
			u32 address =flash_addr.start_address + write_address + 4 * i;
			status = FLASH_ProgramWord(address, value);
		}
	}
		
	if(status != FLASH_COMPLETE)
	{
		return FALSE;
	}
	
	FLASH_Lock();
	return TRUE;
}


/*******************************************************************************
  Function		: read_flash_buf_s32      
  Description	: read_flash_buf_s32
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
T_bool read_flash_buf_s32(T_address_flash flash_addr, u32 read_address, s32 *data)
{
	u32 i = 0;
	u32 size = sizeof(data) / sizeof(data[0]);

	if((read_address +size) > flash_addr.max_length)
	{
		return FALSE;
	}

	for(i = 0; i < size; i++)
	{
		u32 value = 0;
		u32 address = read_address + 4 * i;
		
		read_flash_word(flash_addr, address, &value);
		*(data + i) = (s32)value;
	}
	return TRUE;
}
/*******************************************************************************
  Function		: get_sector      
  Description	: get_sector
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static u32 get_sector(uint32_t address)
{
  u32 sector = 0;
  
  if((address < ADDR_FLASH_SECTOR_1) && (address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((address < ADDR_FLASH_SECTOR_2) && (address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((address < ADDR_FLASH_SECTOR_3) && (address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((address < ADDR_FLASH_SECTOR_4) && (address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((address < ADDR_FLASH_SECTOR_5) && (address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((address < ADDR_FLASH_SECTOR_6) && (address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((address < ADDR_FLASH_SECTOR_7) && (address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((address < ADDR_FLASH_SECTOR_8) && (address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((address < ADDR_FLASH_SECTOR_9) && (address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((address < ADDR_FLASH_SECTOR_10) && (address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((address < ADDR_FLASH_SECTOR_11) && (address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  

  else if((address < ADDR_FLASH_SECTOR_12) && (address >= ADDR_FLASH_SECTOR_11))
  {
    sector = FLASH_Sector_11;  
  }

  else if((address < ADDR_FLASH_SECTOR_13) && (address >= ADDR_FLASH_SECTOR_12))
  {
    sector = FLASH_Sector_12;  
  }
  else if((address < ADDR_FLASH_SECTOR_14) && (address >= ADDR_FLASH_SECTOR_13))
  {
    sector = FLASH_Sector_13;  
  }
  else if((address < ADDR_FLASH_SECTOR_15) && (address >= ADDR_FLASH_SECTOR_14))
  {
    sector = FLASH_Sector_14;  
  }
  else if((address < ADDR_FLASH_SECTOR_16) && (address >= ADDR_FLASH_SECTOR_15))
  {
    sector = FLASH_Sector_15;  
  }
  else if((address < ADDR_FLASH_SECTOR_17) && (address >= ADDR_FLASH_SECTOR_16))
  {
    sector = FLASH_Sector_16;  
  }
  else if((address < ADDR_FLASH_SECTOR_18) && (address >= ADDR_FLASH_SECTOR_17))
  {
    sector = FLASH_Sector_17;  
  }
  else if((address < ADDR_FLASH_SECTOR_19) && (address >= ADDR_FLASH_SECTOR_18))
  {
    sector = FLASH_Sector_18;  
  }
  else if((address < ADDR_FLASH_SECTOR_20) && (address >= ADDR_FLASH_SECTOR_19))
  {
    sector = FLASH_Sector_19;  
  }
  else if((address < ADDR_FLASH_SECTOR_21) && (address >= ADDR_FLASH_SECTOR_20))
  {
    sector = FLASH_Sector_20;  
  } 
  else if((address < ADDR_FLASH_SECTOR_22) && (address >= ADDR_FLASH_SECTOR_21))
  {
    sector = FLASH_Sector_21;  
  }
  else if((address < ADDR_FLASH_SECTOR_23) && (address >= ADDR_FLASH_SECTOR_22))
  {
    sector = FLASH_Sector_22;  
  }
  else
  {
    sector = FLASH_Sector_23;  
  }
  return sector;
}
/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
