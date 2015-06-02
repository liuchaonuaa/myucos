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
 * 名称: uart3_init 
 * 功能: 串口3初始化
 * 输入: 1. bound：波特率
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */  
void uart3_init(u32 bound)
{
	// GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// 使能USART3，GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	USART_DeInit(USART3);  //复位串口3

	// USART3_TX   PB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		// 复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure); 				// 初始化PB10
	
	// USART3_RX	  PB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	// 浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);  		// 初始化PB11
	
	// Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	// 抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			// 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				// IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	// USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;					// 一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	// 字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// 一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;			// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	// 收发模式
	
	USART_Init(USART3, &USART_InitStructure);		// 初始化串口
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	// 开启中断
	USART_Cmd(USART3, ENABLE);                    	// 使能串口 
}


/*
 ***************************************************************
 * 名称: USART3_IRQHandler 
 * 功能: 串口3中断处理函数
 * 输入: NULL
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */  
void USART3_IRQHandler(void)  // 串口1中断服务程序
{	
	u8 ch;
	OS_CPU_SR cpu_sr = 0;

	OS_ENTER_CRITICAL();			// 进入临界区(无法被中断打断)    
	
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		ch = (USART_ReceiveData(USART3) & 0xff); 
				
		/* Clear the USART1 Receive interrupt */
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		
		Uart3RcvData(ch);
	}		

	OS_EXIT_CRITICAL();				// 退出临界区(可以被中断打断)
}


/*
 ***************************************************************
 * 名称: uart3_putchar 
 * 功能: 串口3输出字符
 * 输入: 1. ch 输出的字符
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */  
int uart3_putchar(u8 ch)
{      
	while ((USART3->SR&0X40) == 0);//循环发送,直到发送完毕   
	
	USART3->DR = (u8) ch;      

	return ch;
}


/*
 ***************************************************************
 * 名称: uart3_send 
 * 功能: 串口3发送数据
 * 输入: 1. *buf：要发送的数据
 *       2. len: 要发送的数据长度
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
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
 * 名称: uart3_putString 
 * 功能: 串口3发送字符
 * 输入: 1. *string：要发送的字符
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
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
 * 名称: Rcv3BufClearAll 
 * 功能: 清零参
 * 输入: 1. *prmt：参数
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
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
 * 名称: RcvBufInit 
 * 功能: 初始化
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
void Rcv3BufInit(void)
{
    Rcv3BufClearAll(&uart3_Rx);
}


/*
 ***************************************************************
 * 名称: Uart3RcvData 
 * 功能: 串口3接收数据
 * 输入: 1. dat：串口接收到的数据
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
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

