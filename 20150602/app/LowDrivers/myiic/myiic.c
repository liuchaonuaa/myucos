#include "myiic.h"
#include "delay.h"

/*
 ***************************************************************
 * 名称: IIC_Init 
 * 功能: STM32模拟IIC初始化
 * 输入: 无
 * 输出: NULL 
 * 返回: 无
 * 描述: 无 
 ***************************************************************
 */
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE, GPIO_Pin_8 | GPIO_Pin_9);
}


/*
 ***************************************************************
 * 名称: IIC_Start 
 * 功能: 产生IIC起始信号
 * 输入: 无
 * 输出: NULL 
 * 返回: 无
 * 描述: 无 
 ***************************************************************
 */
void IIC_Start(void)
{
	SDA_OUT();		// sda线输出
	IIC_SDA = 1;	  	  
	IIC_SCL = 1;
	delay_us(4);
 	IIC_SDA = 0;	// START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL = 0;	// 钳住I2C总线，准备发送或接收数据 
}	  


/*
 ***************************************************************
 * 名称: IIC_Stop 
 * 功能: 产生IIC停止信号
 * 输入: 无
 * 输出: NULL 
 * 返回: 无
 * 描述: 无 
 ***************************************************************
 */
void IIC_Stop(void)
{
	SDA_OUT();		// sda线输出
	IIC_SCL = 0;
	IIC_SDA = 0;	// STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL = 1; 
	IIC_SDA = 1;	// 发送I2C总线结束信号
	delay_us(4);							   	
}


/*
 ***************************************************************
 * 名称: IIC_Wait_Ack 
 * 功能: 等待应答信号到来
 * 输入: 无
 * 输出: NULL 
 * 返回: 1，接收应答失败
 *       0，接收应答成功
 * 描述: 无 
 ***************************************************************
 */
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	
	SDA_IN();      //SDA设置为输入  
	IIC_SDA = 1;
	delay_us(1);	   
	IIC_SCL = 1;
	delay_us(1);
	
	while (READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	
	IIC_SCL = 0;	//时钟输出0 	
	
	return 0;  
} 


/*
 ***************************************************************
 * 名称: IIC_Ack 
 * 功能: 产生ACK应答
 * 输入: 无
 * 输出: NULL 
 * 返回: 无
 * 描述: 无 
 ***************************************************************
 */
void IIC_Ack(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 0;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}


/*
 ***************************************************************
 * 名称: IIC_NAck 
 * 功能: 产生ACK非应答
 * 输入: 无
 * 输出: NULL 
 * 返回: 无
 * 描述: 无 
 ***************************************************************
 */
void IIC_NAck(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 1;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(2);
	IIC_SCL = 0;
}		


/*
 ***************************************************************
 * 名称: IIC_Send_Byte 
 * 功能: IIC发送数据
 * 输入: 1. txd：要发送的数据
 * 输出: NULL 
 * 返回: 无
 * 描述: 无 
 ***************************************************************
 */
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	
	SDA_OUT(); 	    
    IIC_SCL = 0;//拉低时钟开始数据传输
   
	for (t = 0; t < 8; t++)
    {              
		if ((txd&0x80) >> 7)
			IIC_SDA = 1;
		else
			IIC_SDA = 0;
		
		txd <<= 1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL = 1;
		delay_us(2); 
		IIC_SCL = 0;	
		delay_us(2);
    }	 
} 	 


/*
 ***************************************************************
 * 名称: IIC_Read_Byte 
 * 功能: IIC读数据
 * 输入: 1. ack：ack=1时，发送ACK; ack=0，发送nACK  
 * 输出: NULL 
 * 返回: 无
 * 描述: 无 
 ***************************************************************
 */
u8 IIC_Read_Byte(u8 ack)
{
	u8 i, receive = 0;
	
	SDA_IN();	// SDA设置为输入
 
	for (i = 0; i < 8; i++ )
	{
        IIC_SCL = 0; 
        delay_us(2);
		IIC_SCL = 1;
        receive <<= 1;
        if (READ_SDA)
			receive++;   
		delay_us(1); 
    }		
	
    if (!ack)
        IIC_NAck();	// 发送nACK
    else
        IIC_Ack(); 	// 发送ACK   
	
    return receive;
}


/*
 ***************************************************************
 * 名称: IIC_WriteNByte 
 * 功能: IIC写数据
 * 输入: 1. sla：IIC器件的地址
 *       2. suba: IIC器件内部的偏移地址
 *       3. *data: 要写的数据
 *       4. len: 要写的长度
 * 输出: 
 * 返回: 无
 * 描述: 无 
 ***************************************************************
 */
void IIC_WriteNByte(u8 sla, u8 suba, u8 *data, u8 len)
{
	u8 writelen = len;
	
	if (writelen == 0)
		return;
	
	IIC_Start();  

	IIC_Send_Byte(sla);			// 发送器件地址,写数据 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(suba);		// 发送低地址
	IIC_Wait_Ack(); 	
	
	while (writelen--)
	{
		IIC_Send_Byte(*data++);	// 发送字节							   
		IIC_Wait_Ack();  
	}
		
    IIC_Stop();					// 产生一个停止条件 
}


/*
 ***************************************************************
 * 名称: IIC_ReadNByte 
 * 功能: IIC读数据
 * 输入: 1. sla：IIC器件的地址
 *       2. suba: IIC器件内部的偏移地址
 *       3. len: 要读的长度
 * 输出: 1. *data: 读出的数据
 * 返回: 无
 * 描述: 无 
 ***************************************************************
 */
void IIC_ReadNByte(u8 sla, u8 suba, u8 *data, u8 len)
{
	u8 readlen = len;
	
	if (readlen == 0)
		return;
	
	IIC_Start();  				 
	IIC_Send_Byte(sla);		// 发送写器件指令	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(suba);	// 发送寄存器地址(数据缓存的起始地址为0X32)
	IIC_Wait_Ack(); 	 										  		   
 
 	IIC_Start();  	 		// 重新启动
	IIC_Send_Byte(sla+1);	// 发送读器件指令
	IIC_Wait_Ack();
	
	while (readlen-- > 1)
	{
		*data++ = IIC_Read_Byte(1);
	}
	
	*data = IIC_Read_Byte(0);
	        	   
    IIC_Stop();				// 产生一个停止条件	
}

