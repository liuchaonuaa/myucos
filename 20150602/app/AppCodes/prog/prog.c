#include "sys.h"
#include "delay.h"
#include "sst25VF016.h"
#include "usart3.h"
#include "prog.h"


/*
 ***************************************************************
 * 名称: progWrite 
 * 功能: 程序更新
 * 输入: 1. index: 更新数据的索引（第几k）
 *       2. datalen：更新数据的长度
 *       3. *data：要更新的数据
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void progWrite(u16 index, u16 datalen, u8 *data)
{
	u32 addr = 0;
	
	addr = index*(1024+8) + PROGRAMADDR;
	
	if (datalen < 1024)
	{
		SPI_Flash_Write(addr, data, 1024+8);
		uart3_putString("OK");
	}
	else
	{
		SPI_Flash_Write(addr, data, 1024+8);
		uart3_putString("Next");			
	}
}
