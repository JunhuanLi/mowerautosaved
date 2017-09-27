/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: queue.c
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

/* includes *******************************************************************/
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
  Function		: init_queue      
  Description	: init queue
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
T_bool init_queue(T_queue *queue)
{
	queue->front = queue->rear = 0;

	return TRUE;
}
/*******************************************************************************
  Function		: destroy_queue      
  Description	: destroy queue
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void destroy_queue(T_queue *queue)
{
	queue->front = queue->rear = 0;
}
/*******************************************************************************
  Function		: clear_queue      
  Description	: clear queue
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
void clear_queue(T_queue *queue)
{
	queue->front = queue->rear = 0;
}
/*******************************************************************************
  Function		: is_empty_queue      
  Description	: is empty queue
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
T_bool  is_empty_queue(T_queue *queue)
{
	if(queue->front == queue->rear)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
/*******************************************************************************
  Function		: length of queue      
  Description	: length of queue
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
int length_of_queue(T_queue *queue)
{
	return (queue->rear - queue->front + MAX_SERIAL_SIZE) % MAX_SERIAL_SIZE;
}
/*******************************************************************************
  Function		: insert_queue      
  Description	: insert queue
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
T_bool insert_queue(T_queue *queue, TSerialPacket *element)
{
	if((queue->rear + 1) % MAX_SERIAL_SIZE == queue->front)	
	{
		return FALSE;
	}
	
	queue->base[queue->rear] = *element;
	queue->rear = (queue->rear + 1)%MAX_SERIAL_SIZE;

	return TRUE;
}

/*******************************************************************************
  Function		: del_queue      
  Description	: del queue
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
T_bool del_queue(T_queue *queue, TSerialPacket *element)
{
	if(queue->front == queue->rear)
	{
		return FALSE;
	}

	*element = queue->base[queue->front];
	queue->front = (queue->front + 1) %MAX_SERIAL_SIZE;
	return TRUE;
}

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
