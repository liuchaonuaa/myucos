#include "sys.h"
#include "stm32spi1.h"
#include "delay.h"
#include "stm32f10x_spi.h"

/*
 ***************************************************************
 * 名称: SPI1_Init 
 * 功能: STM32硬件SPI初始化
 * 输入: NULL
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void SPI1_Init(void)
{	    
 	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);//PORTA，SPI1时钟使能 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// PA5.6.7复用推挽  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);	// PA5.6.7上拉
			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;     //NSS
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	// 设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		// 设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	// 设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;			// 选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;		// 数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			// NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;	// 定义波特率预分频的值:波特率预分频值为16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	// 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	// CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  // 根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); 	//使能SPI外设
}


/*
 ***************************************************************
 * 名称: SPI1_SetSpeed 
 * 功能: STM32硬件SPI初始化
 * 输入: 1. SpeedSet 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void SPI1_SetSpeed(u8 SpeedSet)
{
	SPI1->CR1 &= 0XFFC7; 
	SPI1->CR1 |= SpeedSet;
	SPI_Cmd(SPI1, ENABLE); 
} 


/*
 ***************************************************************
 * 名称: SPI1_ReadWriteByte 
 * 功能: STM32硬件SPI读写字节
 * 输入: 1. 要写入的字节
 * 输出: NULL 
 * 返回: 1. 读出的字节 
 * 描述: 无 
 ***************************************************************
 */
u8 SPI1_ReadWriteByte(u8 TxData)
{				   			 
	u8 retry = 0;
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) // 检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if (retry > 200)
			return 0;
	}	
	
	SPI_I2S_SendData(SPI1, TxData);	// 通过外设SPIx发送一个数据
	
	retry = 0;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) // 检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if (retry > 200)
			return 0;
	}	  			
	
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据					    
}

