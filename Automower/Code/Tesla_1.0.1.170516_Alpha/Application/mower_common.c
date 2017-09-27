/*************************************************
  Copyright (C), 2017-2027, TOPBAND Co., Ltd.
  File name:			mower_common.c
  Author:				Raymond
  Date:				2017.6.7
  Version:        
  Description:		���һЩ�����ĺ���
  						
  						
  History:        // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                  // �߼��޸����ݼ���  
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
    /*ǰ̨*/
    _sys_pattern.front_mode.work_mode_curr = WORK_MODE_NULL;
    _sys_pattern.front_mode.work_mode_goal= WORK_MODE_NULL;
    _sys_pattern.front_mode.work_mode_request= WORK_MODE_NULL;
    _sys_pattern.front_mode.work_mode_last= WORK_MODE_NULL;
    _sys_pattern.front_mode.work_mode_save= WORK_MODE_NULL;
    
    /*��̨*/
    _sys_pattern.background_mode.work_mode_curr = WORK_MODE_NULL;
    _sys_pattern.background_mode.work_mode_goal = WORK_MODE_NULL;
    _sys_pattern.background_mode.work_mode_last = WORK_MODE_NULL;
    _sys_pattern.background_mode.work_mode_request = WORK_MODE_NULL;
    _sys_pattern.background_mode.work_mode_save = WORK_MODE_NULL;
}

/****����״̬�л�*/
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

/****ǰ̨����ģʽת��****/
void front_work_pattern_tran_req(e_work_pattern mode)
{
	if(mode == WORK_MODE_NULL)return;
	if(mode == _sys_pattern.front_mode.work_mode_curr)return;
	
	_sys_pattern.front_mode.work_mode_request= mode;
}

/****��̨����ģʽת��****/
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



/****��ȡǰһ����״̬****/
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

/****��ȡ��ǰ����״̬*/
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
 * ����������ƫ�Ƶ�ַ
 * NVIC_VectTab:��ַ
 * Offset:ƫ����
 ********************************/
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab,u32 Offset)	 
{ 	   	  
	SCB->VTOR=NVIC_VectTab|(Offset&(u32)0xFFFFFE00);//����NVIC��������ƫ�ƼĴ���,VTOR��9λ����,��[8:0]������
}


/***********************************
 * ����NVIC����
 * NVIC_Group:NVIC���� 0~4 �ܹ�5�� 	
 ***********************************/
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
	u32 temp,temp1;	  
	temp1=(~NVIC_Group)&0x07;//ȡ����λ
	temp1<<=8;
	temp=SCB->AIRCR;  //��ȡ��ǰ������
	temp&=0X0000F8FF; //�����ǰ����
	temp|=0X05FA0000; //д��Կ��
	temp|=temp1;	   
	SCB->AIRCR=temp;  //���÷���	    	  				   
}


/**********************************************************************
 * ����NVIC 
 * NVIC_PreemptionPriority:��ռ���ȼ�
 * NVIC_SubPriority       :��Ӧ���ȼ�
 * NVIC_Channel           :�жϱ��
 * NVIC_Group             :�жϷ��� 0~4
 * ע�����ȼ����ܳ����趨����ķ�Χ!����������벻���Ĵ���
 * �黮��:
 * ��0:0λ��ռ���ȼ�,4λ��Ӧ���ȼ�
 * ��1:1λ��ռ���ȼ�,3λ��Ӧ���ȼ�
 * ��2:2λ��ռ���ȼ�,2λ��Ӧ���ȼ�
 * ��3:3λ��ռ���ȼ�,1λ��Ӧ���ȼ�
 * ��4:4λ��ռ���ȼ�,0λ��Ӧ���ȼ�
 * NVIC_SubPriority��NVIC_PreemptionPriority��ԭ����,��ֵԽС,Խ����	  
 *********************************************************************/
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{ 
	u32 temp;	  
	MY_NVIC_PriorityGroupConfig(NVIC_Group);//���÷���
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;								//ȡ����λ
	NVIC->ISER[NVIC_Channel/32]|=1<<NVIC_Channel%32;//ʹ���ж�λ(Ҫ����Ļ�,����ICER��ӦλΪ1����)
	NVIC->IP[NVIC_Channel]|=temp<<4;				//������Ӧ���ȼ����������ȼ�   	    	  				   
} 


/*************************************************************************
 * �ⲿ�ж����ú���
 * ֻ���GPIOA~I;������PVD,RTC,USB_OTG,USB_HS,��̫�����ѵ�
 * ����:
 * GPIOx:0~8,����GPIOA~I
 * BITx:��Ҫʹ�ܵ�λ;
 * TRIM:����ģʽ,1,������;2,�Ͻ���;3�������ƽ����
 * �ú���һ��ֻ������1��IO��,���IO��,���ε���
 * �ú������Զ�������Ӧ�ж�,�Լ�������   
 *************************************************************************/
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
{ 
	u8 EXTOFFSET=(BITx%4)*4;  
	RCC->APB2ENR|=1<<14;  						//ʹ��SYSCFGʱ��  
	SYSCFG->EXTICR[BITx/4]&=~(0x000F<<EXTOFFSET);//���ԭ�����ã�����
	SYSCFG->EXTICR[BITx/4]|=GPIOx<<EXTOFFSET;	//EXTI.BITxӳ�䵽GPIOx.BITx 
	//�Զ�����
	EXTI->IMR|=1<<BITx;					//����line BITx�ϵ��ж�(���Ҫ��ֹ�жϣ��򷴲�������)
	if(TRIM&0x01)EXTI->FTSR|=1<<BITx;	//line BITx���¼��½��ش���
	if(TRIM&0x02)EXTI->RTSR|=1<<BITx;	//line BITx���¼��������ش���
}


/**********************************************************************************************************************************
 * GPIO��������
 * GPIOx:GPIOA~GPIOI.
 * BITx:0~15,����IO���ű��.
 * AFx:0~15,����AF0~AF15.
 * AF0~15�������(��������г����õ�,��ϸ�����429/746�����ֲ�,Table 12):
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
 * GPIOͨ������ 
 * GPIOx:GPIOA~GPIOI.
 * BITx:0X0000~0XFFFF,λ����,ÿ��λ����һ��IO,��0λ����Px0,��1λ����Px1,��������.����0X0101,����ͬʱ����Px0��Px8.
 * MODE:0~3;ģʽѡ��,0,����(ϵͳ��λĬ��״̬);1,��ͨ���;2,���ù���;3,ģ������.
 * OTYPE:0/1;�������ѡ��,0,�������;1,��©���.
 * OSPEED:0~3;����ٶ�����,0,2Mhz;1,25Mhz;2,50Mhz;3,100Mh. 
 * PUPD:0~3:����������,0,����������;1,����;2,����;3,����.
 * ע��:������ģʽ(��ͨ����/ģ������)��,OTYPE��OSPEED������Ч!!
 ***************************************************************************************************************************/
void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD)
{  
	u32 pinpos=0,pos=0,curpin=0;
	for(pinpos=0;pinpos<16;pinpos++)
	{
		pos=1<<pinpos;	//һ����λ��� 
		curpin=BITx&pos;//��������Ƿ�Ҫ����
		if(curpin==pos)	//��Ҫ����
		{
			GPIOx->MODER&=~(3<<(pinpos*2));	//�����ԭ��������
			GPIOx->MODER|=MODE<<(pinpos*2);	//�����µ�ģʽ 
			if((MODE==0X01)||(MODE==0X02))	//��������ģʽ/���ù���ģʽ
			{  
				GPIOx->OSPEEDR&=~(3<<(pinpos*2));	//���ԭ��������
				GPIOx->OSPEEDR|=(OSPEED<<(pinpos*2));//�����µ��ٶ�ֵ  
				GPIOx->OTYPER&=~(1<<pinpos) ;		//���ԭ��������
				GPIOx->OTYPER|=OTYPE<<pinpos;		//�����µ����ģʽ
			}  
			GPIOx->PUPDR&=~(3<<(pinpos*2));	//�����ԭ��������
			GPIOx->PUPDR|=PUPD<<(pinpos*2);	//�����µ�������
		}
	}
} 


/**********************************************
 * THUMBָ�֧�ֻ������
 * �������·���ʵ��ִ�л��ָ��WFI  
 ******************************************************/
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}


/*********************************************
 * �ر������ж�(���ǲ�����fault��NMI�ж�)
 **********************************************/
void INTX_DISABLE(void)
{
	__ASM volatile("cpsid i");
}


/*************************
 * ���������ж�
 *************************/
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}


/*******************************
 * ����ջ����ַ
 *��addr:ջ����ַ
 *******************************/
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}


/****************************
 * �������ģʽ	  
 *****************************/
void Sys_Standby(void)
{ 
	SCB->SCR|=1<<2;		//ʹ��SLEEPDEEPλ (SYS->CTRL)	   
	RCC->APB1ENR|=1<<28;//ʹ�ܵ�Դʱ�� 
	PWR->CSR|=1<<8;     //����WKUP���ڻ���
	PWR->CR|=1<<2;      //���Wake-up ��־
	PWR->CR|=1<<1;      //PDDS��λ   	
	WFI_SET();			//ִ��WFIָ��,�������ģʽ		 
}	  



/********************************
 * ϵͳ��λ   
 ********************************/
void Sys_Soft_Reset(void)
{   
	SCB->AIRCR =0X05FA0000|(u32)0x04;	  
} 



/*********************************************************************************
 * ʱ�����ú���
 * Fvco=Fs*(plln/pllm);
 * Fsys=Fvco/pllp=Fs*(plln/(pllm*pllp));
 * Fusb=Fvco/pllq=Fs*(plln/(pllm*pllq));
 * Fvco:VCOƵ��
 * Fsys:ϵͳʱ��Ƶ��
 * Fusb:USB,SDIO,RNG�ȵ�ʱ��Ƶ��
 * Fs:PLL����ʱ��Ƶ��,������HSI,HSE��. 
 * plln:��PLL��Ƶϵ��(PLL��Ƶ),ȡֵ��Χ:64~432.
 * pllm:��PLL����ƵPLL��Ƶϵ��(PLL֮ǰ�ķ�Ƶ),ȡֵ��Χ:2~63.
 * pllp:ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2,4,6,8.(������4��ֵ!)
 * pllq:USB/SDIO/������������ȵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2~15.

 * �ⲿ����Ϊ25M��ʱ��,�Ƽ�ֵ:plln=360,pllm=25,pllp=2,pllq=8.
 * �õ�:Fvco=25*(360/25)=360Mhz
 *     Fsys=360/2=180Mhz
 *     Fusb=360/8=45Mhz��ʹ��USBʱ��������plln=384�����ɵõ�48MhzƵ�ʣ�
 * ����ֵ:0,�ɹ�;1,ʧ�ܡ�
 ************************************************************************************/
u8 Sys_Clock_Set(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{ 
	u16 retry=0;
	u8 status=0;
	RCC->CR|=1<<16;				//HSE ���� 
	while(((RCC->CR&(1<<17))==0)&&(retry<0X1FFF))retry++;//�ȴ�HSE RDY
	if(retry==0X1FFF)status=1;	//HSE�޷�����
	else   
	{
		RCC->APB1ENR|=1<<28;	//��Դ�ӿ�ʱ��ʹ��
		PWR->CR|=3<<14; 		//������ģʽ,ʱ�ӿɵ�180Mhz
		RCC->CFGR|=(0<<4)|(5<<10)|(4<<13);//HCLK ����Ƶ;APB1 4��Ƶ;APB2 2��Ƶ. 
		RCC->CR&=~(1<<24);	//�ر���PLL
		RCC->PLLCFGR=pllm|(plln<<6)|(((pllp>>1)-1)<<16)|(pllq<<24)|(1<<22);//������PLL,PLLʱ��Դ����HSE
		RCC->CR|=1<<24;			//����PLL
		while((RCC->CR&(1<<25))==0);//�ȴ�PLL׼���� 
		FLASH->ACR|=1<<8;		//ָ��Ԥȡʹ��.
		FLASH->ACR|=1<<9;		//ָ��cacheʹ��.
		FLASH->ACR|=1<<10;		//����cacheʹ��.
		FLASH->ACR|=5<<0;		//5��CPU�ȴ�����. 
		RCC->CFGR&=~(3<<0);		//����
		RCC->CFGR|=2<<0;		//ѡ����PLL��Ϊϵͳʱ��	 
		while((RCC->CFGR&(3<<2))!=(2<<2));//�ȴ���PLL��Ϊϵͳʱ�ӳɹ�. 
	} 
	return status;
}  




/******************************************************************************
 * ϵͳʱ�ӳ�ʼ������
 * plln:��PLL��Ƶϵ��(PLL��Ƶ),ȡֵ��Χ:64~432.
 * pllm:��PLL����ƵPLL��Ƶϵ��(PLL֮ǰ�ķ�Ƶ),ȡֵ��Χ:2~63.
 * pllp:ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2,4,6,8.(������4��ֵ!)
 * pllq:USB/SDIO/������������ȵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2~15.
 ********************************************************************************/
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{  
	RCC->CR|=0x00000001;		//����HISON,�����ڲ�����RC��
	RCC->CFGR=0x00000000;		//CFGR���� 
	RCC->CR&=0xFEF6FFFF;		//HSEON,CSSON,PLLON���� 
	RCC->PLLCFGR=0x24003010;	//PLLCFGR�ָ���λֵ 
	RCC->CR&=~(1<<18);			//HSEBYP����,�ⲿ������·
	RCC->CIR=0x00000000;		//��ֹRCCʱ���ж� 
	Sys_Clock_Set(plln,pllm,pllp,pllq);//����ʱ�� 
	//����������				  
#ifdef  VECT_TAB_RAM
	MY_NVIC_SetVectorTable(1<<29,0x0);
#else   
	MY_NVIC_SetVectorTable(0,0x0);
#endif 
}		    



/****************************************************
 ** ������MCUʱ���ô���ʱ����
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
 ** �͹����ϵ�/���߻���ʱ��
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





















