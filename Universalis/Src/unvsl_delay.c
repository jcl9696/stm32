#include "unvsl_delay.h"
/********************************************************************************/
static u32 fac_us=0;							//us��ʱ������

/********************************************************************************/
//��ʼ���ӳٺ���
//SYSTICK��ʱ��ΪAHBʱ��
//SYSCLK:ϵͳʱ��Ƶ��
void UNVSL_Delay_Init(void)
{
	fac_us = HAL_RCC_GetSysClockFreq()/1000000;
}								    

//��ʱnus
//nusΪҪ��ʱ��us��.	
//nus:(���ֵ��2^32/fac_us)	 
void UNVSL_Delay_Us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks=nus*fac_us; 						//��Ҫ�Ľ����� 
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
}

//��ʱnms
//nms:Ҫ��ʱ��ms��
void UNVSL_Delay_Ms(u16 nms)
{
	u32 i;
	for(i=0;i<nms;i++) UNVSL_Delay_Us(1000);
}
/********************************************************************************/		
