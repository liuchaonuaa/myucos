#include "sys.h"
#include "myspi.h"
#include "delay.h"
#include "stm32f10x_spi.h"

/*
 ***************************************************************
 * ����: spi_delay 
 * ����: ģ��spi��ʱ
 * ����: 1.nCount 
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void spi_delay(vu32 nCount)
{
	nCount *= 4;
	for (; nCount != 0; nCount--);
}


/*******************************
 *1--SCK  <-----> PC10
 *2--MISO <-----> PC11 
 *3--MOSI <-----> PC12
 *4--NSS  <-----> PE0
************************************/
/*
 ***************************************************************
 * ����: spi_delay 
 * ����: ģ��spi�ܽų�ʼ��
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void MYSPI_Init(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOE, ENABLE );	//PORTC PORTE ʱ��ʹ�� 

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
}

/*
 ***************************************************************
 * ����: Send_1_Char 
 * ����: ģ��spi�����ֽ�
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void Send_1_Char(unsigned char Data)
{   
	unsigned char i;

	for (i = 0; i < 8; i++)          		
	{		
		if (Data & 0x80) 
			MYSPI_MOSI_1;     	
		else 
			MYSPI_MOSI_0;
		
		spi_delay(6);
		MYSPI_SCK_1;                   
		Data <<= 1;
		spi_delay(6);
		MYSPI_SCK_0; 						
	} 
}



/*
 ***************************************************************
 * ����: Rec_1_Char 
 * ����: ģ��spi�����ֽ�
 * ����: NULL 
 * ���: NULL 
 * ����: ���ؽ��յ�������
 * ����: �� 
 ***************************************************************
 */
unsigned char  Rec_1_Char(void)
{
	unsigned char i, Temp = 0;

	for (i = 0; i < 8; i++)                   
	{
		Temp <<= 1; 
		Temp |= MYSPI_MISO;
		MYSPI_SCK_1;                   
		spi_delay(6);
		MYSPI_SCK_0; 
		spi_delay(6);		
	}

	return Temp;
}
