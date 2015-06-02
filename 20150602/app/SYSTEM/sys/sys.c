#include "sys.h"

void NVIC_Configuration(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x10000);   
}
