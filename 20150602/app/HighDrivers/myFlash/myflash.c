#include "myflash.h"
#include "stm32f10x_flash.h"

volatile FLASH_Status FLASHStatus;

/*
 ***************************************************************
 * ����: FlashWrite 
 * ����: stm32 flash ����д��
 * ����: 1. flash_add: Ҫд�ĵ�ַ
 *       2. DATA: Ҫд������
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */  
void FlashWrite(u32 flash_add, u16 DATA)
{
	u8 i;
	u16 Save[10];
	
	/* ����ԭ���ݣ�ʹ֮�������� */ 
	for (i = 0; i < 8; i++)
	{
		Save[i] = FlashRead(2*i + 0x0800Fc00);  //0x0800Fc00 ���ҳ��ʼ��ַ
	}
	
	/* �����ޱ仯����ִ�� */ 
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
	
	/* �ȴ�������� */  
	while (!FLASHStatus) 
		FLASHStatus = FLASH_WaitForLastOperation(FLASH_COMPLETE);	 
}


/*
 ***************************************************************
 * ����: FlashWrite 
 * ����: stm32 flash ���ֶ�
 * ����: 1. flash_add: Ҫд�ĵ�ַ
 * ���: NULL
 * ����: ����������
 * ����: NULL
 ***************************************************************
 */  
u16 FlashRead(u32 flash_add)
{
	return (*(vu16*)(flash_add));
}
