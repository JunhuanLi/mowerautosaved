/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			userconfig.h
  Author:				Raymond
  Date:				2017.6.7
  Version:        
  Description:		系统的组件控制宏定义
  						调试功能宏定义
  						在userdef.h中被调用
  						
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述  
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/



#ifndef __SYS_CONF_H__
#define __SYS_CONF_H__



#define SUPPORT_BOARD_VERSION_2 // 第二版硬件对应的软件软件

/************************** 调试控制的相关宏 ***********************/
#define CUSTOMER_SERIAL_DEBUG   // 调试打印宏



/*************************** 产品平台相关宏定义 **********************/
//#define SUPPORT_LCD12864_ST7920		// 支持ST7920驱动的12864屏
#define SUPPORT_LCD12864_ST7565R	// 支持绘晶科技的ST7565R驱动的12864屏
#define SUPPORT_TFT43			// 支持tft4.3寸屏









#endif
/*************end*******************/

