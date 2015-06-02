#include "beep.h"
#include "delay.h"

/*
 ***************************************************************
 * 名称: Beep_DelayMs 
 * 功能: 蜂鸣器延时
 * 输入: 1. ms 输入的时间
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void Beep_DelayMs(u16 ms)
{
    u16 i;

	while (ms--)
	{
	    for (i = 12000; i > 0; i--)	
		{
		    ;
		}
	}
}

/*
 ***************************************************************
 * 名称: Beep_Init 
 * 功能: 蜂鸣器初始化
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void Beep_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(BEEP_APB2Periph, ENABLE);		// 使能端口时钟
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;					// BEEP 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		// 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 	// 速度为50MHz
	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);	 // 根据参数初始化GPIOD12

	GPIO_ResetBits(GPIOD, GPIO_Pin_12);		 // 输出0，关闭蜂鸣器输出
}

void Beep_On(u16 nms)
{	
	BEEP_SetHigh();
	Beep_DelayMs(nms);
	BEEP_SetLow();
}
