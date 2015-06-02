#include "sys.h"
#include "delay.h"
#include "sst25VF016.h"
#include "usart3.h"
#include "prog.h"

u8 testziku[1024];

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
