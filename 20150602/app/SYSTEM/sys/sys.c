#include "sys.h"

void NVIC_Configuration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x10000);   
}
