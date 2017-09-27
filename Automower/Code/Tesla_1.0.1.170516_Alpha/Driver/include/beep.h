/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: beep.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/05/17
  Description	: beep 
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

#ifndef _BEEP_H_
#define _BEEP_H_


/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "typedef.h"

/* macros *********************************************************************/
#define BEEP_PORT										GPIOH
#define	BEEP_PIN										GPIO_Pin_2
#define	BEEP_CLK										RCC_AHB1Periph_GPIOH


// beep enum
typedef enum BEEP_ATTR_TAG
{
 /*01*/ BEEP_ATTR_SINGLE_PASS ,//circulation
 /*02*/ BEEP_ATTR_CIRCUL,
 /*03*/ BEEP_ATTR_CONTINUE,

 /*04*/ BEEP_ATTR_TOTAL,
}e_beep_attr;

struct BEEP_TAG
{   
    e_beep_attr attr;
    u32 period; 
    FunctionalState state;
};
typedef struct BEEP_TAG * e_beep_stru_t;

extern struct BEEP_TAG beep;
extern FunctionalState beep_flag;

#ifdef SUPPORT_BOARD_VERSION_2
void beep_ctrl(struct BEEP_TAG * beep,FunctionalState state,e_beep_attr attr,u32 period);
#else
void beep_ctrl(FunctionalState state,e_beep_attr attr,u32 period);
#endif
void beep_moniter(e_beep_stru_t p);


/* funcitons ******************************************************************/
void beep_init(void);
void reset_beep(void);
void get_beep_info(T_bool *beep);

void disable_beep(void);
void enable_beep(void);

#endif

/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
