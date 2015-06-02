#include "CRC.h"


/*
 ***************************************************************
 * ����: CRC_Config 
 * ����: STM32 CRC ����
 * ����: NULL
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void CRC_Config(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
}

/*
 ***************************************************************
 * ����: CRC_GetVal 
 * ����: STM32 CRC ����
 * ����: 1. *pdata: ҪУ�������
 *       2. len: ҪУ������ݳ���
 *       3. CRC_Val�����յ���crcֵ�������õ���crcֵ�Ƚϣ�
 * ���: NULL 
 * ����: 1����ȷ��0������ȷ
 * ����: index_L index_H   data_len_L  data_len_H   data(no more than 1K)    CRC
 ***************************************************************
 */
u8 CRC_GetVal(u32 *pdata, u16 len, u32 CRC_Val) 
{
	u32 temp;
	
	CRC_ResetDR();

	while (len)
	{
		temp = __rev(*pdata);			//��һ��32λreg�з�ת�ֽ�˳��
		CRC->DR = temp;
		*pdata++;
		len--;
	}
	
	if (CRC_Val != CRC->DR)
		return 0;
	
	return 1;
}

