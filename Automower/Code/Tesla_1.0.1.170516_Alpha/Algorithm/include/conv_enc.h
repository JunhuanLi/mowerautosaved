/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name 	: conv_enc.h
  Author		: 	
  Version		: V1.0.0	  
  Date			: 2017/05/27
  Description	: ekf.h 
  
  History:		  
				  
	1. Date 		:
	   Author		:
	   Modification :
	2. ...
	
*******************************************************************************/
#ifndef __CONV_ENC_H__
#define __CONV_ENC_H__


#include "stm32f4xx.h"
#include <rtthread.h>
#include <stdio.h>
#include "insgps.h"
#include "imu.h"
#include "usart.h"
#include "encoder.h"
#include "debug.h"


/*****functions*********/
void bin2hex( char *input, char *output);
void conv_enc_tx(float data_tx);
void ch_enc(double data_raw, boolean_T dataEnc_data[], int dataEnc_size[2]);
static void convenc(const boolean_T msg_data[], const int msg_size[2], boolean_T varargout_1_data[], int varargout_1_size[2]);


#endif
/* Copyright (C), 2017, TOPBAND Robot Team ************************************/

