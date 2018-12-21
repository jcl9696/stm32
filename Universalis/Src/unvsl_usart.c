#include "unvsl_uasrt.h"
#include "stdio.h"
/********************************************************************************/
UNVSL_Usart_Contrl_Def unvsal_usart_ctrl = {0};

/********************************************************************************/
//�ض���fputc���� 
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

#define RXBUFFERSIZE   1 //�����С	
static u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���

void UNVSL_Usart_DealIRQ(UART_HandleTypeDef *phuart)                	
{ 
	u32 timeout=0;
	u32 maxDelay=0x1FFFF;
     
	timeout=0;
  while (HAL_UART_GetState(phuart) != HAL_UART_STATE_READY)//�ȴ�����
	{
	 timeout++;////��ʱ����
     if(timeout>maxDelay) break;		
	}
	timeout=0;
	while(HAL_UART_Receive_IT(phuart, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK){//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
		if(++timeout>maxDelay) break;
	}
} 

//�����жϴ���
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *phuart)
{
	if(phuart->Instance==USART2)//����Ǵ���2
	{
		if((unvsal_usart_ctrl.rx_sta&0x8000)==0)//����δ���
		{
			if(unvsal_usart_ctrl.rx_sta&0x4000)//���յ���0x0d
			{
				if(aRxBuffer[0]!=0x0a)unvsal_usart_ctrl.rx_sta=0;//���մ���,���¿�ʼ
				else unvsal_usart_ctrl.rx_sta|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(aRxBuffer[0]==0x0d)unvsal_usart_ctrl.rx_sta|=0x4000;
				else
				{
					unvsal_usart_ctrl.rx_buf[unvsal_usart_ctrl.rx_sta&0X3FFF]=aRxBuffer[0];
					unvsal_usart_ctrl.rx_sta++;
					if(unvsal_usart_ctrl.rx_sta>(UNVSL_USART_REC_LEN-1))unvsal_usart_ctrl.rx_sta=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}

	}
}

#endif
/********************************************************************************/	

