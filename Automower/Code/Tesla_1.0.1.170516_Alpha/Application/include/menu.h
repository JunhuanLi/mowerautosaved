



#ifndef __MENU_H__
#define __MENU_H__

#include "stm32f4xx.h"
#include "lcd12864_io_spi.h"
#include "key.h"

/***********��ʾ���ݵĸ�ʽ***************/
typedef enum ui_data_type_self
{
	UI_DATA_TYPE_ICON_BATT, // ͼ����ʾ
	UI_DATA_TYPE_ICON_GPS,
	UI_DATA_TYPE_STRING, // С������ 5*8
	UI_DATA_TYPE_INT, // 
	UI_DATA_TYPE_BYTE, // 

    UI_DATA_TYPE_NULL = 0xff,
}t_ui_data_type;

typedef enum
{
	false = 0,	
	ture = 1,
}bool;



/***********��ʾ����*************/
typedef struct parameter_type_self
{
	rt_int8_t battery_state;	// ���״̬����
	rt_int8_t gps_state;		// gps״̬����
	rt_int8_t operating_mode; 	// ����ģʽ����
	rt_int8_t info;			// ״̬��Ϣ
	rt_int8_t system_state;
}t_parameter_type;



/***********��ǩ�ṹ��***************/
typedef struct label_self
{
	char *name;
	u8 fix_x; 				// ��ʾ����ĺ�����
	u8 fix_y; 				// ��ʾ�����������
	u8 offset_x;				// ������Ҫ��������
	u8 offset_y;
	
	bool inversion;			// ����
	bool flicker;			// ��˸ 
		
	t_ui_data_type type;	// ��ʾ���ݵ�����
	u8 font;				// �����С
	u8 other;               // ���û�ʹ�õ���������
	
	u8 *p_string;		// �ı���ʾ�ַ���ָ��
	const u8 *p_icon;			// ͼ�����ݵ�ַ
	void * p_data; 	// ����ָ��
	struct label_self *p_pre; // ָ����һtext_block��ָ�� 
	struct label_self *p_next; // ָ����һtext_block��ָ��
}t_label;


/*********�ı���ṹ��*****************/
typedef struct text_block_self
{	
	char *name;
	u8 fix_x; 				// ��ʾ����ĺ�����
	u8 fix_y; 				// ��ʾ�����������
	u8 offset_x;				// ��Ҫ��������
	u8 offset_y;				// ��Ҫ��������
	
	bool inversion;			// ����
	bool flicker;			// ��˸ 
	
	t_ui_data_type type;	// ��ʾ���ݵ�����
	u8 font;				// �����С
	u8 other;               // ���û�ʹ�õ���������
	
	u8 *p_string;		// �ı���ʾ�ַ���ָ��
	const u8 *p_icon;			// ͼ�����ݵ�ַ
	void * p_data; 	// ����ָ��
	struct text_block_self *p_pre; // ָ����һtext_block��ָ�� 
	struct text_block_self *p_next; // ָ����һtext_block��ָ��
	
	u8 parameter_max; // �������ֵ
	u8 parameter_mix; // ������Сֵ
	u8 step_mix; // ����ϸ���Ĳ���
	u8 step_max; // �����ֵ��Ĳ���
}t_text_block;


/**********ҳ�����ݽṹ��***********/
typedef struct form_self
{
	u8 * p_background_text; // �����������ݵ�ַָ��
	t_text_block *p_text_block;// �ı���ʾ��
	t_label *p_label;// ��ǩָ��
	struct form_self *p_pre; // ǰһҳ��ָ��
	struct form_self *p_next; // ��һҳ��ָ��
	t_text_block *p_focus; // ��ǰҳ
}t_form;


/*********GUI�õ��ĺ�������********************/
typedef struct function_self
{
	void (* form_parse)(t_form *p_form);
	void (* form_init)(t_form * p_form);
	void (* label_add)(t_form *p_form,t_label *p_label);
	void (* text_box_add)(t_form *p_form,t_text_block *p_text_box);
	void (* text_box_focus_switch)(t_form * p_form,t_text_block * p_text_box);// �л������ı�
	void (* text_box_data_modify)(key_enum key_value); // �ı佹���ı�ֵ
	void (* display_string)(u8 string[][130]);
}t_function_struct;

/**********�˵��ṹ��***********/
typedef struct menu_self
{
	t_form logo_form;
	t_form welcome_form;
	t_form home_form;
	t_form other_form;
	t_form *p_fouce_form;
}t_menu;

/********����GUI���������ݻ���********/
typedef struct gui_self
{
	t_parameter_type data;
	t_menu menu; 
}t_gui_struct;



extern t_gui_struct gui; 
extern t_function_struct gui_function;



void gui_menu_init(void);


#endif


