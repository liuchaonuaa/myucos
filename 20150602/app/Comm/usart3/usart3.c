#include "sys.h"
#include "usart3.h"
#include "ucos_ii.h"
#include "os_cpu.h"
#include "communication3.h"

UART3_RX uart3_Rx;
UART3_TX uart3_Tx;

extern OS_EVENT	*Comm3Sem;

 
/*
 ***************************************************************
 * ����: uart3_init 
 * ����: ����3��ʼ��
 * ����: 1. bound��������
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */  
void uart3_init(u32 bound)
{
	// GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// ʹ��USART3��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	USART_DeInit(USART3);  //��λ����3

	// USART3_TX   PB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		// �����������
	GPIO_Init(GPIOB, &GPIO_InitStructure); 				// ��ʼ��PB10
	
	// USART3_RX	  PB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	// ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);  		// ��ʼ��PB11
	
	// Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	// ��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			// �����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				// IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	// USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;					// һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	// �ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;			// ����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// ��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	// �շ�ģʽ
	
	USART_Init(USART3, &USART_InitStructure);		// ��ʼ������
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	// �����ж�
	USART_Cmd(USART3, ENABLE);                    	// ʹ�ܴ��� 
}


/*
 ***************************************************************
 * ����: USART3_IRQHandler 
 * ����: ����3�жϴ�����
 * ����: NULL
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */  
void USART3_IRQHandler(void)  // ����1�жϷ������
{	
	u8 ch;
	OS_CPU_SR cpu_sr = 0;

	OS_ENTER_CRITICAL();			// �����ٽ���(�޷����жϴ��)    
	
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		ch = (USART_ReceiveData(USART3) & 0xff); 
				
		/* Clear the USART1 Receive interrupt */
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		
		Uart3RcvData(ch);
	}		

	OS_EXIT_CRITICAL();				// �˳��ٽ���(���Ա��жϴ��)
}


/*
 ***************************************************************
 * ����: uart3_putchar 
 * ����: ����3����ַ�
 * ����: 1. ch ������ַ�
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */  
int uart3_putchar(u8 ch)
{      
	while ((USART3->SR&0X40) == 0);//ѭ������,ֱ���������   
	
	USART3->DR = (u8) ch;      

	return ch;
}


/*
 ***************************************************************
 * ����: uart3_send 
 * ����: ����3��������
 * ����: 1. *buf��Ҫ���͵�����
 *       2. len: Ҫ���͵����ݳ���
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */  
void uart3_send(u8 *buf, u8 len)
{
    u8 i;
        
    for (i = 0; i < len; i++)
    {
        uart3_putchar(*buf++);
    }
}


/*
 ***************************************************************
 * ����: uart3_putString 
 * ����: ����3�����ַ�
 * ����: 1. *string��Ҫ���͵��ַ�
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */  
void uart3_putString(uint8_t *string)
{
	while (*string != '\0')
	{
		uart3_putchar(*string);
		string++;
	}
}


/*
 ***************************************************************
 * ����: Rcv3BufClearAll 
 * ����: �����
 * ����: 1. *prmt������
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */  
void Rcv3BufClearAll(UART3_RX *prmt)
{
    memset(prmt->buf, 0, UART3_RX_BUFLEN);
	prmt->count = 0;
	prmt->len   = 0;
}


/*
 ***************************************************************
 * ����: RcvBufInit 
 * ����: ��ʼ��
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ��
 ***************************************************************
 */
void Rcv3BufInit(void)
{
    Rcv3BufClearAll(&uart3_Rx);
}


/*
 ***************************************************************
 * ����: Uart3RcvData 
 * ����: ����3��������
 * ����: 1. dat�����ڽ��յ�������
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */ 
void Uart3RcvData(u8 dat)
{
    uart3_Rx.buf[uart3_Rx.count] = dat;	
    
	if (++(uart3_Rx.count) >= UART3_RX_BUFLEN)		
    {
        uart3_Rx.count = 0;
    }
	
	switch (uart3_Rx.count)
	{
		case 1:
			if (uart3_Rx.buf[0] != 0x5a)
			{
				uart3_Rx.count = 0;
				uart3_Rx.len = 0;	
			}
			break;

		case 2:
			switch (uart3_Rx.buf[1])
			{		                       
				case 0xa0:
					uart3_Rx.len = 8;
					break;	
			
				case 0xa1:
					uart3_Rx.len = 1024+10;
					break;
				
				case 0xb0:
					uart3_Rx.len = 8;
					break;	
			
				case 0xb4:
					uart3_Rx.len = 1024+10;
					break;
				
				case 0xc0:
					uart3_Rx.len = 1024+10;
					break;				
					
				default:
					uart3_Rx.len = 0;
					break;					
			}
			break;
			
		default:
			if ((uart3_Rx.count == uart3_Rx.len) && uart3_Rx.len)
			{
				OSSemPost(Comm3Sem);
				uart3_Rx.count = 0;
			}
			else if (uart3_Rx.len == 0)
			{
				uart3_Rx.count = 0;
			}
			break;
	}
	
	if ((uart3_Rx.count > uart3_Rx.len) && uart3_Rx.len)
	{
		uart3_Rx.count = 0;
		uart3_Rx.len = 0;
	}
		
}

