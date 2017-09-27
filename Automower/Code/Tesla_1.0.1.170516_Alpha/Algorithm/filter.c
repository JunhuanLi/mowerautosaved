/*******************************************************************************

  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name		: filter.h
  Author		: klaus     
  Version		: V1.0.0      
  Date			: 2017/06/07
  Description	: filter
  
  History:        
                  
	1. Date			:
	   Author		:
	   Modification	:
	2. ...
    
*******************************************************************************/

/* includes *******************************************************************/
#include "filter.h"

/*******************************************************************************
  Function		: get_median_value      
  Description	: get median value
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
s32 get_median_value(s32 data[], s32 len)
{
	s32 i = 0, j = 0, temp = 0;
	for(i = 0; i < len -1; i++)
	{
		for(j = 0; j < len -1; j++)
		{
			if(data[j] > data[j + 1])
			{
				temp = data[j];
				data[j] = data[j+1];
				data[j+1] = temp;
			}	
		}
	}

	if((len % 2) == 1)
	{
		return data[(len - 1) / 2 ] ;	
	}
	else
	{
		return ((data[(len/2) - 1] + data[len/2]) / 2);

	}
}

/*******************************************************************************
  Function		: get_moving_average_filter_value      
  Description	: get moving average filter value
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
s32 get_moving_average_filter_value(T_moving_average_filter * filter, s32 data)
{

	filter->sum -=filter->filter_arrary[filter->tap];

	filter->filter_arrary[filter->tap] = data;

	filter->sum += filter->filter_arrary[filter->tap];

	filter->tap++;

	if(filter->tap >= filter->len)
	{
		filter->tap = 0;
	}

	return filter->sum / filter->len;
}
/*******************************************************************************
  Function		: get_median_filter_value      
  Description	: get median filter value
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
s32 get_median_filter_value(T_median_filter * filter, s32 data)
{
	filter->filter_arrary[filter->tap] =data;

	filter->tap++;

	if(filter->tap >= filter->len)
	{
		filter->tap = 0;
	}
	
	return get_median_value(filter->filter_arrary, filter->tap);
}

/*******************************************************************************
  Function		: get_fir_filter_value      
  Description	: get fir filter value
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
s32 get_fir_filter_value(T_fir_filter * filter, s32 data)
{
	s64 sum =0;
	s32 i = filter->len -1;
	s32 *pfilter = filter->filter_array;
	const s32 *pfactor = filter->factor_array;

	while(i--)
	{
		*pfilter = *(pfilter + 1);
		sum += ((s64)(*pfilter)) * ((s64)(*pfactor));

		pfilter++;
		pfactor++;
	}

	*pfilter =data;

	sum +=((s64)(*pfilter)) * ((s64)(*pfactor));
	return (sum >> 16);
}

/*******************************************************************************
  Function		: is_step_signal      
  Description	: is step signal
  Input			: None                    
  Output		: None       
  Return		: None       
*******************************************************************************/
T_bool is_step_signal(T_step_seeker * seeker, s32 data)
{
	s32 i = 0;
	s32 max = seeker->data_array[0];
	s32 min = seeker->data_array[0];

	seeker->data_array[seeker->tap] = data;

	seeker->tap++;

	if(seeker->tap >= seeker->len)
	{
		seeker->tap = 0;
	}

	for(i = 0; i < seeker->len; i++)
	{
		if(max < seeker->data_array[i])
		{
			max = seeker->data_array[i];	
		}

		if(min > seeker->data_array[i])
		{
			min = seeker->data_array[i];
		}	
	}

	if(max - min > seeker->threshold)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* Copyright (C), 2017, TOPBAND Robot Team ************************************/
