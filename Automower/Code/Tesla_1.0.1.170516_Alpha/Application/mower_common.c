/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			mower_common.c
  Author:				Raymond
  Date:				2017.6.7
  Version:        
  Description:		存放一些公共的函数
  						
  						
  History:        // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改内容简述  
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include "stm32f4xx.h"
#include "rtthread.h"
#include "mower_common.h"
#include "menu.h"



struct rt_event sys_event;
struct rt_event exception_event;
rt_mq_t sys_messagequeue; // a mq pointor

#define SAVE_NUM_MAX 20


t_sys_total _sys_pattern;
T_u32_to_bit _sys_flag;

t_work_save _pattern_save[SAVE_NUM_MAX];
u8 pattern_save_num;

void work_pattern_init(void)
{
    /*前台*/
    _sys_pattern.front_mode.work_mode_curr = WORK_MODE_NULL;
    _sys_pattern.front_mode.work_mode_goal= WORK_MODE_NULL;
    _sys_pattern.front_mode.work_mode_request= WORK_MODE_NULL;
    _sys_pattern.front_mode.work_mode_last= WORK_MODE_NULL;
    _sys_pattern.front_mode.work_mode_save= WORK_MODE_NULL;
    
    /*后台*/
    _sys_pattern.background_mode.work_mode_curr = WORK_MODE_NULL;
    _sys_pattern.background_mode.work_mode_goal = WORK_MODE_NULL;
    _sys_pattern.background_mode.work_mode_last = WORK_MODE_NULL;
    _sys_pattern.background_mode.work_mode_request = WORK_MODE_NULL;
    _sys_pattern.background_mode.work_mode_save = WORK_MODE_NULL;
}

/****工作状态切换*/
void work_pattern_switch(void)
{
    //Assert(cond, msg);
    // front
    if(_sys_pattern.front_mode.work_mode_last != WORK_MODE_NULL)
    {
    	_sys_pattern.front_mode.work_mode_save = _sys_pattern.front_mode.work_mode_last;
		_sys_pattern.front_mode.work_mode_last = WORK_MODE_NULL;
    }
	if(_sys_pattern.front_mode.work_mode_goal != WORK_MODE_NULL)
	{
		_sys_pattern.front_mode.work_mode_last = _sys_pattern.front_mode.work_mode_curr;
		_sys_pattern.front_mode.work_mode_curr = _sys_pattern.front_mode.work_mode_goal;
	}
	_sys_pattern.front_mode.work_mode_goal = WORK_MODE_NULL;

	if(_sys_pattern.front_mode.work_mode_request != WORK_MODE_NULL)
	{
		_sys_pattern.front_mode.work_mode_goal= _sys_pattern.front_mode.work_mode_request;
		_sys_pattern.front_mode.work_mode_request = WORK_MODE_NULL;
	}

	// back
    if(_sys_pattern.background_mode.work_mode_last != WORK_MODE_NULL)
    {
    	_sys_pattern.background_mode.work_mode_save = _sys_pattern.background_mode.work_mode_last;
		_sys_pattern.background_mode.work_mode_last = WORK_MODE_NULL;
    }
	if(_sys_pattern.background_mode.work_mode_goal != WORK_MODE_NULL)
	{
		_sys_pattern.background_mode.work_mode_last = _sys_pattern.background_mode.work_mode_curr;
		_sys_pattern.background_mode.work_mode_curr = _sys_pattern.background_mode.work_mode_goal;
	}
	_sys_pattern.background_mode.work_mode_goal = WORK_MODE_NULL;

	if(_sys_pattern.background_mode.work_mode_request != WORK_MODE_NULL)
	{
		_sys_pattern.background_mode.work_mode_goal= _sys_pattern.background_mode.work_mode_request;
		_sys_pattern.background_mode.work_mode_request = WORK_MODE_NULL;
	}	
}

/****前台工作模式转换****/
void front_work_pattern_tran_req(e_work_pattern mode)
{
	if(mode == WORK_MODE_NULL)return;
	if(mode == _sys_pattern.front_mode.work_mode_curr)return;
	
	_sys_pattern.front_mode.work_mode_request= mode;
}

/****后台工作模式转换****/
void background_work_pattern_tran_req(e_work_pattern mode)
{
	if(mode == WORK_MODE_NULL)return;
	if(mode == _sys_pattern.background_mode.work_mode_curr)return;

	_sys_pattern.background_mode.work_mode_request = mode;
}

u8 work_pattern_enter_check(e_work_pattern mode)
{
	u8 flag;

	flag = 0;
	if(
		(_sys_pattern.front_mode.work_mode_goal == mode
		||_sys_pattern.background_mode.work_mode_goal == mode)
		&&
		(_sys_pattern.front_mode.work_mode_curr != mode
		&&_sys_pattern.background_mode.work_mode_curr != mode))
		
	flag = 1;

	return flag;
}

u8 work_pattern_leaves_check(e_work_pattern mode)
{
	u8 flag;

	flag = 0;

	if(
		(_sys_pattern.front_mode.work_mode_last == mode
		&&_sys_pattern.background_mode.work_mode_curr != mode
		&&_sys_pattern.background_mode.work_mode_goal != mode)
		||
		(_sys_pattern.background_mode.work_mode_last == mode
		&&_sys_pattern.front_mode.work_mode_curr != mode
		&&_sys_pattern.front_mode.work_mode_goal !=mode)
		||
		(_sys_pattern.background_mode.work_mode_last == mode
		&&_sys_pattern.front_mode.work_mode_last == mode)
		)
		flag = 1;
	
	return flag;
}



void work_pattern_save(e_work_pattern mode, u8 *enter_num)
{
	u8 i,pos;
	
	for(i = 0,pos = 0xff;i<(*enter_num)&&i<SAVE_NUM_MAX;i++)
	{
		if(_pattern_save[i].mode == mode)
		{
			pos = i;
			return ;
		}
	}

	if(pos != 0xff)
	{
		for(i=pos;i!=0;i--)
		{
			_pattern_save[i].mode = _pattern_save[i-1].mode;
		}

		_pattern_save[0].mode = mode;
	}
	else
	{
		if(*enter_num > SAVE_NUM_MAX) *enter_num = SAVE_NUM_MAX - 1;
		
		for(i=*enter_num;i!=0;i--)
		{
			_pattern_save[i].mode = _pattern_save[i-1].mode;
		}

		_pattern_save[0].mode = mode;

		(*enter_num)++;
	}
}



/****获取前一工作状态****/
u8 work_pattern_get_last(e_work_pattern *get_mode)
{
	t_work_save *savemode;
	//u8 savenum;
	e_work_pattern curmode;
	u8 i;
	u8 flag;
	
	savemode=&_pattern_save[0];
	//savenum = pattern_save_num;
	curmode=_sys_pattern.front_mode.work_mode_curr;

	flag = 0;
	while(flag == 0)
	{
		if(pattern_save_num == 0)
		{
			*get_mode = WORK_PATTERN_DEF;

			flag = 0xff;
				
			if(curmode == *get_mode)
			{
				return 0;
			}
		}
		else
		{
			*get_mode = savemode[0].mode;

			for(i=0;i<pattern_save_num-1;i++)
			{
				savemode[i].mode = savemode[i+1].mode;
			}

			pattern_save_num--;

			if(curmode == *get_mode)
			{
				;
			}
			else
			{
				return 0;
			}
		}
	}
		
	return 1;
}

/****获取当前工作状态*/
e_work_pattern get_m_req(void)
{
    return _sys_pattern.front_mode.work_mode_request;
}

e_work_pattern get_m_goal(void)
{
    return _sys_pattern.front_mode.work_mode_goal;
}

e_work_pattern get_m_curr(void)
{
    return _sys_pattern.front_mode.work_mode_curr;
}

e_work_pattern get_m_last(void)
{
    return _sys_pattern.front_mode.work_mode_last;
}

e_work_pattern get_m_save(void)
{
    return _sys_pattern.front_mode.work_mode_save;
}





void led_idle_indicate(void)
{
    static bool indicate_led;

    indicate_led ^= 1;
    PC_OUT(13) = indicate_led;
}

/*******************************
 * 设置向量表偏移地址
 * NVIC_VectTab:基址
 * Offset:偏移量
 ********************************/
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab,u32 Offset)	 
{ 	   	  
	SCB->VTOR=NVIC_VectTab|(Offset&(u32)0xFFFFFE00);//设置NVIC的向量表偏移寄存器,VTOR低9位保留,即[8:0]保留。
}


/***********************************
 * 设置NVIC分组
 * NVIC_Group:NVIC分组 0~4 总共5组 	
 ***********************************/
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
	u32 temp,temp1;	  
	temp1=(~NVIC_Group)&0x07;//取后三位
	temp1<<=8;
	temp=SCB->AIRCR;  //读取先前的设置
	temp&=0X0000F8FF; //清空先前分组
	temp|=0X05FA0000; //写入钥匙
	temp|=temp1;	   
	SCB->AIRCR=temp;  //设置分组	    	  				   
}


/**********************************************************************
 * 设置NVIC 
 * NVIC_PreemptionPriority:抢占优先级
 * NVIC_SubPriority       :响应优先级
 * NVIC_Channel           :中断编号
 * NVIC_Group             :中断分组 0~4
 * 注意优先级不能超过设定的组的范围!否则会有意想不到的错误
 * 组划分:
 * 组0:0位抢占优先级,4位响应优先级
 * 组1:1位抢占优先级,3位响应优先级
 * 组2:2位抢占优先级,2位响应优先级
 * 组3:3位抢占优先级,1位响应优先级
 * 组4:4位抢占优先级,0位响应优先级
 * NVIC_SubPriority和NVIC_PreemptionPriority的原则是,数值越小,越优先	  
 *********************************************************************/
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{ 
	u32 temp;	  
	MY_NVIC_PriorityGroupConfig(NVIC_Group);//设置分组
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;								//取低四位
	NVIC->ISER[NVIC_Channel/32]|=1<<NVIC_Channel%32;//使能中断位(要清除的话,设置ICER对应位为1即可)
	NVIC->IP[NVIC_Channel]|=temp<<4;				//设置响应优先级和抢断优先级   	    	  				   
} 


/*************************************************************************
 * 外部中断配置函数
 * 只针对GPIOA~I;不包括PVD,RTC,USB_OTG,USB_HS,以太网唤醒等
 * 参数:
 * GPIOx:0~8,代表GPIOA~I
 * BITx:需要使能的位;
 * TRIM:触发模式,1,下升沿;2,上降沿;3，任意电平触发
 * 该函数一次只能配置1个IO口,多个IO口,需多次调用
 * 该函数会自动开启对应中断,以及屏蔽线   
 *************************************************************************/
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
{ 
	u8 EXTOFFSET=(BITx%4)*4;  
	RCC->APB2ENR|=1<<14;  						//使能SYSCFG时钟  
	SYSCFG->EXTICR[BITx/4]&=~(0x000F<<EXTOFFSET);//清除原来设置！！！
	SYSCFG->EXTICR[BITx/4]|=GPIOx<<EXTOFFSET;	//EXTI.BITx映射到GPIOx.BITx 
	//自动设置
	EXTI->IMR|=1<<BITx;					//开启line BITx上的中断(如果要禁止中断，则反操作即可)
	if(TRIM&0x01)EXTI->FTSR|=1<<BITx;	//line BITx上事件下降沿触发
	if(TRIM&0x02)EXTI->RTSR|=1<<BITx;	//line BITx上事件上升降沿触发
}


/**********************************************************************************************************************************
 * GPIO复用设置
 * GPIOx:GPIOA~GPIOI.
 * BITx:0~15,代表IO引脚编号.
 * AFx:0~15,代表AF0~AF15.
 * AF0~15设置情况(这里仅是列出常用的,详细的请见429/746数据手册,Table 12):
 * AF0:MCO/SWD/SWCLK/RTC   		AF1:TIM1/TIM2;            		AF2:TIM3~5;               		AF3:TIM8~11
 * AF4:I2C1~I2C4;          		AF5:SPI1~SPI6;            		AF6:SPI3/SAI1;            	 	AF7:SPI2/3/USART1~3/UART5/SPDIFRX;
 * AF8:USART4~8/SPDIFRX/SAI2; 	AF9;CAN1~2/TIM12~14/LCD/QSPI; 	AF10:USB_OTG/USB_HS/SAI2/QSPI  	AF11:ETH
 * AF12:FMC/SDIO/OTG/HS   		AF13:DCIM                 		AF14:LCD;                  		AF15:EVENTOUT
 ***********************************************************************************************************************************/
void GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx)
{  
	GPIOx->AFR[BITx>>3]&=~(0X0F<<((BITx&0X07)*4));
	GPIOx->AFR[BITx>>3]|=(u32)AFx<<((BITx&0X07)*4);
} 


/**********************************************************************************************************************
 * GPIO通用设置 
 * GPIOx:GPIOA~GPIOI.
 * BITx:0X0000~0XFFFF,位设置,每个位代表一个IO,第0位代表Px0,第1位代表Px1,依次类推.比如0X0101,代表同时设置Px0和Px8.
 * MODE:0~3;模式选择,0,输入(系统复位默认状态);1,普通输出;2,复用功能;3,模拟输入.
 * OTYPE:0/1;输出类型选择,0,推挽输出;1,开漏输出.
 * OSPEED:0~3;输出速度设置,0,2Mhz;1,25Mhz;2,50Mhz;3,100Mh. 
 * PUPD:0~3:上下拉设置,0,不带上下拉;1,上拉;2,下拉;3,保留.
 * 注意:在输入模式(普通输入/模拟输入)下,OTYPE和OSPEED参数无效!!
 ***************************************************************************************************************************/
void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD)
{  
	u32 pinpos=0,pos=0,curpin=0;
	for(pinpos=0;pinpos<16;pinpos++)
	{
		pos=1<<pinpos;	//一个个位检查 
		curpin=BITx&pos;//检查引脚是否要设置
		if(curpin==pos)	//需要设置
		{
			GPIOx->MODER&=~(3<<(pinpos*2));	//先清除原来的设置
			GPIOx->MODER|=MODE<<(pinpos*2);	//设置新的模式 
			if((MODE==0X01)||(MODE==0X02))	//如果是输出模式/复用功能模式
			{  
				GPIOx->OSPEEDR&=~(3<<(pinpos*2));	//清除原来的设置
				GPIOx->OSPEEDR|=(OSPEED<<(pinpos*2));//设置新的速度值  
				GPIOx->OTYPER&=~(1<<pinpos) ;		//清除原来的设置
				GPIOx->OTYPER|=OTYPE<<pinpos;		//设置新的输出模式
			}  
			GPIOx->PUPDR&=~(3<<(pinpos*2));	//先清除原来的设置
			GPIOx->PUPDR|=PUPD<<(pinpos*2);	//设置新的上下拉
		}
	}
} 


/**********************************************
 * THUMB指令不支持汇编内联
 * 采用如下方法实现执行汇编指令WFI  
 ******************************************************/
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}


/*********************************************
 * 关闭所有中断(但是不包括fault和NMI中断)
 **********************************************/
void INTX_DISABLE(void)
{
	__ASM volatile("cpsid i");
}


/*************************
 * 开启所有中断
 *************************/
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}


/*******************************
 * 设置栈顶地址
 *　addr:栈顶地址
 *******************************/
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}


/****************************
 * 进入待机模式	  
 *****************************/
void Sys_Standby(void)
{ 
	SCB->SCR|=1<<2;		//使能SLEEPDEEP位 (SYS->CTRL)	   
	RCC->APB1ENR|=1<<28;//使能电源时钟 
	PWR->CSR|=1<<8;     //设置WKUP用于唤醒
	PWR->CR|=1<<2;      //清除Wake-up 标志
	PWR->CR|=1<<1;      //PDDS置位   	
	WFI_SET();			//执行WFI指令,进入待机模式		 
}	  



/********************************
 * 系统软复位   
 ********************************/
void Sys_Soft_Reset(void)
{   
	SCB->AIRCR =0X05FA0000|(u32)0x04;	  
} 



/*********************************************************************************
 * 时钟设置函数
 * Fvco=Fs*(plln/pllm);
 * Fsys=Fvco/pllp=Fs*(plln/(pllm*pllp));
 * Fusb=Fvco/pllq=Fs*(plln/(pllm*pllq));
 * Fvco:VCO频率
 * Fsys:系统时钟频率
 * Fusb:USB,SDIO,RNG等的时钟频率
 * Fs:PLL输入时钟频率,可以是HSI,HSE等. 
 * plln:主PLL倍频系数(PLL倍频),取值范围:64~432.
 * pllm:主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
 * pllp:系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
 * pllq:USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.

 * 外部晶振为25M的时候,推荐值:plln=360,pllm=25,pllp=2,pllq=8.
 * 得到:Fvco=25*(360/25)=360Mhz
 *     Fsys=360/2=180Mhz
 *     Fusb=360/8=45Mhz（使用USB时，需设置plln=384，即可得到48Mhz频率）
 * 返回值:0,成功;1,失败。
 ************************************************************************************/
u8 Sys_Clock_Set(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{ 
	u16 retry=0;
	u8 status=0;
	RCC->CR|=1<<16;				//HSE 开启 
	while(((RCC->CR&(1<<17))==0)&&(retry<0X1FFF))retry++;//等待HSE RDY
	if(retry==0X1FFF)status=1;	//HSE无法就绪
	else   
	{
		RCC->APB1ENR|=1<<28;	//电源接口时钟使能
		PWR->CR|=3<<14; 		//高性能模式,时钟可到180Mhz
		RCC->CFGR|=(0<<4)|(5<<10)|(4<<13);//HCLK 不分频;APB1 4分频;APB2 2分频. 
		RCC->CR&=~(1<<24);	//关闭主PLL
		RCC->PLLCFGR=pllm|(plln<<6)|(((pllp>>1)-1)<<16)|(pllq<<24)|(1<<22);//配置主PLL,PLL时钟源来自HSE
		RCC->CR|=1<<24;			//打开主PLL
		while((RCC->CR&(1<<25))==0);//等待PLL准备好 
		FLASH->ACR|=1<<8;		//指令预取使能.
		FLASH->ACR|=1<<9;		//指令cache使能.
		FLASH->ACR|=1<<10;		//数据cache使能.
		FLASH->ACR|=5<<0;		//5个CPU等待周期. 
		RCC->CFGR&=~(3<<0);		//清零
		RCC->CFGR|=2<<0;		//选择主PLL作为系统时钟	 
		while((RCC->CFGR&(3<<2))!=(2<<2));//等待主PLL作为系统时钟成功. 
	} 
	return status;
}  




/******************************************************************************
 * 系统时钟初始化函数
 * plln:主PLL倍频系数(PLL倍频),取值范围:64~432.
 * pllm:主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
 * pllp:系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
 * pllq:USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.
 ********************************************************************************/
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{  
	RCC->CR|=0x00000001;		//设置HISON,开启内部高速RC振荡
	RCC->CFGR=0x00000000;		//CFGR清零 
	RCC->CR&=0xFEF6FFFF;		//HSEON,CSSON,PLLON清零 
	RCC->PLLCFGR=0x24003010;	//PLLCFGR恢复复位值 
	RCC->CR&=~(1<<18);			//HSEBYP清零,外部晶振不旁路
	RCC->CIR=0x00000000;		//禁止RCC时钟中断 
	Sys_Clock_Set(plln,pllm,pllp,pllq);//设置时钟 
	//配置向量表				  
#ifdef  VECT_TAB_RAM
	MY_NVIC_SetVectorTable(1<<29,0x0);
#else   
	MY_NVIC_SetVectorTable(0,0x0);
#endif 
}		    



/****************************************************
 ** 在配置MCU时调用此延时函数
 ** Raymond 2017.7.19
 ****************************************************
 **/
void sys_power_control_delaly(u16 cnt)
{
    u16 temp;

    for(;cnt > 0;cnt--)
    {
        for(temp = 0;temp < 3000;temp++)
        {
            __ASM("nop");
        }
    }
}

/****************************************************
 ** 低功耗上电/休眠唤醒时序
 ** Raymond 2017.7.19
 ****************************************************
 */
void sys_power_control(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
#ifdef SUPPORT_BOARD_VERSION_2
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
    
    GPIO_WriteBit(GPIOC,GPIO_Pin_4,1);
    sys_power_control_delaly(230); // more than 230ms delay

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOF,&GPIO_InitStructure);

    GPIO_WriteBit(GPIOF,GPIO_Pin_10,1);
    sys_power_control_delaly(20);// more than 20ms delay
#else
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOG,&GPIO_InitStructure);
    
    GPIO_WriteBit(GPIOG,GPIO_Pin_15,1);
    sys_power_control_delaly(230); // more than 230ms delay

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI,ENABLE);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOI,&GPIO_InitStructure);

    GPIO_WriteBit(GPIOI,GPIO_Pin_6,1);
    sys_power_control_delaly(20);// more than 20ms delay
#endif    
}





















