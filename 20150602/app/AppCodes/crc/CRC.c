#include "CRC.h"


/*
 ***************************************************************
 * 名称: CRC_Config 
 * 功能: STM32 CRC 启用
 * 输入: NULL
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void CRC_Config(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
}

/*
 ***************************************************************
 * 名称: CRC_GetVal 
 * 功能: STM32 CRC 启用
 * 输入: 1. *pdata: 要校验的数据
 *       2. len: 要校验的数据长度
 *       3. CRC_Val：接收到的crc值（与计算得到的crc值比较）
 * 输出: NULL 
 * 返回: 1：正确；0：不正确
 * 描述: index_L index_H   data_len_L  data_len_H   data(no more than 1K)    CRC
 ***************************************************************
 */
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

