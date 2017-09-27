说明：
	本文件是TOPBAND微电事业部技术中心割草机器人项目说明文件，对本项目使用的编辑，编译环境，软硬件平台以及工程里各个文件夹做简要说明。
	如工程架构有更改，及时更新

	2017.7.7 Raymond


工程说明：
1、工程编译环境为:uVision V5.20.0.0 ;  Toolchain:MDK-ARM Professional Version 5.20.
2、工程编辑环境:source insight Version 3.50.0056.
3、硬件平台基于意法半导体STM32f429IGT6芯片搭载著名国产开源RTOS： RT-thread操作系统.
4、主频188MHZ,系统滴答1ms. 

使用说明：
1、工程包含以下文件夹：
	algorithm	----算法代码文件夹
	application	----应用层代码文件夹，主要要存放线程相关文件
	communication	----通讯层代码文件
	dmp		----dmp算法相关，暂时没用到
	driver		----外设驱动代码
	mdk_prj		----MDK工程文件夹
	OS		----rt-thread系统内核代码
	starUp		----CMSIS及板级启动文件
	stmlib		----stm32标准库文件
	工程说明	----顾名思义

2、打开mdk_prj文件夹，找到Tesla_1.0.0.170516_Alpha.uvprojx文件双击就可以在MDK上打开工程
3、从starUp文件夹里的startup.c文件可以找到main（）函数，顺延阅读代码。
4、另外，可以使用其他编辑器来开发软件（建议使用功能强大的Version 3.50.0056版本的suorce insight）前提是以上列出文件夹不可删减。 



更改记录:
1、2017.7.31
	工程文件架构有更改更改如下
	algorithm	----算法代码文件夹
	application	----应用层代码文件夹，主要要存放线程相关文件
	communication	----通讯层代码文件
	dmp		----dmp算法相关，暂时没用到
	driver		----外设驱动代码
	mdk_prj		----MDK工程文件夹
	OS		----rt-thread系统内核代码
	src		----CMSIS及板级启动文件
	stmlib		----stm32标准库文件
	include		----系统全局头文件 
	工程说明	----顾名思义


2、2017.8.8
	工程文件架构有更改更改如下
	algorithm	----算法代码文件夹
	application	----应用层代码文件夹，主要要存放线程相关文件
	communication	----通讯层代码文件
	dmp		----dmp算法相关，暂时没用到
	driver		----外设驱动代码
	include		----系统全局文件目录
	mdk_prj		----MDK工程文件夹
	OS		----rt-thread系统内核代码
	scons_prj	----scons软件管理脚本目录
	src		----CMSIS及板级启动文件
	stmlib		----stm32标准库文件
	tools		----scons软件管理脚本工具目录
	工程说明	----顾名思义
	
	





	  
	