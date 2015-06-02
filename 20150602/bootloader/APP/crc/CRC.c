#include "CRC.h"

void CRC_Config(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
}

// index_L index_H   data_len_L  data_len_H   data(no more than 1K)    CRC
u8 CRC_GetVal(u32 *pdata, u16 len, u32 CRC_Val) 
{
	u32 temp;
	
	CRC_ResetDR();

	while (len)
	{
		temp = __rev(*pdata);			//在一个32位reg中翻转字节顺序
		CRC->DR = temp;
		*pdata++;
		len--;
	}
	
	if (CRC_Val != CRC->DR)
		return 0;
	
	return 1;
}

