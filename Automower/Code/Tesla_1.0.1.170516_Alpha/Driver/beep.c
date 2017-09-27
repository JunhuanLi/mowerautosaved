 /*******************************************************************************
 
   Copyright (C), 2017-2027, TOPBAND Co., Ltd.
   File name	 : beep.c
   Author		 : klaus	 
   Version		 : V1.0.0	   
   Date 		 : 2017/05/17
   Description	 : beep
   
   History: 	   
				   
	 1. Date		 :
		Author		 :
		Modification :
	 2. ...
	 
 *******************************************************************************/


/* includes *******************************************************************/
#include "beep.h"
//#include "mower_common.h"
/* macros *********************************************************************/
#define SUPPORT_NEW_BEEP
/* static variables ***********************************************************/
static T_bool g_beep_state = FALSE;

FunctionalState beep_flag;
struct BEEP_TAG beep;


/* funcitons ******************************************************************/
static void beep_gpio_configuration(void);


/*******************************************************************************
  Function		: beep_gpio_configuration      
  Description	: beep_gpio_configuration
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
static void beep_gpio_configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(BEEP_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
  Function		: beep_init      
  Description	: beep init
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void beep_init(void)
{
	beep_gpio_configuration();
}



#ifdef SUPPORT_NEW_BEEP
/****************************************
 **  state: enable OR disable
 **  attr:  one_pass OR circul OR continue
 **  period: hold time for beep on 
 ****************************************
 **/

#ifdef SUPPORT_BOARD_VERSION_2
void beep_ctrl(struct BEEP_TAG * beep,FunctionalState state,e_beep_attr attr,u32 period)
{
    beep->state = state;

    if(beep->state == ENABLE)
    {
        GPIO_SetBits(BEEP_PORT, BEEP_PIN);
        beep->attr = attr;
        beep->period = period;
    }
    else
    {
        GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
        return;
    }   
}
#else
void beep_ctrl(FunctionalState state,e_beep_attr attr,u32 period)
{
    beep.state = state;
    if(beep.state == DISABLE) // disable beep
    {
        return;
    }
    beep.attr = attr;
    beep.period = period;
}
#endif
/**********************************************
 **  Only referenced by soft_timeout_10ms()
 **********************************************
 **/
void beep_moniter(e_beep_stru_t p)
{
    u8 beep_disable;
    static u8 flag = 0;
    static u32 temp;
    static e_beep_attr attr;
        
    beep_disable = 0;
    
    if(attr != p->attr)
    {
       attr = p->attr;
       temp =0;
    }
    
    switch(p->attr)
    {
        case BEEP_ATTR_SINGLE_PASS:
           if(p->state == ENABLE)
           {
               temp++;
               if(temp >= (p->period))
               {
                  temp = 0;
                  beep_disable = 1;
               }
           }
           else
           {
               beep_disable = 1;
           }
           break;
                
       case BEEP_ATTR_CIRCUL:
          temp++;
          if(temp%(p->period) == 0)
             flag^=1;

          if(flag)
             beep_disable = 1;
          else
             p->state = ENABLE;
          break;
              
       case BEEP_ATTR_CONTINUE:
          beep_disable = 0;
          break;
                
       default:
          break;
     }
    
    if(beep_disable)
    {
		#ifdef SUPPORT_BOARD_VERSION_2
        beep_ctrl(&beep,DISABLE,BEEP_ATTR_SINGLE_PASS,0);
        #else
		beep_ctrl(DISABLE,BEEP_ATTR_SINGLE_PASS,0);
        #endif
    }
}

#endif
/*******************************************************************************
  Function		: enable_beep      
  Description	: enable beep
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void enable_beep(void)
{
#ifdef SUPPORT_NEW_BEEP
    beep_flag = ENABLE;
#else
	if(!g_beep_state)
	{
		g_beep_state = TRUE;
		GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
	}
#endif
}


/*******************************************************************************
  Function		: get_cover_switch_info      
  Description	: get cover switch info
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void disable_beep(void)
{
#ifdef SUPPORT_NEW_BEEP
    beep_flag = DISABLE;
#else
	if(g_beep_state)
	{
		g_beep_state = FALSE;
		GPIO_SetBits(BEEP_PORT, BEEP_PIN);
	}
#endif
}



/*******************************************************************************
  Function		: get_cover_switch_info      
  Description	: get cover switch info
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void get_beep_info(T_bool *beep)
{
	*beep = g_beep_state;
}
/* Copyright (C), 2017-2027, TOPBAND Co., Ltd. ********************************/
