#include "myflash.h"
#include "stm32f10x_flash.h"

volatile FLASH_Status FLASHStatus;

/*
 ***************************************************************
 * 名称: FlashWrite 
 * 功能: stm32 flash 单字写入
 * 输入: 1. flash_add: 要写的地址
 *       2. DATA: 要写的数据
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */  
void FlashWrite(u32 flash_add, u16 DATA)
{
	u8 i;
	u16 Save[10];
	
	/* 保存原数据，使之不被擦除 */ 
	for (i = 0; i < 8; i++)
	{
		Save[i] = FlashRead(2*i + 0x0800Fc00);  //0x0800Fc00 最后页开始地址
	}
	
	/* 数据无变化，不执行 */ 
	if (DATA != FlashRead(flash_add))
	{	
		FLASH_Unlock();
		FLASH_ErasePage(flash_add);
		i =  (flash_add - 0x0800Fc00) / 2;
		Save[i] = DATA;
		for (i = 0; i < 8; i++)
		{
			FLASH_ProgramHalfWord((2*i + 0x0800Fc00), Save[i]);
		}
		FLASH_Lock();
	}

	FLASHStatus = FLASH_WaitForLastOperation(FLASH_COMPLETE);
	
	/* 等待操作完成 */  
	while (!FLASHStatus) 
		FLASHStatus = FLASH_WaitForLastOperation(FLASH_COMPLETE);	 
}


/*
 ***************************************************************
 * 名称: FlashWrite 
 * 功能: stm32 flash 单字读
 * 输入: 1. flash_add: 要写的地址
 * 输出: NULL
 * 返回: 读到的数据
 * 描述: NULL
 ***************************************************************
 */  
u16 FlashRead(u32 flash_add)
{
	return (*(vu16*)(flash_add));
}
