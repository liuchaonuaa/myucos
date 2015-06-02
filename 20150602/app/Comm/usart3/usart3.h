#ifndef __USART3_H
#define __USART3_H

#include "stdio.h"	
#include "sys.h" 
#include "communication3.h" 


//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����1��ʼ��		   
////////////////////////////////////////////////////////////////////////////////// 	
extern  u8  RcvChar;	

extern  UART3_RX uart3_Rx;
extern  UART3_TX uart3_Tx;

void uart3_init(u32 bound);
void uart3_send(u8 *buf, u8 len);
void Uart3RcvData(u8 dat);
void Rcv3BufInit(void);
void uart3_putString(uint8_t *string);

#endif


