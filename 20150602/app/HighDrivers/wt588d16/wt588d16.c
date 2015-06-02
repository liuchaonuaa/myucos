#include "sys.h"
#include "delay.h"
#include "wt588d16.h"
#include "usart3.h"


u8 voiceFlashEraseFlag;
u8 voicebuf[1024];
u32  pagebuf[PAGE_SIZE/4];


/*
 ***************************************************************
 * ����: spi2_delay 
 * ����: ģ��spi����ʱ
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void spi2_delay(vu32 nCount)
{
	nCount *= 6;
	for (; nCount != 0; nCount--);
}


/*
 ***************************************************************
 * ����: Voice_DelayMs 
 * ����: ��ʱ
 * ����: 1. ms ��ʱ��ʱ�䣨ms��
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void Voice_DelayMs(u16 ms)
{
    u16 i;

	while (ms--)
	{
	    for (i = 12000; i > 0; i--)	
		{
		    ;
		}
	}
}


/*
 ***************************************************************
 * ����: Wt588d16_Reset 
 * ����: Wt588d16����оƬ��λ
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void Wt588d16_Reset(void)
{
	SPK2_RESET_0;
	Voice_DelayMs(100);
	SPK2_RESET_1;		
}


/*
 *****************************
 * SPK2_RST   <-----> PE1
 * SPK2_MOSI  <-----> PD4
 * SPK2_MISO  <-----> PD5
 * SPK2_CLK   <-----> PD7
 * SPK2_CS    <-----> PD11
 * SPK2_DATA  <-----> PE10
 *****************************
 */
/*
 ***************************************************************
 * ����: Wt588d16_Pin_Init 
 * ����: Wt588d16����оƬ��ʼ��
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void Wt588d16_Pin_Init(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);
							  	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_10;		 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	VOICE2_DATA_1;
	Wt588d16_Reset();
}


/*
 ***************************************************************
 * ����: VoiceUpdate_Pin_Init 
 * ����: Wt588d16����оƬ�������µĹܽ�����
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void VoiceUpdate_Pin_Init(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_7 | GPIO_Pin_11;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

	MYSPI2_NSS_1;
}


/*
 ***************************************************************
 * ����: VoiceNormal_Pin_Config 
 * ����: Wt588d16����оƬ������ʹ���µ�����
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void VoiceNormal_Pin_Config(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_7 | GPIO_Pin_11;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

	MYSPI2_NSS_1;
}


/*
 ***************************************************************
 * ����: Wt588d16_Play 
 * ����: Wt588d16����оƬ����ĳ������
 * ����: 1. addr ĳ�������ĵ�ַ 
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void Wt588d16_Play(u8 addr)
{
	u8 i;		 	
	
	VOICE2_DATA_1;		 				/* ���������� 	*/
	delay_ms(20);						
	VOICE2_DATA_0;		 				/* ���������� 	*/
	delay_ms(5);

	for (i = 0; i < 8; i++)
	{
		VOICE2_DATA_1;	 				/* ���������� 	*/
		if (addr & 0x01)
		{
			delay_us(600);  			/* ��ʱ600us 	*/
			VOICE2_DATA_0;				/* ���������� 	*/	 	  
			delay_us(200);	 			/* ��ʱ200us 	*/
		}
		else 
		{
			delay_us(200);	 			/* ��ʱ200us 	*/
			VOICE2_DATA_0;				/* ���������� 	*/		
		    delay_us(600);	 			/* ��ʱ600us 	*/
		}
		addr>>=1; 
	}
	
	VOICE2_DATA_1;		 				/* ���������� 	*/
}


/*
 ***************************************************************
 * ����: MYSPI2_ReadByte 
 * ����: ģ��spi�Ķ��ֽ�
 * ����: NULL
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
u8 MYSPI2_ReadByte(void)
{
	u8 i, Temp = 0;

	for (i = 0; i < 8; i++)                   
	{
		Temp <<= 1; 
		Temp |= MYSPI2_MISO;
		MYSPI2_SCK_1;                   
		spi2_delay(6);
		MYSPI2_SCK_0; 
		spi2_delay(6);		
	}

	return Temp;
}


/*
 ***************************************************************
 * ����: MYSPI2_ReadString 
 * ����: ģ��spi�Ķ�����ֽ�
 * ����: 1. length: Ҫ�����ֽڳ���
 * ���: 1. *buff: �������ֽڵĴ��λ��
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void MYSPI2_ReadString(u8 *buff, const u16 length)
{
  	u16 i;

  	for (i = 0; i < length; i++)
  	{
		*buff = MYSPI2_ReadByte();
		buff++;
  	}
}


/*
 ***************************************************************
 * ����: MYSPI2_WriteByte 
 * ����: ģ��spi��д�ֽ�
 * ����: 1. Data:ģ��spiҪд���ֽ�
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void MYSPI2_WriteByte(u8 Data)
{
	unsigned char i;

	for (i = 0; i < 8; i++)          		
	{
		spi2_delay(6);	
		
		if (Data & 0x80) 
			MYSPI2_MOSI_1;     	
		else 
			MYSPI2_MOSI_0;
		
		spi2_delay(6);
		MYSPI2_SCK_1;                   
		Data <<= 1;
		spi2_delay(6);
		MYSPI2_SCK_0; 						
	} 
}



/*
 ***************************************************************
 * ����: SPIFLASH_ChipInfo 
 * ����: ��ȡSPI Flash��ID
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: C8 40 16 // GD25Q16
 ***************************************************************
 */
void SPIFLASH_ChipInfo(void)
{
	u8 chipInfo[3];
	
	MYSPI2_NSS_0;
	MYSPI2_WriteByte(0x9f);
	chipInfo[0] = MYSPI2_ReadByte();
	chipInfo[1] = MYSPI2_ReadByte();
	chipInfo[2] = MYSPI2_ReadByte();
	MYSPI2_NSS_1;
}


/*
 ***************************************************************
 * ����: VoiceRead_CMD 
 * ����: ��ȡSPI Flash��״̬�Ĵ���
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: NULL 
 ***************************************************************
 */
u8 VoiceRead_CMD(void)
{
	u8 busy;
	
	MYSPI2_NSS_0;
	
	MYSPI2_WriteByte(0x05);
	busy = MYSPI2_ReadByte();
	
	MYSPI2_NSS_1;

	return (busy);
}


/*
 ***************************************************************
 * ����: VoiceBusy_Test 
 * ����: ���SPI FLASH��æ���
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: NULL
 ***************************************************************
 */
void VoiceBusy_Test(void)
{
	u8 a = 1;
	
	while ((a&0x01) == 1) 
		a = VoiceRead_CMD();	
}


/*
 ***************************************************************
 * ����: SPIFLASH_Write_EN 
 * ����: ��SpiFlash��д
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: NULL
 ***************************************************************
 */
void SPIFLASH_Write_EN(void)
{
	MYSPI2_NSS_0;			// SPI CS ʹ��
	MYSPI2_WriteByte(0x06);	// дʹ��ָ��
	MYSPI2_NSS_1;			// SPI CS ����
}


/*
 ***************************************************************
 * ����: SPIFLASH_Write_EN 
 * ����: ��SpiFlash����д
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: NULL
 ***************************************************************
 */
void SPIFLASH_Write_DIS(void)
{
	MYSPI2_NSS_0;  			// SPI CS ʹ��
	MYSPI2_WriteByte(0x04);	// д��ָֹ��
	MYSPI2_NSS_1;			// SPI CS ����
	
	VoiceBusy_Test();		// æ���
}	


/*
 ***************************************************************
 * ����: SPIFLASH_EraseSector 
 * ����: ��������
 * ����: SecNum��������0~511  ������С4KB
 * ���: NULL 
 * ����: NULL 
 * ����: NULL
 ***************************************************************
 */
void SPIFLASH_EraseSector(u16 SecNum)  //������0~511  
{
	u32 addr;
	
	addr = SecNum*0x1000;
	
	SPIFLASH_Write_EN();
	MYSPI2_NSS_0;	// SPI CS ʹ��

	MYSPI2_WriteByte(0x20);
	MYSPI2_WriteByte((addr&0xffffff)>>16);
	MYSPI2_WriteByte((addr&0xffff)>>8);
	MYSPI2_WriteByte(addr&0xff);
	
	MYSPI2_NSS_1;	// SPI CS ����
	
	SPIFLASH_Write_DIS();
	
	VoiceBusy_Test();	// æ���
}


/*
 ***************************************************************
 * ����: SPIFLASH_EraseChip 
 * ����: ��������оƬ
 * ����: NULL
 * ���: NULL 
 * ����: NULL 
 * ����: NULL
 ***************************************************************
 */
void SPIFLASH_EraseChip(void)  
{
	SPIFLASH_Write_EN();

	MYSPI2_NSS_0;	// SPI CS ʹ��
	MYSPI2_WriteByte(0x60);
	MYSPI2_NSS_1;	// SPI CS ����

	SPIFLASH_Write_DIS();
	
	VoiceBusy_Test();	// æ���
}


/*
 ***************************************************************
 * ����: SPIFLASH_EraseSecs 
 * ����: �����������
 * ����: 1. sec��Ҫ��������ʼ����
 *       2. num��Ҫ��������������
 * ���: NULL 
 * ����: NULL 
 * ����: NULL
 ***************************************************************
 */
void SPIFLASH_EraseSecs(u16 sec, u16 num)
{
	u16 i;
	
	for (i = 0; i < num; i++)
	{
		if ((sec + i) < 512)
			SPIFLASH_EraseSector(sec + i);
	}
}


/*
 ***************************************************************
 * ����: VoiceFLASH_Read 
 * ����: ��SPI Flash�϶�����
 * ����: 1. addr��Ҫ��ȡSPI FLASH����ʼ��ַ
 *       2. len��Ҫ���ĳ���
 * ���: 1. *data����ȡ�������� 
 * ����: NULL 
 * ����: NULL
 ***************************************************************
 */
void VoiceFLASH_Read(u32 addr, u8 *data, u16 len)
{
	u16 i;
		
	MYSPI2_NSS_0;	// SPI CS ʹ��
	
	MYSPI2_WriteByte(0x03);
	MYSPI2_WriteByte((addr&0xffffff)>>16);
	MYSPI2_WriteByte((addr&0xffff)>>8);
	MYSPI2_WriteByte(addr&0xff);
	
	for (i = 0; i < len; i++)
	{
		*(data+i) = MYSPI2_ReadByte();
	}
	
	MYSPI2_NSS_1;	// SPI CS ����
	
	VoiceBusy_Test();	// æ���
}


/*
 ***************************************************************
 * ����: SPI_FLASH_Write 
 * ����: ��SPI Flash ��д����
 * ����: 1. addr��Ҫд��SPI FLASH����ʼ��ַ
 *       2. *data��Ҫд������
 *       3. len��Ҫд�ĳ���
 * ���: NULL
 * ����: NULL 
 * ����: NULL
 ***************************************************************
 */
void SPI_FLASH_Write(u32 addr, u8 *data, u16 len)
{
	u16 i;	
	u32 copylen;
	
	VoiceBusy_Test();	// æ���
	SPIFLASH_Write_EN();
	
	while (len)
	{
		MYSPI2_NSS_0;	// SPI CS ʹ��
		MYSPI2_WriteByte(PageProgram);
		MYSPI2_WriteByte((addr>>16) & 0xff);
		MYSPI2_WriteByte((addr>>8) & 0xff);
		MYSPI2_WriteByte(addr & 0xff);
		
		copylen = len > PAGE_SIZE? PAGE_SIZE: len;
		
		for (i = 0; i < copylen; i++)
		{
			MYSPI2_WriteByte(*data);
			data++;
		}
		
		MYSPI2_NSS_1;	// SPI CS ����
		VoiceBusy_Test();	// æ���
		addr += PAGE_SIZE;
		len -= copylen;		
	}
	
	SPIFLASH_Write_DIS();
}


/*
 ***************************************************************
 * ����: VoiceFlash_Write 
 * ����: д����
 * ����: 1. WriteAddr��Ҫд��SPI FLASH����ʼ��ַ
 *       2. *pBuffer��Ҫд������
 *       3. NumByteToWrite��Ҫд�ĳ���
 * ���: NULL
 * ����: NULL 
 * ����: NULL
 ***************************************************************
 */
void VoiceFlash_Write(u32 WriteAddr, u8 *pBuffer,  u16 NumByteToWrite)
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	Addr 		= WriteAddr % PAGE_SIZE;
	count 		= PAGE_SIZE - Addr;
	NumOfPage 	= NumByteToWrite / PAGE_SIZE;
	NumOfSingle = NumByteToWrite % PAGE_SIZE;
	
	if (Addr == 0) /*!< ��ַ��ҳ��С����  */
	{
		if (NumOfPage == 0) /*!< NumByteToWrite < PAGE_SIZE */
		{
			SPI_FLASH_Write(WriteAddr, pBuffer, NumByteToWrite);
		}
		else /*!< NumByteToWrite > PAGE_SIZE */
		{
			while (NumOfPage--)
			{
				SPI_FLASH_Write(WriteAddr,pBuffer, PAGE_SIZE);
				WriteAddr +=  PAGE_SIZE;
				pBuffer += PAGE_SIZE;
			}                        
			SPI_FLASH_Write(WriteAddr, pBuffer, NumOfSingle);
		}
	}
	else /*!< ��ַ��ҳ��С������ */
	{
		if (NumOfPage == 0) /*!< NumByteToWrite < PAGE_SIZE */
		{
			if (NumOfSingle > count) /*!< (NumByteToWrite + WriteAddr) > PAGE_SIZE */
			{
				temp = NumOfSingle - count;
				
				SPI_FLASH_Write(WriteAddr, pBuffer, count);
				WriteAddr += count;
				pBuffer += count;
				
				SPI_FLASH_Write(WriteAddr, pBuffer, temp);
			}
			else
			{
				SPI_FLASH_Write(WriteAddr, pBuffer, NumByteToWrite);
			}
		}
		else /*!< NumByteToWrite > PAGE_SIZE */
		{
			NumByteToWrite -= count;
			NumOfPage = NumByteToWrite / PAGE_SIZE;
			NumOfSingle = NumByteToWrite % PAGE_SIZE;
			
			SPI_FLASH_Write(WriteAddr, pBuffer, count);
			WriteAddr += count;
			pBuffer += count;
			
			while (NumOfPage--)
			{
				SPI_FLASH_Write(WriteAddr, pBuffer, PAGE_SIZE);
				WriteAddr += PAGE_SIZE;
				pBuffer   += PAGE_SIZE;
			}
			
			if (NumOfSingle != 0)
			{
				SPI_FLASH_Write(WriteAddr, pBuffer, NumOfSingle);
			}
		}
	}
}


/*
 ***************************************************************
 * ����: voiceFlashErase 
 * ����: ��������оƬ
 * ����: NULL
 * ���: NULL
 * ����: NULL 
 * ����: NULL
 ***************************************************************
 */
void voiceFlashErase(void)
{
	SPIFLASH_EraseSecs(0, 512);
}


/*
 ***************************************************************
 * ����: voiceFlashErase 
 * ����: ��������оƬ
 * ����: 1. BlockAddr���������׵�ַ
 *       2. BlockSize��Ҫ�����Ĵ�С
 * ���: NULL
 * ����: NULL 
 * ����: NULL
 ***************************************************************
 */
u32 SPI_FLASH_ERASE(u32 BlockAddr, u32 BlockSize)
{
	u8 block_cnt;
	
	VoiceBusy_Test();
	block_cnt = (BlockSize + BLOCK_SIZE -1)/BLOCK_SIZE;
	
	while (block_cnt--) 
	{
		SPIFLASH_Write_EN();
		MYSPI2_NSS_0;	// SPI CS ʹ��
		MYSPI2_WriteByte(BlockErase);
		MYSPI2_WriteByte((BlockAddr>>16)&0xFF);
		MYSPI2_WriteByte((BlockAddr>>8)&0xFF);
		MYSPI2_WriteByte(BlockAddr&0xFF);
		MYSPI2_NSS_1;	// SPI CS ����
		VoiceBusy_Test();
		SPIFLASH_Write_DIS();
		BlockAddr += BLOCK_SIZE;
	}
	
	return 0;
}


/*
 ***************************************************************
 * ����: EraseDataProgram 
 * ����: ��������оƬ���ж��Ƿ��д�ɾ�
 * ����: NULL
 * ���: NULL
 * ����: NULL 
 * ����: NULL
 ***************************************************************
 */
u8  EraseDataProgram(void)
{
	u32 i, cntr;
	
	SPI_FLASH_ERASE(0, FLASH_SIZE);  				// ��BLOCK ����оƬ����С��ƬоƬ
	
	/* ��������Ƿ�ȫ�� */
	for (cntr=0; cntr < FLASH_SIZE; cntr += PAGE_SIZE) 
	{
		VoiceFLASH_Read((u32)(0 + cntr), (u8 *)pagebuf, PAGE_SIZE);
		for (i=0; i<(PAGE_SIZE/4); i++) 
		{
			if (pagebuf[i] != 0xFFFFFFFF) 
			{
				return(1);	// error
			}
		}
	}
	
	return (0);	// ok	
}


/*
 ***************************************************************
 * ����: voiceWrite 
 * ����: ����оƬ��������
 * ����: 1. index��Ҫ�������ݵ�֡����
 *       2. datalen����֡�������ݵĳ���
 *       3. *data: Ҫ���µ�����
 * ���: NULL
 * ����: NULL 
 * ����: NULL
 ***************************************************************
 */
void voiceWrite(u16 index, u16 datalen, u8 *data)
{
	u32 addr = 0;
	
	addr = index*1024;
	
	if (datalen < 1024)
	{
		VoiceFlash_Write(addr, data, 1024);
		voiceFlashEraseFlag = 0;
		uart3_putString("OK");
	}
	else
	{
		VoiceFlash_Write(addr, data, 1024);
		uart3_putString("Next");			
	}
}


/*
 ***************************************************************
 * ����: voiceFlashTest 
 * ����: ����оƬ����
 * ����: NULL
 * ���: NULL
 * ����: NULL 
 * ����: NULL
 ***************************************************************
 */
void voiceFlashTest(void)
{
	u8 i;
	u8 flag = 0;
	
	if (flag)
		SPIFLASH_EraseChip();
	
	for (i = 0; i < 255; i++)
		VoiceFLASH_Read(i*1024, voicebuf, 1024);
}

