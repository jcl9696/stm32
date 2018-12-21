#include "unvsl_delay.h"
/********************************************************************************/
static u32 fac_us=0;							//us延时倍乘数

/********************************************************************************/
//初始化延迟函数
//SYSTICK的时钟为AHB时钟
//SYSCLK:系统时钟频率
void UNVSL_Delay_Init(void)
{
	fac_us = HAL_RCC_GetSysClockFreq()/1000000;
}								    

//延时nus
//nus为要延时的us数.	
//nus:(最大值即2^32/fac_us)	 
void UNVSL_Delay_Us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks=nus*fac_us; 						//需要的节拍数 
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};
}

//延时nms
//nms:要延时的ms数
void UNVSL_Delay_Ms(u16 nms)
{
	u32 i;
	for(i=0;i<nms;i++) UNVSL_Delay_Us(1000);
}
/********************************************************************************/		
