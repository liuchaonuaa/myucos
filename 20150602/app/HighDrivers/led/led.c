#include "led.h"
#include "delay.h"

/*
 ***************************************************************
 * 名称: Led_Init 
 * 功能: LED
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void Led_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);		// 使能端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;		// BEEP 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			// 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 		// 速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	 	

	GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	GPIO_ResetBits(GPIOC, GPIO_Pin_5);
}

/********************************************************************************************************
* FunctionName   : LEDSwitchCtrl()
* Description    : LED开关
* EntryParameter : ledNum - LED，sw - 开关(0-关闭，1-打开)
* ReturnWRITE    : None
********************************************************************************************************/
void LEDSwitchCtrl(u8 ledNum, u8 sw)
{
    if (ledNum & LEDRED)
	{
		if(sw)
			redLed_on();
		else
			redLed_off();		
	}

    if (ledNum & LEDGREEN)
	{
		if (sw)    
			greenLed_on();
		else
			greenLed_off();	
	}
	
}
