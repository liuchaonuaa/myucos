#include "myflash.h"
#include "stm32f10x_flash.h"

volatile FLASH_Status FlashStatus;

/**
  **************************************************************************************
  * 函数名称: FlashWrite
  * 功能描述: flash 单字写入
  * 参    数: None
  * 返 回 值: None
  **************************************************************************************
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

	FlashStatus = FLASH_WaitForLastOperation(FLASH_COMPLETE);
	
	/* 等待操作完成 */  
	while (!FlashStatus) 
		FlashStatus = FLASH_WaitForLastOperation(FLASH_COMPLETE);	 
}

/**
  **************************************************************************************
  * 函数名称: FlashRead
  * 功能描述: flash 单字读出
  * 参    数: None
  * 返 回 值: None
  **************************************************************************************
  */
u16 FlashRead(u32 flash_add)
{
	return (*(vu16*)(flash_add));
}
