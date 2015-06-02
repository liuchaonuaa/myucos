#include "sys.h"
#include "delay.h"
#include "mySpi.h"
#include "sst25VF016.h"

u8 SPI_FLASH_BUF[4096];
u8 spiFlashTest1[32];
u8 testbuf[65];
u8 fac_id, dev_id;
u16 flashIndex;


/*
 ***************************************************************
 * 名称: SPI_Flash_ReadByte 
 * 功能: SPI 读字节
 * 输入: NULL
 * 输出: NULL
 * 返回: 读到的字节 
 * 描述: NULL
 ***************************************************************
 */
u8 SPI_Flash_ReadByte(void)
{
	return Rec_1_Char();
}


/*
 ***************************************************************
 * 名称: SPI_Flash_SendByte 
 * 功能: SPI 写字节
 * 输入: 1. byte：要写的数据
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
void SPI_Flash_SendByte(u8 byte)
{	
	Send_1_Char(byte);
}


/*
 ***************************************************************
 * 名称: SST25_Init 
 * 功能: SST25初始化
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
void SST25_Init(void)
{
	MYSPI_Init();
	
	NotSelect_Flash();	
}


/*
 ***************************************************************
 * 名称: Read_CMD 
 * 功能: 读SST25的状态寄存器
 * 输入: NULL
 * 输出: NULL
 * 返回: 状态寄存器的值
 * 描述: NULL
 ***************************************************************
 */
u8 Read_CMD(void)
{
	u8 busy;
	
	Select_Flash();
	
	SPI_Flash_SendByte(0x05);
	busy = SPI_Flash_ReadByte();
	
	NotSelect_Flash();

	return (busy);
}


/*
 ***************************************************************
 * 名称: FlashReadID 
 * 功能: 读SST25的ID
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
void FlashReadID(void)
{
	Select_Flash();	
	
	SPI_Flash_SendByte(0x90);
	SPI_Flash_SendByte(0x00);
	SPI_Flash_SendByte(0x00);
	SPI_Flash_SendByte(0x00);
	fac_id = SPI_Flash_ReadByte();	// BFH: 工程码SST
	dev_id = SPI_Flash_ReadByte();	// 41H: 器件型号SST25VF016B     
	
	NotSelect_Flash();	
}


/*
 ***************************************************************
 * 名称: Busy_Test 
 * 功能: 检测SST25忙标记
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
void Busy_Test(void)
{
	u8 a = 1;
	
	while ((a&0x01) == 1) 
		a = Read_CMD();	
}


/*
 ***************************************************************
 * 名称: Write_EN 
 * 功能: 让SST25可写
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
void Write_EN(void)
{
	Select_Flash();				// SPI CS 使能
	SPI_Flash_SendByte(0x06);	// 写使能指令
	NotSelect_Flash();			// SPI CS 禁用
}


/*
 ***************************************************************
 * 名称: Write_EN 
 * 功能: 让SST25不可写
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
void Write_DIS(void)
{
	Select_Flash();  			// SPI CS 使能
	SPI_Flash_SendByte(0x04);	// 写禁止指令
	NotSelect_Flash();			// SPI CS 禁用
	Busy_Test();				// 忙检测
}	


/*
 ***************************************************************
 * 名称: Write_CMD 
 * 功能: 写SST25状态寄存器
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
void Write_CMD(void)
{	
	Select_Flash();  			// SPI CS 使能
	SPI_Flash_SendByte(0x50);
	NotSelect_Flash(); 			// SPI CS 禁用
	
	Select_Flash();				// SPI CS 使能
	SPI_Flash_SendByte(0x01);
	SPI_Flash_SendByte(0x00); 
	NotSelect_Flash();			// SPI CS 禁用
	Busy_Test();				// 忙检测
}


/*
 ***************************************************************
 * 名称: SST25_ReadFlash 
 * 功能: 从SST25读数据
 * 输入: 1. addr: 从SST25读数据的首地址
 *       2. length: 要读的长度
 * 输出: 1. *readbuff：要读的数据
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
void SST25_ReadFlash(u32 addr, u8 *readbuff, u16 length)
{
	u32 i = 0; 
	
	Select_Flash();
	
	SPI_Flash_SendByte(0x0b);
	SPI_Flash_SendByte((addr&0xffffff)>>16);
	SPI_Flash_SendByte((addr&0xffff)>>8);
	SPI_Flash_SendByte(addr&0xff);
	SPI_Flash_SendByte(0);
	
	while (i < length)
	{	
		readbuff[i] = SPI_Flash_ReadByte();
		i++;
	}
	
	NotSelect_Flash();	 	
}


/*
 ***************************************************************
 * 名称: Page_Clr 
 * 功能: 1. a1: 要擦写的地址
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
void Page_Clr(u32 a1)
{
	Write_CMD();
	Write_EN();     
	Select_Flash();	  
	SPI_Flash_SendByte(0x20);
	SPI_Flash_SendByte((a1&0xffffff)>>16);        
	SPI_Flash_SendByte((a1&0xffff)>>8);          
	SPI_Flash_SendByte(a1&0xff);            
	NotSelect_Flash();
	Busy_Test();
}


/*
 ***************************************************************
 * 名称: EraseSectors 
 * 功能: 1. sec: 要擦写的第一个扇区
 *       2. num：要擦写的扇区个数
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
void EraseSectors(u16 sec, u16 num)
{
	u16 i;
	
	for (i = 0; i < num; i++)
	{
		Page_Clr((sec+i) << 12);
	}
}


/*
 ***************************************************************
 * 名称: SST25_WriteFlash 
 * 功能: 1. addr：要写的首地址
 *       2. *writebuff：要写的数据
 *       3. length：要写的数据长度
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: 写之前会擦除整个页
 ***************************************************************
 */
void SST25_WriteFlash(u32 addr, u8 *writebuff, u16 length)
{
	u32 i = 0, a2;
	
 	Page_Clr(addr);	// 删除页		  
	
	Write_CMD();
	Write_EN();	
	Select_Flash();    
	SPI_Flash_SendByte(0xad);
	SPI_Flash_SendByte((addr&0xffffff)>>16);
	SPI_Flash_SendByte((addr&0xffff)>>8);
	SPI_Flash_SendByte(addr&0xff);
	SPI_Flash_SendByte(writebuff[0]);
	SPI_Flash_SendByte(writebuff[1]);
	NotSelect_Flash();
	i = 2;
	
	while (i < length)
	{
		a2 = 120;
		while (a2 > 0) 
			a2--;

		Select_Flash();
		SPI_Flash_SendByte(0xad);
		SPI_Flash_SendByte(writebuff[i++]);
		SPI_Flash_SendByte(writebuff[i++]);
		NotSelect_Flash();
	}

	a2 = 100;
	while (a2 > 0) 
		a2--;
	
	Write_DIS();	
	Select_Flash();	
	Busy_Test();
}


/*
 ***************************************************************
 * 名称: SST25_WriteFlash2 
 * 功能: 1. addr：要写的首地址
 *       2. *writebuff：要写的数据
 *       3. length：要写的数据长度
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: 写之前不会擦除整个页
 ***************************************************************
 */
void SST25_WriteFlash2(u32 addr, u8 *writebuff, u16 length)
{
	u32 i = 0, a2;
		
	Write_CMD();
	Write_EN();	
	Select_Flash();    
	SPI_Flash_SendByte(0xad);
	SPI_Flash_SendByte((addr&0xffffff)>>16);
	SPI_Flash_SendByte((addr&0xffff)>>8);
	SPI_Flash_SendByte(addr&0xff);
	SPI_Flash_SendByte(writebuff[0]);
	SPI_Flash_SendByte(writebuff[1]);
	NotSelect_Flash();
	i = 2;
	
	while (i < length)
	{
		a2 = 120;
		while (a2 > 0) 
			a2--;
		
		Select_Flash();
		SPI_Flash_SendByte(0xad);
		SPI_Flash_SendByte(writebuff[i++]);
		SPI_Flash_SendByte(writebuff[i++]);
		NotSelect_Flash();
	}

	Write_DIS();	
	
	Select_Flash();	
	
	Busy_Test();
}


/*
 ***************************************************************
 * 名称: SPI_Flash_Write 
 * 功能: 1. WriteAddr：要写的首地址
 *       2. *pBuffer：要写的数据
 *       3. NumByteToWrite：要写的数据长度
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: 写之前会保存当前页其他的数据
 ***************************************************************
 */
void SPI_Flash_Write(u32 WriteAddr, u8* pBuffer, u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
	u16 i;    

	secpos = WriteAddr / 4096;	// 扇区地址 0~511 for w25x16
	secoff = WriteAddr % 4096;	// 在扇区内的偏移
	secremain = 4096 - secoff;	// 扇区剩余空间大小   

	if (NumByteToWrite <= secremain)
		secremain = NumByteToWrite;		// 不大于4096个字节
	
	while (1) 
	{	
		SST25_ReadFlash(secpos*4096, SPI_FLASH_BUF, 4096);	// 读出整个扇区的内容
		
		for (i = 0; i < secremain; i++)	   // 复制
		{
			SPI_FLASH_BUF[i+secoff] = pBuffer[i];	  
		}
		
		SST25_WriteFlash(secpos*4096, SPI_FLASH_BUF, 4096);// 写入整个扇区  
		
		if (NumByteToWrite == secremain)	// 写入结束了
			break;
		else	// 写入未结束
		{
			secpos++;	// 扇区地址增1
			secoff = 0;	// 偏移位置为0 	 

			pBuffer += secremain;  			// 指针偏移
			WriteAddr += secremain;			// 写地址偏移	   
			NumByteToWrite -= secremain;	// 字节数递减
			
			if (NumByteToWrite > 4096)	// 下一个扇区还是写不完
				secremain = 4096;	
			else // 下一个扇区可以写完了
				secremain = NumByteToWrite;			
		}	 
	};	 	 
}


/*
 ***************************************************************
 * 名称: writeInfoToSpiFlash 
 * 功能: 1. addr：均匀擦写的地址（4k的整数倍）
 *       2. *data：要写的数据
 *       3. len：要写的数据长度
 * 输入: NULL
 * 输出: NULL
 * 返回: 0-错误；1-正确
 * 描述: 写之前会保存当前页其他的数据
 ***************************************************************
 */
u8 writeInfoToSpiFlash(u32 addr, u8 *data, u8 len)
{
	u8  index;
	u8  tempdata[32];
	u32 tempAddr;
	
	tempAddr = addr + 63*64;
	SST25_ReadFlash(tempAddr, tempdata, 1);
	if (tempdata[0] == 0xfa)
	{
		Page_Clr(addr);	
	}
	
	for (index = 0; index < 64; index++)
	{
		tempAddr = addr + index*64;
		SST25_ReadFlash(tempAddr, tempdata, 1);
		if (tempdata[0] == 0xff)
		{
			tempdata[0] = 0xfa;
			memcpy(&tempdata[1], data, len);
			SST25_WriteFlash2(addr+index*64, tempdata, len+1);
			return 1;
		}
		else if (tempdata[0] == 0xfa)
		{
			tempdata[0] = 0xf2;
			SST25_WriteFlash2(addr+index*64, tempdata, 1);
			tempdata[0] = 0xfa;
			memcpy(&tempdata[1], data, len);
			SST25_WriteFlash2(addr+(index+1)*64, tempdata, len+1);
			return 1;
		}
	}
	
	return 0;
}


/*
 ***************************************************************
 * 名称: writeInfoToSpiFlash 
 * 功能: 1. addr：均匀擦写的地址（4k的整数倍）
 *       2. len：读到的数据长度
 * 输入: 1. *data：读到的数据
 * 输出: NULL
 * 返回: 0-错误；1-正确
 * 描述: 写之前会保存当前页其他的数据
 ***************************************************************
 */
u8 readInfoFromSpiFlash(u32 addr, u8 *data, u8 len)
{
	u8  index;
	u8  tempdata[32];
	u32 tempAddr;
	
	for (index = 0; index < 64; index++)
	{
		tempAddr = addr + index*64;
		SST25_ReadFlash(tempAddr, tempdata, 1);
		if ((tempdata[0] == 0xff) && (index == 0))	// 之前没写入数据
			return 0;
		if (tempdata[0] == 0xfa)
		{
			SST25_ReadFlash(tempAddr, data, len+1);
			
			return 1;
		}
	}
	
	return 0;
}


/*
 ***************************************************************
 * 名称: testFlash 
 * 功能: SST25测试
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
u8 testFlash(void)
{		
	u8 flag;
	
	for (flashIndex = 0; flashIndex < 3000; flashIndex++)
	{
		SST25_ReadFlash(flashIndex*64, spiFlashTest1, 16);
		writeInfoToSpiFlash(402*4096, spiFlashTest1, 16);
		readInfoFromSpiFlash(402*4096, testbuf, 16);
		
		if (memcmp(spiFlashTest1, &testbuf[1], 16))
		{
			flag = 0;
			SST25_ReadFlash(402*4096, SPI_FLASH_BUF, 4096);
		}
	}
	
	flag = 1;
	
	return flag;
}
