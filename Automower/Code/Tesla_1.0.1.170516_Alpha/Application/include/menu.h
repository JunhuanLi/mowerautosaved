



#ifndef __MENU_H__
#define __MENU_H__

#include "stm32f4xx.h"
#include "lcd12864_io_spi.h"
#include "key.h"

/***********显示内容的格式***************/
typedef enum ui_data_type_self
{
	UI_DATA_TYPE_ICON_BATT, // 图标显示
	UI_DATA_TYPE_ICON_GPS,
	UI_DATA_TYPE_STRING, // 小号字体 5*8
	UI_DATA_TYPE_INT, // 
	UI_DATA_TYPE_BYTE, // 

    UI_DATA_TYPE_NULL = 0xff,
}t_ui_data_type;

typedef enum
{
	false = 0,	
	ture = 1,
}bool;



/***********显示参数*************/
typedef struct parameter_type_self
{
	rt_int8_t battery_state;	// 电池状态参数
	rt_int8_t gps_state;		// gps状态参数
	rt_int8_t operating_mode; 	// 工作模式参数
	rt_int8_t info;			// 状态信息
	rt_int8_t system_state;
}t_parameter_type;



/***********标签结构体***************/
typedef struct label_self
{
	char *name;
	u8 fix_x; 				// 显示区域的横坐标
	u8 fix_y; 				// 显示区域的纵坐标
	u8 offset_x;				// 区域需要的列数量
	u8 offset_y;
	
	bool inversion;			// 反向
	bool flicker;			// 闪烁 
		
	t_ui_data_type type;	// 显示数据的类型
	u8 font;				// 字体大小
	u8 other;               // 供用户使用的其他参数
	
	u8 *p_string;		// 文本显示字符串指针
	const u8 *p_icon;			// 图标数据地址
	void * p_data; 	// 数据指针
	struct label_self *p_pre; // 指向上一text_block的指针 
	struct label_self *p_next; // 指向下一text_block的指针
}t_label;


/*********文本块结构体*****************/
typedef struct text_block_self
{	
	char *name;
	u8 fix_x; 				// 显示区域的横坐标
	u8 fix_y; 				// 显示区域的纵坐标
	u8 offset_x;				// 需要的列数量
	u8 offset_y;				// 需要的列数量
	
	bool inversion;			// 反向
	bool flicker;			// 闪烁 
	
	t_ui_data_type type;	// 显示数据的类型
	u8 font;				// 字体大小
	u8 other;               // 供用户使用的其他参数
	
	u8 *p_string;		// 文本显示字符串指针
	const u8 *p_icon;			// 图标数据地址
	void * p_data; 	// 数据指针
	struct text_block_self *p_pre; // 指向上一text_block的指针 
	struct text_block_self *p_next; // 指向下一text_block的指针
	
	u8 parameter_max; // 参数最大值
	u8 parameter_mix; // 参数最小值
	u8 step_mix; // 参数细调的步进
	u8 step_max; // 参数粗调的步进
}t_text_block;


/**********页面数据结构体***********/
typedef struct form_self
{
	u8 * p_background_text; // 背景文字数据地址指针
	t_text_block *p_text_block;// 文本显示块
	t_label *p_label;// 标签指针
	struct form_self *p_pre; // 前一页面指针
	struct form_self *p_next; // 后一页面指针
	t_text_block *p_focus; // 当前页
}t_form;


/*********GUI用到的函数集合********************/
typedef struct function_self
{
	void (* form_parse)(t_form *p_form);
	void (* form_init)(t_form * p_form);
	void (* label_add)(t_form *p_form,t_label *p_label);
	void (* text_box_add)(t_form *p_form,t_text_block *p_text_box);
	void (* text_box_focus_switch)(t_form * p_form,t_text_block * p_text_box);// 切换焦点文本
	void (* text_box_data_modify)(key_enum key_value); // 改变焦点文本值
	void (* display_string)(u8 string[][130]);
}t_function_struct;

/**********菜单结构体***********/
typedef struct menu_self
{
	t_form logo_form;
	t_form welcome_form;
	t_form home_form;
	t_form other_form;
	t_form *p_fouce_form;
}t_menu;

/********整个GUI包含的数据机构********/
typedef struct gui_self
{
	t_parameter_type data;
	t_menu menu; 
}t_gui_struct;



extern t_gui_struct gui; 
extern t_function_struct gui_function;



void gui_menu_init(void);


#endif


