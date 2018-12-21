#ifndef __UNVSL_USART_H__
#define __UNVSL_USART_H__

#include "universalis.h"

#define EN_UNVSL_USART_RX 	      (1)			//使能（1）/禁止（0）串口接收

#define UNVSL_USART_REC_LEN  			(64)  	//定义最大接收字节数

typedef struct{
#if EN_UNVSL_USART_RX
	//接收状态
	//bit15，	接收完成标志
	//bit14，	接收到0x0d
	//bit13~0，	接收到的有效字节数目
	u16 rx_sta;       //接收状态标记	
	u8 rx_buf[UNVSL_USART_REC_LEN];
#endif
	UART_HandleTypeDef *phuart;
}UNVSL_Usart_Contrl_Def;

extern UNVSL_Usart_Contrl_Def unvsal_usart_ctrl;
/********************************************************************************/
void UNVSL_Usart_Init(UART_HandleTypeDef *phuart, UNVSL_Usart_Contrl_Def *pu);
void UNVSL_Usart_DealIRQ(UART_HandleTypeDef *phuart);
/********************************************************************************/
#endif /*__UNVSL_USART_H__*/
