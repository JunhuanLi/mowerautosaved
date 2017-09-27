/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			key.C
  Author:				Raymond
  Date:				2017.6.8
  Version:        
  Description:    // 用于详细说明此程序文件完成的主要功能，与其他模块
                  // 或函数的接口，输出值、取值范围、含义及参数间的控
                  // 制、顺序、独立或依赖等关系
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述  
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/

#include "stm32f4xx.h"
#include <rtthread.h>
#include "Key.h"


/* 按键表格 */
const mower_key_struct key_table[KEY_NUM_MAX]=
{
#ifdef SUPPORT_BOARD_VERSION_2
/* 1*/{K_NUM_1,     KEY_ATTR_SHORT, K_NULL},        
/* 2*/{K_NUM_2,     KEY_ATTR_SHORT, K_NULL},
/* 3*/{K_NUM_3,     KEY_ATTR_SHORT, K_NULL},
/* 4*/{K_NUM_4,     KEY_ATTR_SHORT, K_NULL},
/* 5*/{K_NUM_5,     KEY_ATTR_SHORT, K_POWER},
/* 6*/{K_NUM_6,     KEY_ATTR_SHORT, K_NULL},
/* 7*/{K_NUM_7,     KEY_ATTR_SHORT, K_NULL},        
/* 8*/{K_NUM_8,     KEY_ATTR_SHORT, K_NULL},
/* 9*/{K_NUM_9,     KEY_ATTR_SHORT, K_NULL},
/*10*/{K_NUM_0,     KEY_ATTR_SHORT, K_NULL},
/*11*/{K_UP,        KEY_ATTR_SHORT, K_NULL},
/*12*/{K_DOWN,      KEY_ATTR_SHORT, K_NULL},
/*13*/{K_HOME,      KEY_ATTR_SHORT, K_NULL},
/*14*/{K_START,     KEY_ATTR_SHORT, K_NULL},
/*15*/{K_OK,        KEY_ATTR_SHORT, K_NULL},
/*16*/{K_RETURN,    KEY_ATTR_SHORT, K_NULL},
/*17*/{K_POWER,		KEY_ATTR_LONG1, K_POWER},
/*18*/{K_STOP,      KEY_ATTR_SHORT, K_NULL},
/*19*/{K_NULL,      KEY_ATTR_SHORT, K_NULL},
/*20*/{K_NULL,      KEY_ATTR_SHORT, K_NULL},
/*21*/{K_NULL,		KEY_ATTR_SHORT, K_NULL},
/*22*/{K_NULL,      KEY_ATTR_SHORT, K_NULL},
/*23*/{K_NULL,      KEY_ATTR_SHORT, K_NULL},
/*24*/{K_NULL,      KEY_ATTR_SHORT, K_NULL},
/*25*/{K_NULL,		KEY_ATTR_SHORT, K_NULL},
#else
/* 0*/{K_NUM_5,     KEY_ATTR_SHORT, K_POWER},
/* 1*/{K_NUM_1,     KEY_ATTR_SHORT, K_NULL},        
/* 2*/{K_NUM_2,     KEY_ATTR_SHORT, K_NULL},
/* 3*/{K_NUM_3,     KEY_ATTR_SHORT, K_NULL},
/* 4*/{K_NUM_4,     KEY_ATTR_SHORT, K_NULL},
/* 5*/{K_NUM_0,     KEY_ATTR_SHORT, K_NULL},
/* 6*/{K_NUM_6,     KEY_ATTR_SHORT, K_NULL},
/* 7*/{K_NUM_7,     KEY_ATTR_SHORT, K_NULL},        
/* 8*/{K_NUM_8,     KEY_ATTR_SHORT, K_NULL},
/* 9*/{K_NUM_9,     KEY_ATTR_SHORT, K_NULL},
/*10*/{K_OK,        KEY_ATTR_SHORT, K_NULL},
/*11*/{K_UP,        KEY_ATTR_SHORT, K_NULL},
/*12*/{K_DOWN,      KEY_ATTR_SHORT, K_NULL},
/*13*/{K_HOME,      KEY_ATTR_SHORT, K_NULL},
/*14*/{K_START,     KEY_ATTR_SHORT, K_NULL},        
/*15*/{K_NULL,      KEY_ATTR_SHORT, K_NULL},
/*16*/{K_RETURN,    KEY_ATTR_SHORT, K_NULL},
/*17*/{K_POWER,		KEY_ATTR_LONG1, K_POWER},
/*18*/{K_STOP,      KEY_ATTR_SHORT, K_NULL},
/*19*/{K_NULL,      KEY_ATTR_SHORT, K_NULL},
/*20*/{K_NULL,		KEY_ATTR_SHORT, K_NULL},
/*21*/{K_NULL,      KEY_ATTR_SHORT, K_NULL},
/*22*/{K_NULL,      KEY_ATTR_SHORT, K_NULL},
/*23*/{K_NULL,      KEY_ATTR_SHORT, K_NULL},
/*24*/{K_NULL,		KEY_ATTR_SHORT, K_NULL},
#endif
};

#ifndef SUPPORT_KEY_REGISTER
u8 key_long_flag;
#else
struct rt_device mower_key_device;
rt_device_t _key_device = RT_NULL;
#endif

void key_hw_config(void)
{
#if 0
	GPIO_InitTypeDef gpio_conf;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	gpio_conf.GPIO_Mode = GPIO_Mode_OUT;
	gpio_conf.GPIO_OType = GPIO_OType_PP;
	gpio_conf.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	gpio_conf.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_conf.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOF,&gpio_conf);

	gpio_conf.GPIO_Mode = GPIO_Mode_IN;
	gpio_conf.GPIO_OType = GPIO_OType_PP;
	gpio_conf.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	gpio_conf.GPIO_PuPd = GPIO_PuPd_DOWN;
	gpio_conf.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&gpio_conf);
#endif

#ifdef SUPPORT_KEY_REGISTER
    rt_hw_key_register(&mower_key_device, "d_key",RT_DEVICE_FLAG_RDONLY, &t_key_m);
#endif
}



void key_variable_init(void)
{

}




/*********************************
 * 按键扫描,返回对应的按键号
 *********************************/
u8 key_scan(void)
{
	u8 i;
	u16 temp;
    u8 index;
    static u8 index_pre;

    index = 0xff;
    
    rt_enter_critical();
    for(i = 0;i < ROW_MAX;i++)
    {
        temp = 1 << i;
        /* 赋值给输出IO */
        ROW_1 = (temp & 0x0f) >> 0;
        ROW_2 = (temp & 0x0f) >> 1;
        ROW_3 = (temp & 0x0f) >> 2;
        ROW_4 = (temp & 0x0f) >> 3;
        /* 短暂延时 */
        systick_us_delay(2);
        /* 根据读回来的数据计算出按键号，并返回给后台 */
        if(COLUMN_1 == 1)
        {
            index = (i * COLUMN_MAX) + 0;
        }
        if(COLUMN_2 == 1)
        {
            index = (i * COLUMN_MAX) + 1;
        }
        if(COLUMN_3 == 1)
        {
            index = (i * COLUMN_MAX) + 2;
        }
        if(COLUMN_4 == 1)
        {
            index = (i * COLUMN_MAX) + 3;
        }  
        if(COLUMN_5 == 1)
        {
            index = (i * COLUMN_MAX) + 4;
        }
    }
    rt_exit_critical();
    #ifdef CUSTOMER_SERIAL_DEBUG
    if(index_pre != index)
    {
        index_pre = index;
        rt_kprintf("\r\n...Key index=%2d...",index);
    }
	#endif

    return index;
}


#ifndef SUPPORT_KEY_REGISTER
/***************************************
 * function : check key value
 * input: 
 *
 ***************************************/
u8 key_check(const mower_key_struct *key_table,u8 *pre_index,u8 index,u8 *key_step,volatile u16 *press_10ms_cnt)
{
	u8 key_value = K_NULL;

	key_long_flag = 0;

	if(index != INDEX_CURRENT_INVALID)
	{
		if((*pre_index == INDEX_CURRENT_INVALID) || *pre_index != index)
		{
			*press_10ms_cnt = 0;
			*key_step = 0;
			*pre_index = index;
		}
		else if(*press_10ms_cnt != KEY_PRESS_TIME_LOCK)
		{
			if(((mower_key_struct *)key_table)[*pre_index].attribute == KEY_ATTR_SHORT)
			{
				if(*press_10ms_cnt > KEY_PRESS_TIME_SHORT)
				{
					key_value = ((mower_key_struct *)key_table)[*pre_index].short_value;
					*press_10ms_cnt = KEY_PRESS_TIME_LOCK;

					#ifdef CUSTOMER_SERIAL_DEBUG
					rt_kprintf("\r\n...Key press short..key_value=0x%2x...",key_value);
					#endif
				}
			}
			else if(((mower_key_struct *)key_table)[*pre_index].attribute == KEY_ATTR_CONTINUE)
			{
				if(*key_step == 0)
				{
					if(*press_10ms_cnt > KEY_PRESS_TIME_SHORT)
					{
						key_value = ((mower_key_struct *)key_table)[*pre_index].short_value;
						*press_10ms_cnt = 0;
						*key_step = 1;
						#ifdef CUSTOMER_SERIAL_DEBUG
						rt_kprintf("\r\n...Key press continue 0..key_value=0x%2x...",key_value);
						#endif

					}
				}
				else if(*key_step == 1)
				{
					if(*press_10ms_cnt > KEY_PRESS_TIME_CONTINUE)
					{
						key_value = ((mower_key_struct *)key_table)[*pre_index].short_value;
						*press_10ms_cnt = 0;
						*key_step = 2;
						#ifdef CUSTOMER_SERIAL_DEBUG
						rt_kprintf("\r\n...Key press continue 1..key_value=0x%2x...",key_value);
						#endif
					}
				}
				else if(*key_step == 2)
				{
					if(*press_10ms_cnt > KEY_PRESS_TIME_CONTINUE_SECOND)
					{
						key_value = ((mower_key_struct *)key_table)[*pre_index].short_value;
						*press_10ms_cnt = 0;
						// 连续按着就会一直进这里赋值按键值
						//*key_step = 2;
						#ifdef CUSTOMER_SERIAL_DEBUG
						rt_kprintf("\r\n...Key press continue 2..key_value=0x%2x...",key_value);
						#endif
					}					
				}
			}
			else if(((mower_key_struct *)key_table)[*pre_index].attribute == KEY_ATTR_LONG1)
			{
				if(*press_10ms_cnt > KEY_PRESS_TIME_LONG1)
				{
					key_value = ((mower_key_struct *)key_table)[*pre_index].short_value;
					*press_10ms_cnt = KEY_PRESS_TIME_LOCK;
					key_long_flag = 1;
					#ifdef CUSTOMER_SERIAL_DEBUG
					rt_kprintf("\r\n...Key long press 1..key_value=0x%2x...",key_value);
					#endif
				}				
			}
			else if(((mower_key_struct *)key_table)[*pre_index].attribute == KEY_ATTR_LONG2)
			{
				if(*press_10ms_cnt > KEY_PRESS_TIME_LONG2)
				{
					key_value = ((mower_key_struct *)key_table)[*pre_index].short_value;
					*press_10ms_cnt = KEY_PRESS_TIME_LOCK;
					key_long_flag = 1;
					#ifdef CUSTOMER_SERIAL_DEBUG
					rt_kprintf("\r\n...Key long press 2..key_value=0x%2x...",key_value);
					#endif
				}

			}

		}
	}
	else
	{
		if((*pre_index != INDEX_CURRENT_INVALID) && *press_10ms_cnt != KEY_PRESS_TIME_LOCK)
		{
			if(((mower_key_struct *)key_table)[*pre_index].attribute == KEY_ATTR_LONG1 
				|| ((mower_key_struct *)key_table)[*pre_index].attribute == KEY_ATTR_LONG2)
			{
				if(*press_10ms_cnt >= KEY_PRESS_TIME_SHORT)
				{
					key_value = ((mower_key_struct *)key_table)[*pre_index].short_value;
					#ifdef CUSTOMER_SERIAL_DEBUG
					rt_kprintf("\r\n...Key release..key_value=0x%2x...",key_value);
					#endif
				}
			}
		}

		*press_10ms_cnt = 0;
		*key_step = 0;
		*pre_index = INDEX_CURRENT_INVALID;
	}
	return key_value;	
}



#else // 使用注册方式

//************************************************************
//*********************以下规范化驱动代码*********************
//************************************************************

u8 key_check_d(const mower_key_struct *key_table,mower_key_dev * key_m)
{
	u8 key_value = K_NULL;

	key_m->key_long_flag = 0;

	if(key_m->key_index != INDEX_CURRENT_INVALID)
	{
		if((key_m->key_index_pre == INDEX_CURRENT_INVALID) || key_m->key_index_pre != key_m->key_index)
		{
			key_m->press_10ms_cnt = 0;
			key_m->key_step = 0;
			key_m->key_index_pre = key_m->key_index;
		}
		else if(key_m->press_10ms_cnt != KEY_PRESS_TIME_LOCK)
		{
			if(((mower_key_struct *)key_table)[key_m->key_index_pre].attribute == KEY_ATTR_SHORT)
			{
				if(key_m->press_10ms_cnt > KEY_PRESS_TIME_SHORT)
				{
					key_value = ((mower_key_struct *)key_table)[key_m->key_index_pre].short_value;
					key_m->press_10ms_cnt = KEY_PRESS_TIME_LOCK;

					#ifdef CUSTOMER_SERIAL_DEBUG
					rt_kprintf("\r\n...Key press short..key_value=0x%2x...",key_value);
					#endif
				}
			}
			else if(((mower_key_struct *)key_table)[key_m->key_index_pre].attribute == KEY_ATTR_CONTINUE)
			{
				if(key_m->key_step == 0)
				{
					if(key_m->press_10ms_cnt > KEY_PRESS_TIME_SHORT)
					{
						key_value = ((mower_key_struct *)key_table)[key_m->key_index_pre].short_value;
						key_m->press_10ms_cnt = 0;
						key_m->key_step = 1;
						#ifdef CUSTOMER_SERIAL_DEBUG
						rt_kprintf("\r\n...Key press continue 0..key_value=0x%2x...",key_value);
						#endif

					}
				}
				else if(key_m->key_step == 1)
				{
					if(key_m->press_10ms_cnt > KEY_PRESS_TIME_CONTINUE)
					{
						key_value = ((mower_key_struct *)key_table)[key_m->key_index_pre].short_value;
						key_m->press_10ms_cnt = 0;
						key_m->key_step = 2;
						#ifdef CUSTOMER_SERIAL_DEBUG
						rt_kprintf("\r\n...Key press continue 1..key_value=0x%2x...",key_value);
						#endif
					}
				}
				else if(key_m->key_step == 2)
				{
					if(key_m->press_10ms_cnt > KEY_PRESS_TIME_CONTINUE_SECOND)
					{
						key_value = ((mower_key_struct *)key_table)[key_m->key_index_pre].short_value;
						key_m->press_10ms_cnt = 0;
						// 连续按着就会一直进这里赋值按键值
						//*key_step = 2;
						#ifdef CUSTOMER_SERIAL_DEBUG
						rt_kprintf("\r\n...Key press continue 2..key_value=0x%2x...",key_value);
						#endif
					}					
				}
			}
			else if(((mower_key_struct *)key_table)[key_m->key_index_pre].attribute == KEY_ATTR_LONG1)
			{
				if(key_m->press_10ms_cnt > KEY_PRESS_TIME_LONG1)
				{
					key_value = ((mower_key_struct *)key_table)[key_m->key_index_pre].short_value;
					key_m->press_10ms_cnt = KEY_PRESS_TIME_LOCK;
					key_m->key_long_flag = 1;
					#ifdef CUSTOMER_SERIAL_DEBUG
					rt_kprintf("\r\n...Key long press 1..key_value=0x%2x...",key_value);
					#endif
				}				
			}
			else if(((mower_key_struct *)key_table)[key_m->key_index_pre].attribute == KEY_ATTR_LONG2)
			{
				if(key_m->press_10ms_cnt > KEY_PRESS_TIME_LONG2)
				{
					key_value = ((mower_key_struct *)key_table)[key_m->key_index_pre].short_value;
					key_m->press_10ms_cnt = KEY_PRESS_TIME_LOCK;
					key_m->key_long_flag = 1;
					#ifdef CUSTOMER_SERIAL_DEBUG
					rt_kprintf("\r\n...Key long press 2..key_value=0x%2x...",key_value);
					#endif
				}

			}

		}
	}
	else
	{
		if((key_m->key_index_pre != INDEX_CURRENT_INVALID) && key_m->press_10ms_cnt != KEY_PRESS_TIME_LOCK)
		{
			if(((mower_key_struct *)key_table)[key_m->key_index_pre].attribute == KEY_ATTR_LONG1 
				|| ((mower_key_struct *)key_table)[key_m->key_index_pre].attribute == KEY_ATTR_LONG2)
			{
				if(key_m->press_10ms_cnt >= KEY_PRESS_TIME_SHORT)
				{
					key_value = ((mower_key_struct *)key_table)[key_m->key_index_pre].short_value;
					#ifdef CUSTOMER_SERIAL_DEBUG
					rt_kprintf("\r\n...Key release..key_value=0x%2x...",key_value);
					#endif
				}
			}
		}

		key_m->press_10ms_cnt = 0;
		key_m->key_step = 0;
		key_m->key_index_pre = INDEX_CURRENT_INVALID;
	}
	return key_value;	
}


/*******************************************************
 ** 初始化按键设备变量
 *******************************************************
 **/
mower_key_dev t_key_m =
{
    0,
    0,
    0,
    0,
    0,
    0,
    
    key_scan,
    key_check_d,
};



// 参数初始化
static rt_err_t rt_key_init(rt_device_t dev)
{
    if(!(dev->flag & RT_DEVICE_FLAG_ACTIVATED))
    {
        // 此处放置变量初始化
        dev->flag |= RT_DEVICE_FLAG_ACTIVATED;
    }

    return RT_EOK;
}

rt_err_t rt_hw_key_register(rt_device_t device, const char* name, rt_uint32_t flag, mower_key_dev *key_m)
{
    
    
	RT_ASSERT(device != RT_NULL);

    device->device_id = 0x02;
    
	device->type 		= RT_Device_Class_Char;
	device->rx_indicate = RT_NULL;
	device->tx_complete = RT_NULL;
	device->init 		= rt_key_init;
	device->open		= RT_NULL;
	device->close		= RT_NULL;
	device->read 		= RT_NULL;
	device->write 		= RT_NULL;
	device->control 	= RT_NULL;
	device->user_data	= key_m;

	/* register a character device */
	return rt_device_register(device, name, RT_DEVICE_FLAG_RDONLY|flag);
}



rt_device_t rt_key_device_set(const char *name)
{
    rt_device_t new, old;

    /* save old device */
    old = _key_device;

    /* find new console device */
    new = rt_device_find(name);
    if (new != RT_NULL)
    {
        if (_key_device != RT_NULL)
        {
            /* close old console device */
            rt_device_close(_key_device);
        }

        /* set new console device */
        rt_device_open(new, RT_DEVICE_OFLAG_RDONLY);
        _key_device = new;
    }

    return old;
	
}

#endif



