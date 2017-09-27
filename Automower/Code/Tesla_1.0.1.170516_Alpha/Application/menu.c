


#include "stm32f4xx.h"
//#include "thread_ui.h"
#include "lcd12864_io_spi.h"

#include "menu.h"

t_gui_struct gui; 
t_function_struct gui_function;


/***8x5字库***/
u8  home_page_background[8][27]=
{
    "                          ",
    "                          ",
    "Mode :                    ",
    "                          ",
    "Info :                    ",
    "                          ",
    "State:                    ",
    "                          ",
};

u8  other_page_background[8][27]=
{
    "                          ",
    "                          ",
    "                          ",
    "                          ",
    "                          ",
    "                          ",
    "                          ",
    "                          "
};

u8  logo_page_background[8][27]=
{
    "                          ",
    "                          ",
    "                          ",
    "       TOPBAND            ",
    "                          ",
    "                          ",
    "                          ",
    "                          "
};

u8 option_mode[][30]=
{
    {"Automatic mode           "},
    {"Spot cutting"},
    {"Work time schedule       "},
    {"testMode4"},
    {"testMode5"},
    {"testMode6"},
    {"testMode7"},
    {"testMode8"},
    {"testMode9"},
    {"testMode10"},
};

u8 option_state[][30]=
{
    {"          Aaa    "},
    {"          Bbb    "},
    {"          Ccc    "},
    {"          Ddd    "},
    {"          Eee    "},
    {"          Fff    "},
    {"          Ggg    "},
    {"          Hhh    "},
    {"          Iii    "},
    {"          Jjj    "},
};

u8 option_info[][30]=
{
    {"No loop Signal"},
    {"Outside the boundary     "},
    {"Blade motor overloaded     "},
    {"Wheel motor overloaded     "},
    {"Lifted up"},
    {"Too steep"},
    {"Battery very low     "},
    {"Trapped  "},
    {"Back home  "},
    {" "},
};

static void logo_form_init(void)
{
	gui_function.form_init(&gui.menu.logo_form);
	gui.menu.logo_form.p_background_text = 	*logo_page_background;
}

static void welcome_form_init(void)
{
	gui_function.form_init(&gui.menu.welcome_form);	
	gui.menu.welcome_form.p_background_text = RT_NULL;
}


static void home_form_init(void)
{
	static t_label label_battery;
	static t_label label_gps;
	static t_label label_info;
	static t_label label_system_state;

	static t_text_block t_text_box;
	
	gui_function.form_init(&gui.menu.home_form);
	
	gui.menu.home_form.p_background_text = *home_page_background;
	
	gui_function.label_add(&gui.menu.home_form,&label_battery);
	label_battery.name = "battery";
	label_battery.fix_x = 0; // 0行
	label_battery.fix_y = 0; // 0列
	label_battery.offset_x = 16;
    label_battery.offset_y = 1;
	label_battery.inversion = false; // 不取反显示
	label_battery.flicker = false; // 不闪烁
	label_battery.font = 0;
	label_battery.type = UI_DATA_TYPE_ICON_BATT;// 显示小图片
    label_battery.other = 0;
	label_battery.p_data =(void *)(&gui.data.battery_state);
	label_battery.p_icon = *changer_8x16;// icon of battery
	label_battery.p_string = RT_NULL;

	gui_function.label_add(&gui.menu.home_form,&label_gps);
	label_gps.name = "gps";
	label_gps.fix_x = 24;		// 列
	label_gps.fix_y = 0;		// 行
	label_gps.offset_x = 8;
    label_gps.offset_y = 1;
	label_gps.inversion = false;
	label_gps.flicker = false;
	label_gps.font = 0;
	label_gps.type = UI_DATA_TYPE_ICON_GPS;// 显示小图片
	label_gps.p_data = (void *)(&gui.data.gps_state);
	label_gps.p_icon = *gps_icon_8x8;
	label_gps.p_string = RT_NULL;

	gui_function.text_box_add(&gui.menu.home_form,&t_text_box);
	t_text_box.name = "mode";
	t_text_box.fix_x = 36;		// 3行
	t_text_box.fix_y = 2;		// 64列
	t_text_box.offset_x = 15;
    t_text_box.offset_y = 0;
	t_text_box.inversion = false;
	t_text_box.flicker = false;
	t_text_box.font = 1; // 0: 8x16字体   1: 5x8字体
	t_text_box.type = UI_DATA_TYPE_STRING; // 字体显示
	t_text_box.p_data = (void *)(&gui.data.operating_mode);
	t_text_box.p_icon = RT_NULL;
	t_text_box.p_string = *option_mode;	
	
	t_text_box.parameter_max = 2; 
	t_text_box.parameter_mix = 0;
	t_text_box.step_max = 9;
	t_text_box.step_mix = 1;

	gui_function.label_add(&gui.menu.home_form,&label_info);
	label_info.name = "info";
	label_info.fix_x = 36;
	label_info.fix_y = 4;
    label_info.offset_x = 15;
    label_info.offset_y = 1;
	label_info.inversion = false;
	label_info.flicker = false;
	label_info.font = 1; // 5x8字体
	label_info.type = UI_DATA_TYPE_STRING;
	label_info.p_data = (void *)(&gui.data.info);
	label_info.p_icon = RT_NULL;
	label_info.p_string = *option_info;

	gui_function.label_add(&gui.menu.home_form,&label_system_state);
	label_system_state.name = "state";
	label_system_state.fix_x = 36;
	label_system_state.fix_y = 6;
    label_system_state.offset_x = 15;
    label_system_state.offset_y = 1;
	label_system_state.inversion = false;
	label_system_state.flicker = false;
	label_system_state.font = 1; // 5x8字体
	label_system_state.type = UI_DATA_TYPE_STRING;
	label_system_state.p_data = (void *)(&gui.data.system_state);
	label_system_state.p_icon = RT_NULL;
	label_system_state.p_string = *option_state;
}


static void other_form_init(void)
{
	gui_function.form_init(&gui.menu.other_form);
	gui.menu.other_form.p_background_text = RT_NULL;//*other_page_background;
}

void gui_menu_init(void)
{
    #ifdef CUSTOMER_SERIAL_DEBUG
    rt_kprintf("\r\n...Enter_gui_menu_init");
    #endif

    logo_form_init();  		// logo 页面
	welcome_form_init(); 	// 欢迎页面
	home_form_init(); 		// 主页面
	other_form_init(); 		// 其他 (可做测试用)

    #ifdef CUSTOMER_SERIAL_DEBUG
    rt_kprintf("\r\n...Out_gui_menu_init");
    #endif

}




