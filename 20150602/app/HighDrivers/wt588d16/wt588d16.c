#include "sys.h"
#include "delay.h"
#include "wt588d16.h"
#include "usart3.h"


u8 voiceFlashEraseFlag;
u8 voicebuf[1024];
u32  pagebuf[PAGE_SIZE/4];


/*
 ***************************************************************
 * 名称: spi2_delay 
 * 功能: 模拟spi的延时
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void spi2_delay(vu32 nCount)
{
	nCount *= 6;
	for (; nCount != 0; nCount--);
}


/*
 ***************************************************************
 * 名称: Voice_DelayMs 
 * 功能: 延时
 * 输入: 1. ms 延时的时间（ms）
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
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
 * 名称: Wt588d16_Reset 
 * 功能: Wt588d16语音芯片复位
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
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
 * 名称: Wt588d16_Pin_Init 
 * 功能: Wt588d16语音芯片初始化
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
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
 * 名称: VoiceUpdate_Pin_Init 
 * 功能: Wt588d16语音芯片在升级下的管脚配置
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
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
 * 名称: VoiceNormal_Pin_Config 
 * 功能: Wt588d16语音芯片在正常使用下的配置
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
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
 * 名称: Wt588d16_Play 
 * 功能: Wt588d16语音芯片播放某段语音
 * 输入: 1. addr 某段语音的地址 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void Wt588d16_Play(u8 addr)
{
	u8 i;		 	
	
	VOICE2_DATA_1;		 				/* 拉高数据线 	*/
	delay_ms(20);						
	VOICE2_DATA_0;		 				/* 拉低数据线 	*/
	delay_ms(5);

	for (i = 0; i < 8; i++)
	{
		VOICE2_DATA_1;	 				/* 拉高数据线 	*/
		if (addr & 0x01)
		{
			delay_us(600);  			/* 延时600us 	*/
			VOICE2_DATA_0;				/* 拉低数据线 	*/	 	  
			delay_us(200);	 			/* 延时200us 	*/
		}
		else 
		{
			delay_us(200);	 			/* 延时200us 	*/
			VOICE2_DATA_0;				/* 拉低数据线 	*/		
		    delay_us(600);	 			/* 延时600us 	*/
		}
		addr>>=1; 
	}
	
	VOICE2_DATA_1;		 				/* 拉高数据线 	*/
}


/*
 ***************************************************************
 * 名称: MYSPI2_ReadByte 
 * 功能: 模拟spi的读字节
 * 输入: NULL
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
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
 * 名称: MYSPI2_ReadString 
 * 功能: 模拟spi的读多个字节
 * 输入: 1. length: 要读的字节长度
 * 输出: 1. *buff: 读到的字节的存放位置
 * 返回: NULL 
 * 描述: 无 
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
 * 名称: MYSPI2_WriteByte 
 * 功能: 模拟spi的写字节
 * 输入: 1. Data:模拟spi要写的字节
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
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
 * 名称: SPIFLASH_ChipInfo 
 * 功能: 读取SPI Flash的ID
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: C8 40 16 // GD25Q16
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
 * 名称: VoiceRead_CMD 
 * 功能: 读取SPI Flash的状态寄存器
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: NULL 
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
 * 名称: VoiceBusy_Test 
 * 功能: 检测SPI FLASH的忙标记
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: NULL
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
 * 名称: SPIFLASH_Write_EN 
 * 功能: 让SpiFlash可写
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: NULL
 ***************************************************************
 */
void SPIFLASH_Write_EN(void)
{
	MYSPI2_NSS_0;			// SPI CS 使能
	MYSPI2_WriteByte(0x06);	// 写使能指令
	MYSPI2_NSS_1;			// SPI CS 禁用
}


/*
 ***************************************************************
 * 名称: SPIFLASH_Write_EN 
 * 功能: 让SpiFlash不可写
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: NULL
 ***************************************************************
 */
void SPIFLASH_Write_DIS(void)
{
	MYSPI2_NSS_0;  			// SPI CS 使能
	MYSPI2_WriteByte(0x04);	// 写禁止指令
	MYSPI2_NSS_1;			// SPI CS 禁用
	
	VoiceBusy_Test();		// 忙检测
}	


/*
 ***************************************************************
 * 名称: SPIFLASH_EraseSector 
 * 功能: 扇区擦除
 * 输入: SecNum：扇区号0~511  扇区大小4KB
 * 输出: NULL 
 * 返回: NULL 
 * 描述: NULL
 ***************************************************************
 */
void SPIFLASH_EraseSector(u16 SecNum)  //扇区号0~511  
{
	u32 addr;
	
	addr = SecNum*0x1000;
	
	SPIFLASH_Write_EN();
	MYSPI2_NSS_0;	// SPI CS 使能

	MYSPI2_WriteByte(0x20);
	MYSPI2_WriteByte((addr&0xffffff)>>16);
	MYSPI2_WriteByte((addr&0xffff)>>8);
	MYSPI2_WriteByte(addr&0xff);
	
	MYSPI2_NSS_1;	// SPI CS 禁用
	
	SPIFLASH_Write_DIS();
	
	VoiceBusy_Test();	// 忙检测
}


/*
 ***************************************************************
 * 名称: SPIFLASH_EraseChip 
 * 功能: 擦除整个芯片
 * 输入: NULL
 * 输出: NULL 
 * 返回: NULL 
 * 描述: NULL
 ***************************************************************
 */
void SPIFLASH_EraseChip(void)  
{
	SPIFLASH_Write_EN();

	MYSPI2_NSS_0;	// SPI CS 使能
	MYSPI2_WriteByte(0x60);
	MYSPI2_NSS_1;	// SPI CS 禁用

	SPIFLASH_Write_DIS();
	
	VoiceBusy_Test();	// 忙检测
}


/*
 ***************************************************************
 * 名称: SPIFLASH_EraseSecs 
 * 功能: 擦除多个扇区
 * 输入: 1. sec：要擦除的起始扇区
 *       2. num：要擦除的扇区个数
 * 输出: NULL 
 * 返回: NULL 
 * 描述: NULL
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
 * 名称: VoiceFLASH_Read 
 * 功能: 从SPI Flash上读数据
 * 输入: 1. addr：要读取SPI FLASH的起始地址
 *       2. len：要读的长度
 * 输出: 1. *data：读取到的数据 
 * 返回: NULL 
 * 描述: NULL
 ***************************************************************
 */
void VoiceFLASH_Read(u32 addr, u8 *data, u16 len)
{
	u16 i;
		
	MYSPI2_NSS_0;	// SPI CS 使能
	
	MYSPI2_WriteByte(0x03);
	MYSPI2_WriteByte((addr&0xffffff)>>16);
	MYSPI2_WriteByte((addr&0xffff)>>8);
	MYSPI2_WriteByte(addr&0xff);
	
	for (i = 0; i < len; i++)
	{
		*(data+i) = MYSPI2_ReadByte();
	}
	
	MYSPI2_NSS_1;	// SPI CS 禁用
	
	VoiceBusy_Test();	// 忙检测
}


/*
 ***************************************************************
 * 名称: SPI_FLASH_Write 
 * 功能: 向SPI Flash 上写数据
 * 输入: 1. addr：要写的SPI FLASH的起始地址
 *       2. *data：要写的数据
 *       3. len：要写的长度
 * 输出: NULL
 * 返回: NULL 
 * 描述: NULL
 ***************************************************************
 */
void SPI_FLASH_Write(u32 addr, u8 *data, u16 len)
{
	u16 i;	
	u32 copylen;
	
	VoiceBusy_Test();	// 忙检测
	SPIFLASH_Write_EN();
	
	while (len)
	{
		MYSPI2_NSS_0;	// SPI CS 使能
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
		
		MYSPI2_NSS_1;	// SPI CS 禁用
		VoiceBusy_Test();	// 忙检测
		addr += PAGE_SIZE;
		len -= copylen;		
	}
	
	SPIFLASH_Write_DIS();
}


/*
 ***************************************************************
 * 名称: VoiceFlash_Write 
 * 功能: 写数据
 * 输入: 1. WriteAddr：要写的SPI FLASH的起始地址
 *       2. *pBuffer：要写的数据
 *       3. NumByteToWrite：要写的长度
 * 输出: NULL
 * 返回: NULL 
 * 描述: NULL
 ***************************************************************
 */
void VoiceFlash_Write(u32 WriteAddr, u8 *pBuffer,  u16 NumByteToWrite)
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	Addr 		= WriteAddr % PAGE_SIZE;
	count 		= PAGE_SIZE - Addr;
	NumOfPage 	= NumByteToWrite / PAGE_SIZE;
	NumOfSingle = NumByteToWrite % PAGE_SIZE;
	
	if (Addr == 0) /*!< 地址与页大小对齐  */
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
	else /*!< 地址与页大小不对齐 */
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
 * 名称: voiceFlashErase 
 * 功能: 擦除整个芯片
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL 
 * 描述: NULL
 ***************************************************************
 */
void voiceFlashErase(void)
{
	SPIFLASH_EraseSecs(0, 512);
}


/*
 ***************************************************************
 * 名称: voiceFlashErase 
 * 功能: 擦除整个芯片
 * 输入: 1. BlockAddr：擦除的首地址
 *       2. BlockSize：要擦除的大小
 * 输出: NULL
 * 返回: NULL 
 * 描述: NULL
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
		MYSPI2_NSS_0;	// SPI CS 使能
		MYSPI2_WriteByte(BlockErase);
		MYSPI2_WriteByte((BlockAddr>>16)&0xFF);
		MYSPI2_WriteByte((BlockAddr>>8)&0xFF);
		MYSPI2_WriteByte(BlockAddr&0xFF);
		MYSPI2_NSS_1;	// SPI CS 禁用
		VoiceBusy_Test();
		SPIFLASH_Write_DIS();
		BlockAddr += BLOCK_SIZE;
	}
	
	return 0;
}


/*
 ***************************************************************
 * 名称: EraseDataProgram 
 * 功能: 擦除整个芯片并判断是否擦写干净
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL 
 * 描述: NULL
 ***************************************************************
 */
u8  EraseDataProgram(void)
{
	u32 i, cntr;
	
	SPI_FLASH_ERASE(0, FLASH_SIZE);  				// 按BLOCK 擦除芯片，大小整片芯片
	
	/* 检查擦完的是否全空 */
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
 * 名称: voiceWrite 
 * 功能: 语音芯片更新数据
 * 输入: 1. index：要更新数据的帧索引
 *       2. datalen：这帧更新数据的长度
 *       3. *data: 要更新的数据
 * 输出: NULL
 * 返回: NULL 
 * 描述: NULL
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
 * 名称: voiceFlashTest 
 * 功能: 语音芯片测试
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL 
 * 描述: NULL
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

