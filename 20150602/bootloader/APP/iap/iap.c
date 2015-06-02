#include "sys.h"
#include "delay.h"
#include "usart3.h"
#include "common.h"
#include "iap.h"
#include "prog.h"
#include "sst25VF016.h"
#include "lcd12864.h"
#include "crc.h"

extern  u8  programIndex;
extern u8 F_programFinish;

void STMFLASH_Write_NoCheck(u32 WriteAddr, u16 *pBuffer, u16 NumToWrite)
{
	u16 i = 0;
	
	for (i = 0; i < NumToWrite; i++)
	{
		FLASH_ProgramHalfWord(WriteAddr, pBuffer[i]);
		WriteAddr += 2;
	}
}


void IAP_WriteSTMFLASH(u16 index, u16 datalen, u8 *data)
{
	u32 addr = 0;
	
	addr = index*1024 + ApplicationAddress;
	
	FLASH_Unlock();//½âËøflash±àÐ´²Á³ý¿ØÖÆÆ÷

	// Å¼ÊýÊ±½«Õâ¸öÉÈÇø²ÁÐ´
	if ((index%2) == 0)
		FLASH_ErasePage(addr);
	
	if (datalen%2 == 1)
		datalen += 1;
	
	if (datalen < 1024)
	{
		STMFLASH_Write_NoCheck(addr, (u16 *)data, datalen/2);
		F_programFinish = 1;
	}
	else if (datalen == 1024)
	{
		STMFLASH_Write_NoCheck(addr, (u16 *)data, 1024/2);		
	}
	
	FLASH_Lock();   //Ëø¶¨flash±àÐ´²Á³ý¿ØÖÆÆ÷		
}

void IAP_WRITE(void)
{
	u16 index;
	u32 addr = 0;
	u32 crc;
	Program_Struct prog;
	u8 progInfo[32];


	while (1)
	{
		for (index = 0; index < 0xff; index++)
		{
			addr = index*(1024+8) + PROGRAMADDR;
			SST25_ReadFlash(addr, (u8*)(&prog), sizeof(Program_Struct));
			
			crc = prog.crc;
			if (!CRC_GetVal((uint32_t *)(&prog), 257, crc))
			{
				Show_Str(4, 0, "ÉÕÐ´³ÌÐòÊ§°Ü");
				return;
			}
			
			IAP_WriteSTMFLASH(prog.index, prog.len, prog.data);
			sprintf(progInfo, "ÉÕÐ´³ÌÐòÖÐ%3d", index*100/programIndex);
			Show_Str(4, 0, progInfo);
			
			if (prog.len < 1024)
			{
				FlashWrite(FLASH_ADR_1, 0xaa);
				Show_Str(4, 0, "ÉÕÐ´Íê³É");

				return;
			}
		}
		
	} 
}
