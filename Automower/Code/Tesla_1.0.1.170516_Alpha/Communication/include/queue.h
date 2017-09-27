/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: queue.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/06/07
  Description	: queue 
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

#ifndef _QUEUE_H_
#define _QUEUE_H_

/* includes *******************************************************************/
#include "stm32f4xx.h"
#include "packet.h"


/* macros *********************************************************************/
#define MAX_SERIAL_SIZE		40
#define MAX_DOCK_IR_SIZE	30
#define MAX_REMOTE_SIZE		80

typedef struct
{
	TSerialPacket base[MAX_SERIAL_SIZE];
	s32 front;
	s32 rear;
}T_queue;

/* funcitons ******************************************************************/

T_bool init_queue(T_queue *queue);
void destroy_queue(T_queue *queue);
void clear_queue(T_queue *queue);
T_bool is_empty_queue(T_queue *queue);
int  length_of_queue(T_queue *queue);
T_bool insert_queue(T_queue *queue, TSerialPacket *element);
T_bool del_queue(T_queue *queue, TSerialPacket *element);

#endif

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
