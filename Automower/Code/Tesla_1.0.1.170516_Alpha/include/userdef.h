/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			userdef.h
  Author:				Raymond
  Date:				2017.6.8
  Version:        
  Description:		开发这自定义的宏  此文件在STM32F4XX.H里调用 

  					一些全局变量也可以放在这个文件里，整个工程都可以调用
  						
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述  
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/



#ifndef __USER_DEF_H__
#define __USER_DEF_H__

/* add by Raymond 2017.6.8---common file */
#include "userconfig.h"
#include "rtdef.h"
#include "delay.h"
#include "typedef.h"  // 把类型宏定义包含进来，确保兼容性


//#include "mower_common.h" 

typedef union{
	u16 halfword;
	u8 byte[2]; // MBS byte[0]
} T_u16_to_u8;

typedef union{
	u32 word;
	u16 halfword[2];
	u8 byte[4]; // MBS byte[0]
} T_u32_to_u8;


typedef struct 
{
	__IO unsigned int bit0:		1;
	__IO unsigned int bit1:		1;
	__IO unsigned int bit2:		1;
	__IO unsigned int bit3:		1;
	__IO unsigned int bit4:		1;
	__IO unsigned int bit5:		1;
	__IO unsigned int bit6:		1;
	__IO unsigned int bit7:		1;
	__IO unsigned int bit8:		1;
	__IO unsigned int bit9:		1;
	__IO unsigned int bit10:		1;
	__IO unsigned int bit11:		1;
	__IO unsigned int bit12:		1;
	__IO unsigned int bit13:		1;
	__IO unsigned int bit14:		1;
	__IO unsigned int bit15:		1;

}T_bit_field;

typedef union{
	__IO u32 word;
	__IO T_bit_field bitfield; // MBS byte[0]
} T_u32_to_bit;


#define P0_MASK	0x00000001u
#define P1_MASK	0x00000002u
#define P2_MASK	0x00000004u
#define P3_MASK	0x00000008u
#define P4_MASK	0x00000010u
#define P5_MASK	0x00000020u
#define P6_MASK	0x00000040u
#define P7_MASK	0x00000080u
#define P8_MASK	0x00000100u
#define P9_MASK	0x00000200u
#define P10_MASK	0x00000400u
#define P11_MASK	0x00000800u
#define P12_MASK	0x00001000u
#define P13_MASK	0x00002000u
#define P14_MASK	0x00004000u
#define P15_MASK	0x00008000u


/*****************port A*****************************/
#define GPIOA_IDR ((T_u32_to_bit *)(GPIOA_BASE+0x10))
#define	PA6_IN	(GPIOA_IDR->bitfield.bit6)
#define	PA7_IN	(GPIOA_IDR->bitfield.bit7)
#define	PA8_IN	(GPIOA_IDR->bitfield.bit8)
#define	PA9_IN	(GPIOA_IDR->bitfield.bit9)
#define	PA10_IN	(GPIOA_IDR->bitfield.bit10)
#define	PA11_IN	(GPIOA_IDR->bitfield.bit11)
#define	PA12_IN	(GPIOA_IDR->bitfield.bit12)
#define	PA13_IN	(GPIOA_IDR->bitfield.bit13)

#define GPIOA_ODR ((T_u32_to_bit *)(GPIOA_BASE+0x14))
#define	PA0_OUT	(GPIOA_ODR->bitfield.bit0)
#define	PA1_OUT	(GPIOA_ODR->bitfield.bit1)
#define	PA2_OUT	(GPIOA_ODR->bitfield.bit2)

#define	PA6_OUT	(GPIOA_ODR->bitfield.bit6)
#define	PA7_OUT	(GPIOA_ODR->bitfield.bit7)
#define	PA8_OUT	(GPIOA_ODR->bitfield.bit8)
#define	PA9_OUT	(GPIOA_ODR->bitfield.bit9)
#define	PA10_OUT	(GPIOA_ODR->bitfield.bit10)
#define	PA11_OUT	(GPIOA_ODR->bitfield.bit11)
#define	PA12_OUT	(GPIOA_ODR->bitfield.bit12)
#define	PA13_OUT	(GPIOA_ODR->bitfield.bit13)

/*****************port B*****************************/
#define GPIOB_IDR ((T_u32_to_bit *)(GPIOB_BASE+0x10))
#define	PB6_IN	(GPIOB_IDR->bitfield.bit6)
#define	PB7_IN	(GPIOB_IDR->bitfield.bit7)
#define	PB8_IN	(GPIOB_IDR->bitfield.bit8)
#define	PB9_IN	(GPIOB_IDR->bitfield.bit9)
#define	PB10_IN	(GPIOB_IDR->bitfield.bit10)
#define	PB11_IN	(GPIOB_IDR->bitfield.bit11)
#define	PB12_IN	(GPIOB_IDR->bitfield.bit12)
#define	PB13_IN	(GPIOB_IDR->bitfield.bit13)

#define GPIOB_ODR ((T_u32_to_bit *)(GPIOB_BASE+0x14))
#define	PB0_OUT	(GPIOB_ODR->bitfield.bit0)
#define	PB1_OUT	(GPIOB_ODR->bitfield.bit1)
#define	PB2_OUT	(GPIOB_ODR->bitfield.bit2)

#define	PB6_OUT	(GPIOB_ODR->bitfield.bit6)
#define	PB7_OUT	(GPIOB_ODR->bitfield.bit7)
#define	PB8_OUT	(GPIOB_ODR->bitfield.bit8)
#define	PB9_OUT	(GPIOB_ODR->bitfield.bit9)
#define	PB10_OUT	(GPIOB_ODR->bitfield.bit10)
#define	PB11_OUT	(GPIOB_ODR->bitfield.bit11)
#define	PB12_OUT	(GPIOB_ODR->bitfield.bit12)
#define	PB13_OUT	(GPIOB_ODR->bitfield.bit13)

/*****************port C*****************************/
//extern __IO T_u32_to_bit GPIOC_ODR @(GPIOC_BASE+0x14);
#define GPIOC_IDR ((T_u32_to_bit *)(GPIOC_BASE+0x10))
#define	PC6_IN	(GPIOC_IDR->bitfield.bit6)
#define	PC7_IN	(GPIOC_IDR->bitfield.bit7)
#define	PC8_IN	(GPIOC_IDR->bitfield.bit8)
#define	PC9_IN	(GPIOC_IDR->bitfield.bit9)
#define	PC10_IN	(GPIOC_IDR->bitfield.bit10)
#define	PC11_IN	(GPIOC_IDR->bitfield.bit11)
#define	PC12_IN	(GPIOC_IDR->bitfield.bit12)
#define	PC13_IN	(GPIOC_IDR->bitfield.bit13)

#define GPIOC_ODR ((T_u32_to_bit *)(GPIOC_BASE+0x14))
#define	PC6_OUT	(GPIOC_ODR->bitfield.bit6)
#define	PC7_OUT	(GPIOC_ODR->bitfield.bit7)
#define	PC8_OUT	(GPIOC_ODR->bitfield.bit8)
#define	PC9_OUT	(GPIOC_ODR->bitfield.bit9)
#define	PC10_OUT	(GPIOC_ODR->bitfield.bit10)
#define	PC11_OUT	(GPIOC_ODR->bitfield.bit11)
#define	PC12_OUT	(GPIOC_ODR->bitfield.bit12)
#define	PC13_OUT	(GPIOC_ODR->bitfield.bit13)

/*****************port I*****************************/
//extern __IO T_u32_to_bit GPIOI_ODR @(GPIOI_BASE+0x14);
#define GPIOI_ODR ((T_u32_to_bit *)(GPIOI_BASE+0x14))
#define	PI0	(GPIOI_ODR->bitfield.bit0)	
#define	PI1	(GPIOI_ODR->bitfield.bit1)
#define	PI2	(GPIOI_ODR->bitfield.bit2)
#define	PI3	(GPIOI_ODR->bitfield.bit3)
#define	PI4	(GPIOI_ODR->bitfield.bit4)
#define	PI5	(GPIOI_ODR->bitfield.bit5)
#define	PI6	(GPIOI_ODR->bitfield.bit6)
#define	PI7	(GPIOI_ODR->bitfield.bit7)
#define	PI8	(GPIOI_ODR->bitfield.bit8)
#define	PI9	(GPIOI_ODR->bitfield.bit9)
#define	PI10	(GPIOI_ODR->bitfield.bit10)
#define	PI11	(GPIOI_ODR->bitfield.bit11)
#define	PI12	(GPIOI_ODR->bitfield.bit12)
#define	PI13	(GPIOI_ODR->bitfield.bit13)
#define	PI14	(GPIOI_ODR->bitfield.bit14)
#define	PI15	(GPIOI_ODR->bitfield.bit15)
#define	PI16	(GPIOI_ODR->bitfield.bit16)
#define	PI17	(GPIOI_ODR->bitfield.bit17)
#define	PI18	(GPIOI_ODR->bitfield.bit18)
#define	PI19	(GPIOI_ODR->bitfield.bit19)
#define	PI20	(GPIOI_ODR->bitfield.bit20)
#define	PI21	(GPIOI_ODR->bitfield.bit21)
#define	PI22	(GPIOI_ODR->bitfield.bit22)
#define	PI23	(GPIOI_ODR->bitfield.bit23)
#define	PI24	(GPIOI_ODR->bitfield.bit24)
#define	PI25	(GPIOI_ODR->bitfield.bit25)
#define	PI26	(GPIOI_ODR->bitfield.bit26)
#define	PI27	(GPIOI_ODR->bitfield.bit27)
#define	PI28	(GPIOI_ODR->bitfield.bit28)
#define	PI29	(GPIOI_ODR->bitfield.bit29)
#define	PI30	(GPIOI_ODR->bitfield.bit30)
#define	PI31	(GPIOI_ODR->bitfield.bit31)


//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).M4同M3类似,只是寄存器地址变了.
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_ADDR    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_ADDR     (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_ADDR     (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_ADDR     (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_ADDR     (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_ADDR     (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_ADDR     (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_ADDR     (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_ADDR     (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_ADDR     (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_ADDR     (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_ADDR     (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_ADDR     (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_ADDR     (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_ADDR     (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_ADDR     (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_ADDR     (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_ADDR     (GPIOI_BASE+16) //0x40022010 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PA_OUT(n)   BIT_ADDR(GPIOA_ODR_ADDR,n)  //输出 
#define PA_IN(n)    BIT_ADDR(GPIOA_IDR_ADDR,n)  //输入 

#define PB_OUT(n)   BIT_ADDR(GPIOB_ODR_ADDR,n)  //输出 
#define PB_IN(n)    BIT_ADDR(GPIOB_IDR_ADDR,n)  //输入 

#define PC_OUT(n)   BIT_ADDR(GPIOC_ODR_ADDR,n)  //输出 
#define PC_IN(n)    BIT_ADDR(GPIOC_IDR_ADDR,n)  //输入 

#define PD_OUT(n)   BIT_ADDR(GPIOD_ODR_ADDR,n)  //输出 
#define PD_IN(n)    BIT_ADDR(GPIOD_IDR_ADDR,n)  //输入 

#define PE_OUT(n)   BIT_ADDR(GPIOE_ODR_ADDR,n)  //输出 
#define PE_IN(n)    BIT_ADDR(GPIOE_IDR_ADDR,n)  //输入

#define PF_OUT(n)   BIT_ADDR(GPIOF_ODR_ADDR,n)  //输出 
#define PF_IN(n)    BIT_ADDR(GPIOF_IDR_ADDR,n)  //输入

#define PG_OUT(n)   BIT_ADDR(GPIOG_ODR_ADDR,n)  //输出 
#define PG_IN(n)    BIT_ADDR(GPIOG_IDR_ADDR,n)  //输入

#define PH_OUT(n)   BIT_ADDR(GPIOH_ODR_ADDR,n)  //输出 
#define PH_IN(n)    BIT_ADDR(GPIOH_IDR_ADDR,n)  //输入

#define PI_OUT(n)   BIT_ADDR(GPIOI_ODR_ADDR,n)  //输出 
#define PI_IN(n)    BIT_ADDR(GPIOI_IDR_ADDR,n)  //输入


#endif


