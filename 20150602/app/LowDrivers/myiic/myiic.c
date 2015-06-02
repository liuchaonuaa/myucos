#include "myiic.h"
#include "delay.h"

/*
 ***************************************************************
 * ����: IIC_Init 
 * ����: STM32ģ��IIC��ʼ��
 * ����: ��
 * ���: NULL 
 * ����: ��
 * ����: �� 
 ***************************************************************
 */
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE, GPIO_Pin_8 | GPIO_Pin_9);
}


/*
 ***************************************************************
 * ����: IIC_Start 
 * ����: ����IIC��ʼ�ź�
 * ����: ��
 * ���: NULL 
 * ����: ��
 * ����: �� 
 ***************************************************************
 */
void IIC_Start(void)
{
	SDA_OUT();		// sda�����
	IIC_SDA = 1;	  	  
	IIC_SCL = 1;
	delay_us(4);
 	IIC_SDA = 0;	// START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL = 0;	// ǯסI2C���ߣ�׼�����ͻ�������� 
}	  


/*
 ***************************************************************
 * ����: IIC_Stop 
 * ����: ����IICֹͣ�ź�
 * ����: ��
 * ���: NULL 
 * ����: ��
 * ����: �� 
 ***************************************************************
 */
void IIC_Stop(void)
{
	SDA_OUT();		// sda�����
	IIC_SCL = 0;
	IIC_SDA = 0;	// STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL = 1; 
	IIC_SDA = 1;	// ����I2C���߽����ź�
	delay_us(4);							   	
}


/*
 ***************************************************************
 * ����: IIC_Wait_Ack 
 * ����: �ȴ�Ӧ���źŵ���
 * ����: ��
 * ���: NULL 
 * ����: 1������Ӧ��ʧ��
 *       0������Ӧ��ɹ�
 * ����: �� 
 ***************************************************************
 */
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	
	SDA_IN();      //SDA����Ϊ����  
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
	
	IIC_SCL = 0;	//ʱ�����0 	
	
	return 0;  
} 


/*
 ***************************************************************
 * ����: IIC_Ack 
 * ����: ����ACKӦ��
 * ����: ��
 * ���: NULL 
 * ����: ��
 * ����: �� 
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
 * ����: IIC_NAck 
 * ����: ����ACK��Ӧ��
 * ����: ��
 * ���: NULL 
 * ����: ��
 * ����: �� 
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
 * ����: IIC_Send_Byte 
 * ����: IIC��������
 * ����: 1. txd��Ҫ���͵�����
 * ���: NULL 
 * ����: ��
 * ����: �� 
 ***************************************************************
 */
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	
	SDA_OUT(); 	    
    IIC_SCL = 0;//����ʱ�ӿ�ʼ���ݴ���
   
	for (t = 0; t < 8; t++)
    {              
		if ((txd&0x80) >> 7)
			IIC_SDA = 1;
		else
			IIC_SDA = 0;
		
		txd <<= 1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL = 1;
		delay_us(2); 
		IIC_SCL = 0;	
		delay_us(2);
    }	 
} 	 


/*
 ***************************************************************
 * ����: IIC_Read_Byte 
 * ����: IIC������
 * ����: 1. ack��ack=1ʱ������ACK; ack=0������nACK  
 * ���: NULL 
 * ����: ��
 * ����: �� 
 ***************************************************************
 */
u8 IIC_Read_Byte(u8 ack)
{
	u8 i, receive = 0;
	
	SDA_IN();	// SDA����Ϊ����
 
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
        IIC_NAck();	// ����nACK
    else
        IIC_Ack(); 	// ����ACK   
	
    return receive;
}


/*
 ***************************************************************
 * ����: IIC_WriteNByte 
 * ����: IICд����
 * ����: 1. sla��IIC�����ĵ�ַ
 *       2. suba: IIC�����ڲ���ƫ�Ƶ�ַ
 *       3. *data: Ҫд������
 *       4. len: Ҫд�ĳ���
 * ���: 
 * ����: ��
 * ����: �� 
 ***************************************************************
 */
void IIC_WriteNByte(u8 sla, u8 suba, u8 *data, u8 len)
{
	u8 writelen = len;
	
	if (writelen == 0)
		return;
	
	IIC_Start();  

	IIC_Send_Byte(sla);			// ����������ַ,д���� 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(suba);		// ���͵͵�ַ
	IIC_Wait_Ack(); 	
	
	while (writelen--)
	{
		IIC_Send_Byte(*data++);	// �����ֽ�							   
		IIC_Wait_Ack();  
	}
		
    IIC_Stop();					// ����һ��ֹͣ���� 
}


/*
 ***************************************************************
 * ����: IIC_ReadNByte 
 * ����: IIC������
 * ����: 1. sla��IIC�����ĵ�ַ
 *       2. suba: IIC�����ڲ���ƫ�Ƶ�ַ
 *       3. len: Ҫ���ĳ���
 * ���: 1. *data: ����������
 * ����: ��
 * ����: �� 
 ***************************************************************
 */
void IIC_ReadNByte(u8 sla, u8 suba, u8 *data, u8 len)
{
	u8 readlen = len;
	
	if (readlen == 0)
		return;
	
	IIC_Start();  				 
	IIC_Send_Byte(sla);		// ����д����ָ��	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(suba);	// ���ͼĴ�����ַ(���ݻ������ʼ��ַΪ0X32)
	IIC_Wait_Ack(); 	 										  		   
 
 	IIC_Start();  	 		// ��������
	IIC_Send_Byte(sla+1);	// ���Ͷ�����ָ��
	IIC_Wait_Ack();
	
	while (readlen-- > 1)
	{
		*data++ = IIC_Read_Byte(1);
	}
	
	*data = IIC_Read_Byte(0);
	        	   
    IIC_Stop();				// ����һ��ֹͣ����	
}

