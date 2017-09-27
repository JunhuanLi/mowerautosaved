/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			thread_ui.c
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
#include "rtdef.h"
#include "thread_app.h"
#include "mower_common.h"

#ifdef SUPPORT_LCD12864_ST7920
#include "lcd12864_parallel.h"
#endif

#ifdef SUPPORT_LCD12864_ST7565R
#include "lcd12864_io_spi.h"
#endif

// 测试按键延时
#include "key.h"
#include "menu.h"

/***映射的ram空间比实际大***/
u8 gui_ram[8][130]; // 虚拟lcd ram
u8 virtual_ram[8][130]; // 写入lcd ram 的映射内存

static t_form *focus_form = RT_NULL; 
t_form home_page;

void gui_data_init(void)
{
    gui.data.battery_state = 3;
    gui.data.gps_state = 3;
    gui.data.info = 2;
    gui.data.operating_mode = 2;
    gui.data.system_state = 9;
}

void label_2_gui_ram(t_label * label)
{
	u8 type;
	u8 x,y,offset_x,offset_y;
    u8 data;
	bool inversion;
	u8 i,j,k,n,m;
	u8 buff[5] = 0;

    u8 (*p_string)[30] = RT_NULL;

	const u8 (* p_icon_batt)[16] = RT_NULL;
    const u8(* p_icon_gps)[8] = RT_NULL;
	
	x = label->fix_x;
	y = label->fix_y;
	type = label->type;
	inversion = label->inversion;
	offset_x = label->offset_x;
	offset_y = label->offset_y;
    
    data = *(u8*)label->p_data;
	p_icon_batt =(const u8 (*)[16])label->p_icon;
    p_icon_gps =(const u8 (*)[8])label->p_icon;
    p_string = (u8 (*)[30])label->p_string;

	i = j = k = n = m = 0;

    //rt_kprintf("\r\n...type = %d...",type);
    //rt_kprintf("\r\n...x = %d...",x);
    //rt_kprintf("\r\n...y = %d...",y);
    //rt_kprintf("\r\n...offset_x = %d...",offset_x);
    //rt_kprintf("\r\n...offset_y = %d...",offset_y);
    //rt_kprintf("\r\n...data = %d...",data);

	switch(type)
	{
		case UI_DATA_TYPE_ICON_BATT:
			for(i = y;i < y+offset_y;i++)
			{
				for(j = x,n = 0;j < x+offset_x;j++,n++)
				{
					gui_ram[i][j] = p_icon_batt[data][n];
				}
			}
			break;
        case UI_DATA_TYPE_ICON_GPS:
			for(i = y;i < y+offset_y;i++)
			{
				for(j = x,n = 0;j < x+offset_x;j++,n++)
				{
					gui_ram[i][j] = p_icon_gps[data][n];
				}
			}
			break;
		case UI_DATA_TYPE_STRING:
			if(label->font == 0)// 8x16字体
			{
				while(label->p_string[i]>0x00)
				{
					if((label->p_string[i] >= 0x20)&&(label->p_string[i] <= 0x7e))
					{
						j=label->p_string[i]-0x20;
			
						for(n = 0;n < 2;n++)
						{
							for(k = 0;k < 8;k++)
							{
								if(inversion == 0)gui_ram[x+n][y+k] = ascii_table_8x16[j][k+8*n];
								else gui_ram[x+n][y+k] = ~ascii_table_8x16[j][k+8*n];
							}
						}
						i++;
						y += 8;
						
					}
					else
					i++;
				}
			}
			else if(label->font == 1)
			{
				for(n = 0;n < offset_x;n++)
				{ 
					if((p_string[data][n] >= 0x20)&&(p_string[data][n] <= 0x7e))
					{
						j=p_string[data][n]-0x20;

						for(k = 0;k < 6;k++)
						{
							if(inversion == 0)gui_ram[y][x+k] = ascii_table_6x8[j][k];
							else gui_ram[y][x+k] = ~ascii_table_6x8[j][k];
						}
                        x+=6;
					}
				}

			}

			break;
		case UI_DATA_TYPE_BYTE:
			buff[0] = *(u8 *)label->p_data + '0';
			while(label->p_string[i]>0x00)
			{
				if((label->p_string[i] >= 0x20)&&(label->p_string[i] <= 0x7e))
				{
					j=label->p_string[i]-0x20;

					for(k = 0;k < 5;k++)
					{
						if(inversion == 0)gui_ram[x][y+k] = ascii_table_6x8[j][k];
						else gui_ram[x][y+k] = ~ascii_table_6x8[j][k];
					}
						
					i++;
					y += 6;
				}
					else
						i++;
			}			
			break;
		default:
			
			break;
	}
}

/********************************************
 *描述:
 *      页面背景文字解析，在Updata函数中调用
 *      2017.6.24 ok
 *********************************************/
void back_text_parse(void)
{
	u8 i,j,k,m;
    u8 (*p_text)[27] = RT_NULL;

	static u8 x = 0,y = 0;

	p_text = (u8 (*)[27])focus_form->p_background_text;

	if(p_text == RT_NULL)
	{
		for(i = 0;i < 8;i++)
		{
			for(j = 0;j < 128;j++)
			{
				gui_ram[i][j] = 0;
			}
		}
	}	
	else
	{
		for(i = 0;i < 8;i++)
		{
            for(m = 0;m < 22;m++)//一行只要26个字符
            { 
			    if((p_text[i][m] >= 0x20)&&(p_text[i][m] <= 0x7e))
			    {
				    j = p_text[i][m] - 0x20;
		
				    for(k = 0;k < 6;k++)//每个字符的宽度
				    {
					    gui_ram[i][m*6+k] = ascii_table_6x8[j][k];
				    }
                 }
			}
		}	
	}
}

void text_box_parse(void)
{
	t_text_block *p_text_box = RT_NULL;
	u8 (*p_string)[30] = RT_NULL;

	bool flicker;
	static u8 count;
	static u8 move_count;	
	u8 data;
	u8 temp;
	u8 i;
		
	p_text_box = focus_form->p_text_block;
	
	
	while(p_text_box != RT_NULL)
	{
		// 每个text_box的自己参数都应该在while循环里读取，而不应该在while外读取
		data = * (u8 *)p_text_box->p_data;
		p_string = (u8 (*)[30])p_text_box->p_string;
		flicker = p_text_box->flicker;
#if 0		
		rt_kprintf("\r\n...name = %s...",p_text_box->name);
		if(rt_strcmp(p_text_box->name,"mode") == 0)
		{
			rt_kprintf("\r\n...data = %d...",data);
			rt_kprintf("\r\n...p_string = 0x%8x...",p_text_box->p_string);
			rt_kprintf("\r\n...p_str = 0x%8x...",p_string);
			rt_kprintf("\r\n...leng = %d...",(u8)strlen(p_text_box[data]));
		}
#endif
		// 不必是焦点 只要一行显示不下，就需要滚动
		
		if((u8)strlen(p_string[data]) > 15)
		{	
			move_count++;
			
			if(move_count%2 == 0)
			{
				temp = p_string[data][0];
				for(i= 0;i < (u8)strlen(p_string[data]);i++)
				{
					p_string[data][i] =  p_string[data][i+1];
				}
			
				p_string[data][i-1] = temp;
			}
		}
		
		if(flicker == 1) // 是否启动闪烁
		{
			if(focus_form->p_focus == p_text_box)
			{
				if(++count > 8)count = 0;
				if(count < 4)
				{
					p_text_box = p_text_box->p_next;
					continue;
				}
			}
		}

		label_2_gui_ram((t_label *)p_text_box);
		p_text_box = p_text_box->p_next;
	}
}

void label_parse(void)
{
	t_label *p_label = RT_NULL;
	static u8 info_move_count;
	static u8 state_move_count;
	
	u8 (*p_str)[30] = RT_NULL;
	u8 data;
	u8 temp,i;
	u8 move_enable;
	
	p_label = focus_form->p_label;

	// 不能再这里给P_str赋值 否则只能遍历一个label
	//if(p_label->p_string != RT_NULL)
		//p_str = (u8 (*)[30])p_label->p_string;
		
	while(p_label != RT_NULL)
	{
		data = *(u8 *)p_label->p_data;
		if(p_label->p_string != RT_NULL)
			p_str = (u8 (*)[30])p_label->p_string;
		
#if 0
		rt_kprintf("\r\n...name = %s...",p_label->name);
		if(rt_strcmp(p_label->name,"info") == 0)
		{
			rt_kprintf("\r\n...data = %d...",data);
			rt_kprintf("\r\n...p_string = 0x%8x...",p_label->p_string);
			rt_kprintf("\r\n...p_str = 0x%8x...",p_str);
			rt_kprintf("\r\n...leng = %d...",(u8)strlen(p_str[data]));
		}
#endif

		move_enable = 0;

		// 不必是焦点 只要一行显示不下，就需要滚动
		// p_str指针指向的字符串不能是const型
		if((u8)strlen((char *)p_str[data]) > 15)
		{	
			if(strcmp(p_label->name,"info") == 0)
			{
				info_move_count++;
				if(info_move_count%2 == 0)
					move_enable = 1;
			}
			
			if(strcmp(p_label->name,"state") == 0) 
			{
				state_move_count++;
				if(state_move_count%2 == 0)
					move_enable = 1;
			}
			
		}
		
		if(move_enable)
		{
			temp = p_str[data][0];
			for(i= 0;i < (u8)strlen(p_str[data]);i++)
			{
				p_str[data][i] =  p_str[data][i+1];
			}
		
			p_str[data][i-1] = temp;
		}

		label_2_gui_ram(p_label);
		p_label = p_label->p_next;
	}
}

void gui_update(void)
{
	u8 i,j;
	u8 flag;

	flag = 0;

	/***********检测某些区块是否有变化***************/
	for(i = 0;i < 8;i++)
	{
		for(j = 0;j < 128;j++)
		{
			if(gui_ram[i][j] != virtual_ram[i][j])
			{		
				flag = 1;
				virtual_ram[i][j] = gui_ram[i][j];
			}
		}
	}

	/****如果有数据变化整屏刷新*****/
	if(flag)
	{
		gui_function.display_string(virtual_ram);
	}
	
	
}


void form_parse(t_form *p_form)
{
		focus_form = p_form;

		back_text_parse();

		text_box_parse();

		label_parse();

		gui_update();
}

void label_add(t_form *p_form,t_label *p_label)
{
	t_label *pointer = RT_NULL;

	p_label->p_data = RT_NULL;

	
	if(p_form->p_label == RT_NULL)
	{
		p_form->p_label = p_label; // 如果页面的标签指针为空则直接把标签添加进来
		return;
	}
	
	pointer = p_form->p_label;
	
	while(pointer->p_next != RT_NULL)pointer = pointer ->p_next;
	pointer->p_next = p_label;
	p_label->p_pre = pointer;
	
}

void text_box_add(t_form *p_form,t_text_block *p_text_box)
{
	t_text_block *pointer = RT_NULL;

	p_text_box->p_data = RT_NULL;

	
	if(p_form->p_text_block== RT_NULL)
	{
		p_form->p_text_block = p_text_box; // 如果页面的标签指针为空则直接把标签添加进来
		//p_text_box->p_pre = p_text_box->p_next = p_text_box;
		return;
	}
	
	pointer = p_form->p_text_block;	
	
	//p_form->p_text_block->p_next = p_text_box;
	//p_text_box->p_pre = p_form->p_text_block;
	//p_text_box->p_next = pointer;
	
	while(pointer->p_next!= RT_NULL)pointer = pointer ->p_next;
	pointer->p_next = p_text_box;
	p_text_box->p_pre = pointer;
}

void text_box_focus_switch(void)
{
	if(focus_form->p_focus == RT_NULL)
	{
		focus_form->p_focus = focus_form->p_text_block;
		return;
	}

	focus_form->p_focus = focus_form->p_text_block->p_next;
}


void text_box_data_modify(key_enum key_value)
{
	u8 data_type;
	void *modify_data = RT_NULL;
	t_text_block *p_text_box = RT_NULL;

	rt_int8_t data;
	rt_int8_t data_max;
	rt_int8_t data_mix;
	rt_int8_t data_step_max;
	rt_int8_t data_step_mix;

	p_text_box = focus_form->p_focus;
	if(p_text_box == RT_NULL)return;
	
	data_type = p_text_box->type;
	modify_data = p_text_box->p_data;
	
	switch(data_type)
	{
		case UI_DATA_TYPE_ICON_BATT:
		case UI_DATA_TYPE_ICON_GPS:
		case UI_DATA_TYPE_STRING:
			
		case UI_DATA_TYPE_BYTE:
			data = *(u8 *)(modify_data);
			data_max = p_text_box->parameter_max;
            data_mix = p_text_box->parameter_mix;
			data_step_max = p_text_box->step_max;
			data_step_mix = p_text_box->step_mix;
			switch(key_value)
			{
				case K_UP:
					data -= data_step_mix;
					break;
				case K_DOWN:
					data += data_step_mix;
					break;
			}

			if(data > data_max)data = data_mix;
			if(data < data_mix)data = data_max;

			* (rt_int8_t *)(p_text_box->p_data) = data; 
			
			break;
		case UI_DATA_TYPE_INT:
			break;
		default:
			break;
	}
}


void form_init(t_form * p_form)
{
	p_form->p_background_text = RT_NULL;
	p_form->p_label = RT_NULL;
	p_form->p_focus= RT_NULL;
	p_form->p_next = RT_NULL;
	p_form->p_pre = RT_NULL;
	p_form->p_text_block = RT_NULL;
}

void gui_init(void)
{
	u8 i,j;

	/*********初始化内存*********/
	for(i = 0;i < 8;i++)
	{
		for(j = 0;j < 128;j++)
		{
			gui_ram[i][j] = 0;
			virtual_ram[i][j] = 0;	
		}
	}
	
	gui_function.form_parse = form_parse;
	gui_function.form_init = form_init;
	gui_function.label_add = label_add;
	gui_function.text_box_add = text_box_add;
	gui_function.display_string = all_dispaly;

    #ifdef CUSTOMER_SERIAL_DEBUG
    rt_kprintf("\r\n...Out gui_init()");
    #endif    
}



ALIGN(RT_ALIGN_SIZE)
char thread_ui_stack[512];
struct rt_thread thread_ui;

void mower_ui_thread(void* parameter)
{

   u32 event_rece;
	rt_err_t err;

   u8 cnt;

   const u8 tab[]="June 17th,2017";
   
   //led_test();

  // home_page = gui.menu.other_form;

   gui_menu_init();
   gui_data_init();
   form_parse(&gui.menu.home_form);
   text_box_focus_switch();
   
    while (1)
    {
        //rt_kprintf("\r\n...ui_thread_entry()");
        //i--;
        //if(i < 0)i = 0x3f;
       // transfer_command(0x40+i);
        
        // 这个函数出入的是字符串常量就不能再 函数里对字符串常量做写操作
        //display_string_5x8_gundong(5,17,tab);

        // 200ms做一次更新
#if 1
        //err = rt_event_recv(&sys_event,SYS_EVN_UI_UPDATA,RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR,RT_WAITING_FOREVER,&event_rece);
        if(sys_power_state)
        {		    
			#if 1
		    form_parse(&gui.menu.home_form);
		    cnt++;
		    if(cnt%9 == 0)
		{
        gui.data.gps_state++;
        if(gui.data.gps_state > 4)gui.data.gps_state = 0;
        
        gui.data.battery_state++;
        if(gui.data.battery_state > 5)gui.data.battery_state = 0;

		gui.data.system_state++;
        if(gui.data.system_state > 9)gui.data.system_state = 0;

		//gui.data.info++;
        //if(gui.data.info > 5)gui.data.info = 0;
		}
        #endif

        }
#endif
        //rt_kprintf("\r\n>>>ui_thread");
		
		rt_thread_sleep(200); /* sleep 0.5 second and switch to other thread */
    }
}

