#include "unvsl_uasrt.h"
#include "stdio.h"
/********************************************************************************/
UNVSL_Usart_Contrl_Def unvsal_usart_ctrl = {0};

/********************************************************************************/
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	u8 temp[1]={ch};
	HAL_UART_Transmit(unvsal_usart_ctrl.phuart,temp,1,2);      
	return ch;
}
/********************************************************************************/	
void UNVSL_Usart_Init(UART_HandleTypeDef *phuart, UNVSL_Usart_Contrl_Def *pu)
{
	pu->phuart = phuart;
#if EN_UNVSL_USART_RX
	pu->rx_sta = 0;
#endif
}
/********************************************************************************/	
#if EN_UNVSL_USART_RX

#define RXBUFFERSIZE   1 //缓存大小	
static u8 aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲

void UNVSL_Usart_DealIRQ(UART_HandleTypeDef *phuart)                	
{ 
	u32 timeout=0;
	u32 maxDelay=0x1FFFF;
     
	timeout=0;
  while (HAL_UART_GetState(phuart) != HAL_UART_STATE_READY)//等待就绪
	{
	 timeout++;////超时处理
     if(timeout>maxDelay) break;		
	}
	timeout=0;
	while(HAL_UART_Receive_IT(phuart, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK){//一次处理完成之后，重新开启中断并设置RxXferCount为1
		if(++timeout>maxDelay) break;
	}
} 

//串口中断处理
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *phuart)
{
	if(phuart->Instance==USART2)//如果是串口2
	{
		if((unvsal_usart_ctrl.rx_sta&0x8000)==0)//接收未完成
		{
			if(unvsal_usart_ctrl.rx_sta&0x4000)//接收到了0x0d
			{
				if(aRxBuffer[0]!=0x0a)unvsal_usart_ctrl.rx_sta=0;//接收错误,重新开始
				else unvsal_usart_ctrl.rx_sta|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(aRxBuffer[0]==0x0d)unvsal_usart_ctrl.rx_sta|=0x4000;
				else
				{
					unvsal_usart_ctrl.rx_buf[unvsal_usart_ctrl.rx_sta&0X3FFF]=aRxBuffer[0];
					unvsal_usart_ctrl.rx_sta++;
					if(unvsal_usart_ctrl.rx_sta>(UNVSL_USART_REC_LEN-1))unvsal_usart_ctrl.rx_sta=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}

	}
}

#endif
/********************************************************************************/	

