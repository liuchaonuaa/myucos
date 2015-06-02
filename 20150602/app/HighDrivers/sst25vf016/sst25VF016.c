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
 * ����: SPI_Flash_ReadByte 
 * ����: SPI ���ֽ�
 * ����: NULL
 * ���: NULL
 * ����: �������ֽ� 
 * ����: NULL
 ***************************************************************
 */
u8 SPI_Flash_ReadByte(void)
{
	return Rec_1_Char();
}


/*
 ***************************************************************
 * ����: SPI_Flash_SendByte 
 * ����: SPI д�ֽ�
 * ����: 1. byte��Ҫд������
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
void SPI_Flash_SendByte(u8 byte)
{	
	Send_1_Char(byte);
}


/*
 ***************************************************************
 * ����: SST25_Init 
 * ����: SST25��ʼ��
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
void SST25_Init(void)
{
	MYSPI_Init();
	
	NotSelect_Flash();	
}


/*
 ***************************************************************
 * ����: Read_CMD 
 * ����: ��SST25��״̬�Ĵ���
 * ����: NULL
 * ���: NULL
 * ����: ״̬�Ĵ�����ֵ
 * ����: NULL
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
 * ����: FlashReadID 
 * ����: ��SST25��ID
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
void FlashReadID(void)
{
	Select_Flash();	
	
	SPI_Flash_SendByte(0x90);
	SPI_Flash_SendByte(0x00);
	SPI_Flash_SendByte(0x00);
	SPI_Flash_SendByte(0x00);
	fac_id = SPI_Flash_ReadByte();	// BFH: ������SST
	dev_id = SPI_Flash_ReadByte();	// 41H: �����ͺ�SST25VF016B     
	
	NotSelect_Flash();	
}


/*
 ***************************************************************
 * ����: Busy_Test 
 * ����: ���SST25æ���
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: NULL
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
 * ����: Write_EN 
 * ����: ��SST25��д
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
void Write_EN(void)
{
	Select_Flash();				// SPI CS ʹ��
	SPI_Flash_SendByte(0x06);	// дʹ��ָ��
	NotSelect_Flash();			// SPI CS ����
}


/*
 ***************************************************************
 * ����: Write_EN 
 * ����: ��SST25����д
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
void Write_DIS(void)
{
	Select_Flash();  			// SPI CS ʹ��
	SPI_Flash_SendByte(0x04);	// д��ָֹ��
	NotSelect_Flash();			// SPI CS ����
	Busy_Test();				// æ���
}	


/*
 ***************************************************************
 * ����: Write_CMD 
 * ����: дSST25״̬�Ĵ���
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
void Write_CMD(void)
{	
	Select_Flash();  			// SPI CS ʹ��
	SPI_Flash_SendByte(0x50);
	NotSelect_Flash(); 			// SPI CS ����
	
	Select_Flash();				// SPI CS ʹ��
	SPI_Flash_SendByte(0x01);
	SPI_Flash_SendByte(0x00); 
	NotSelect_Flash();			// SPI CS ����
	Busy_Test();				// æ���
}


/*
 ***************************************************************
 * ����: SST25_ReadFlash 
 * ����: ��SST25������
 * ����: 1. addr: ��SST25�����ݵ��׵�ַ
 *       2. length: Ҫ���ĳ���
 * ���: 1. *readbuff��Ҫ��������
 * ����: NULL
 * ����: NULL
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
 * ����: Page_Clr 
 * ����: 1. a1: Ҫ��д�ĵ�ַ
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: NULL
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
 * ����: EraseSectors 
 * ����: 1. sec: Ҫ��д�ĵ�һ������
 *       2. num��Ҫ��д����������
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: NULL
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
 * ����: SST25_WriteFlash 
 * ����: 1. addr��Ҫд���׵�ַ
 *       2. *writebuff��Ҫд������
 *       3. length��Ҫд�����ݳ���
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: д֮ǰ���������ҳ
 ***************************************************************
 */
void SST25_WriteFlash(u32 addr, u8 *writebuff, u16 length)
{
	u32 i = 0, a2;
	
 	Page_Clr(addr);	// ɾ��ҳ		  
	
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
 * ����: SST25_WriteFlash2 
 * ����: 1. addr��Ҫд���׵�ַ
 *       2. *writebuff��Ҫд������
 *       3. length��Ҫд�����ݳ���
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: д֮ǰ�����������ҳ
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
 * ����: SPI_Flash_Write 
 * ����: 1. WriteAddr��Ҫд���׵�ַ
 *       2. *pBuffer��Ҫд������
 *       3. NumByteToWrite��Ҫд�����ݳ���
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: д֮ǰ�ᱣ�浱ǰҳ����������
 ***************************************************************
 */
void SPI_Flash_Write(u32 WriteAddr, u8* pBuffer, u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
	u16 i;    

	secpos = WriteAddr / 4096;	// ������ַ 0~511 for w25x16
	secoff = WriteAddr % 4096;	// �������ڵ�ƫ��
	secremain = 4096 - secoff;	// ����ʣ��ռ��С   

	if (NumByteToWrite <= secremain)
		secremain = NumByteToWrite;		// ������4096���ֽ�
	
	while (1) 
	{	
		SST25_ReadFlash(secpos*4096, SPI_FLASH_BUF, 4096);	// ������������������
		
		for (i = 0; i < secremain; i++)	   // ����
		{
			SPI_FLASH_BUF[i+secoff] = pBuffer[i];	  
		}
		
		SST25_WriteFlash(secpos*4096, SPI_FLASH_BUF, 4096);// д����������  
		
		if (NumByteToWrite == secremain)	// д�������
			break;
		else	// д��δ����
		{
			secpos++;	// ������ַ��1
			secoff = 0;	// ƫ��λ��Ϊ0 	 

			pBuffer += secremain;  			// ָ��ƫ��
			WriteAddr += secremain;			// д��ַƫ��	   
			NumByteToWrite -= secremain;	// �ֽ����ݼ�
			
			if (NumByteToWrite > 4096)	// ��һ����������д����
				secremain = 4096;	
			else // ��һ����������д����
				secremain = NumByteToWrite;			
		}	 
	};	 	 
}


/*
 ***************************************************************
 * ����: writeInfoToSpiFlash 
 * ����: 1. addr�����Ȳ�д�ĵ�ַ��4k����������
 *       2. *data��Ҫд������
 *       3. len��Ҫд�����ݳ���
 * ����: NULL
 * ���: NULL
 * ����: 0-����1-��ȷ
 * ����: д֮ǰ�ᱣ�浱ǰҳ����������
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
 * ����: writeInfoToSpiFlash 
 * ����: 1. addr�����Ȳ�д�ĵ�ַ��4k����������
 *       2. len�����������ݳ���
 * ����: 1. *data������������
 * ���: NULL
 * ����: 0-����1-��ȷ
 * ����: д֮ǰ�ᱣ�浱ǰҳ����������
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
		if ((tempdata[0] == 0xff) && (index == 0))	// ֮ǰûд������
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
 * ����: testFlash 
 * ����: SST25����
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: NULL
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
