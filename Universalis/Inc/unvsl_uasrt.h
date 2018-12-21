#ifndef __UNVSL_USART_H__
#define __UNVSL_USART_H__

#include "universalis.h"

#define EN_UNVSL_USART_RX 	      (1)			//ʹ�ܣ�1��/��ֹ��0�����ڽ���

#define UNVSL_USART_REC_LEN  			(64)  	//�����������ֽ���

typedef struct{
#if EN_UNVSL_USART_RX
	//����״̬
	//bit15��	������ɱ�־
	//bit14��	���յ�0x0d
	//bit13~0��	���յ�����Ч�ֽ���Ŀ
	u16 rx_sta;       //����״̬���	
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
