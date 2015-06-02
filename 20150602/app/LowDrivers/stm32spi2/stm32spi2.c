#include "sys.h"
#include "stm32spi2.h"
#include "delay.h"
#include "stm32f10x_spi.h"

/*
 ***************************************************************
 * ����: SPI2_Init 
 * ����: STM32Ӳ��SPI��ʼ��
 * ����: NULL
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void SPI2_Init(void)
{	    
	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// PORTBʱ��ʹ�� 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,  ENABLE);	// SPI2ʱ��ʹ�� 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);  //PB13/14/15����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;     //NSS
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		// ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	// ���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		// NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		// ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	// CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  // ����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
}


/*
 ***************************************************************
 * ����: SPI1_SetSpeed 
 * ����: STM32Ӳ��SPI��ʼ��
 * ����: 1. SpeedSet 
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void SPI2_SetSpeed(u8 SpeedSet)
{
	SPI2->CR1 &= 0XFFC7; 
	SPI2->CR1 |= SpeedSet;
	SPI_Cmd(SPI2, ENABLE); 
} 


/*
 ***************************************************************
 * ����: SPI2_ReadWriteByte 
 * ����: STM32Ӳ��SPI��д�ֽ�
 * ����: 1. Ҫд����ֽ�
 * ���: NULL 
 * ����: 1. �������ֽ� 
 * ����: �� 
 ***************************************************************
 */
u8 SPI2_ReadWriteByte(u8 TxData)
{				   			 
	u8 retry = 0;				 	
	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if (retry > 200)
			return 0;
	}	
	
	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
	
	retry = 0;
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if (retry > 200)
			return 0;
	}	
	
	return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����					    			    
}

