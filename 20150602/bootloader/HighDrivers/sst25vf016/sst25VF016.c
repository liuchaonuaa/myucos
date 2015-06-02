#include "sys.h"
#include "delay.h"
#include "mySpi.h"
#include "sst25VF016.h"

u8 SPI_FLASH_BUF[4096];

u8 spiFlashTest1[1032];


u8 testaa[4096] = "abcdefgh";
u8 testbb[10] = "mnopqrst";

u8 fac_id, dev_id;

u8 SPI_Flash_ReadByte(void)
{
	return Rec_1_Char();
}

void SPI_Flash_SendByte(u8 byte)
{	
	Send_1_Char(byte);
}

void SST25_Init(void)
{
	MYSPI_Init();
	
	NotSelect_Flash();	
}

u8 Read_CMD(void)
{
	u8 busy;
	
	Select_Flash();
	
	SPI_Flash_SendByte(0x05);
	busy = SPI_Flash_ReadByte();
	
	NotSelect_Flash();

	return (busy);
}

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

void Busy_Test(void)
{
	u8 a = 1;
	
	while ((a&0x01) == 1) 
		a = Read_CMD();	
}

void Write_EN(void)
{
	Select_Flash();				// SPI CS ʹ��
	SPI_Flash_SendByte(0x06);	// дʹ��ָ��
	NotSelect_Flash();			// SPI CS ����
}

void Write_DIS(void)
{
	Select_Flash();  			// SPI CS ʹ��
	SPI_Flash_SendByte(0x04);	// д��ָֹ��
	NotSelect_Flash();			// SPI CS ����
	Busy_Test();				// æ���
}	

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


void EraseSectors(u16 sec, u16 num)
{
	u16 i;
	
	for (i = 0; i < num; i++)
	{
		Page_Clr((sec+i) << 12);
	}
}


void SST25_WriteFlash(u32 addr, u8 *readbuff, u16 length)
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
	SPI_Flash_SendByte(readbuff[0]);
	SPI_Flash_SendByte(readbuff[1]);
	NotSelect_Flash();
	i = 2;
	
	while (i < length)
	{
		a2 = 120;
		while (a2 > 0) 
			a2--;

		Select_Flash();
		SPI_Flash_SendByte(0xad);
		SPI_Flash_SendByte(readbuff[i++]);
		SPI_Flash_SendByte(readbuff[i++]);
		NotSelect_Flash();
	}

	a2 = 100;
	while (a2 > 0) 
		a2--;
	
	Write_DIS();	
	Select_Flash();	
	Busy_Test();
}


void SST25_WriteFlash2(u32 addr, u8 *readbuff, u16 length)
{
	u32 i = 0;
		
	Write_CMD();
	Write_EN();	
	Select_Flash();    
	SPI_Flash_SendByte(0xad);
	SPI_Flash_SendByte((addr&0xffffff)>>16);
	SPI_Flash_SendByte((addr&0xffff)>>8);
	SPI_Flash_SendByte(addr&0xff);
	SPI_Flash_SendByte(readbuff[0]);
	SPI_Flash_SendByte(readbuff[1]);
	NotSelect_Flash();
	i = 2;
	
	while (i < length)
	{
		Select_Flash();
		SPI_Flash_SendByte(0xad);
		SPI_Flash_SendByte(readbuff[i++]);
		SPI_Flash_SendByte(readbuff[i++]);
		NotSelect_Flash();
	}

	Write_DIS();	
	
	Select_Flash();	
	
	Busy_Test();
}


// дSPI FLASH  
// ��ָ����ַ��ʼд��ָ�����ȵ�����
// �ú�������������!
// pBuffer:���ݴ洢��
// WriteAddr:��ʼд��ĵ�ַ(24bit)
// NumByteToWrite:Ҫд����ֽ���(���65535)  		   
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


u8 testFlash(void)
{		
// 	EraseSectors(79, 1);
	u8 index;
	
	for (index = 0; index < 255; index++)
		SST25_ReadFlash(4096*80 + index*1032, spiFlashTest1, 1032);

	return 1;
}



