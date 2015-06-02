#include "led.h"
#include "delay.h"

/*
 ***************************************************************
 * ����: Led_Init 
 * ����: LED
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void Led_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);		// ʹ�ܶ˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;		// BEEP 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			// �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 		// �ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	 	

	GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	GPIO_ResetBits(GPIOC, GPIO_Pin_5);
}

/********************************************************************************************************
* FunctionName   : LEDSwitchCtrl()
* Description    : LED����
* EntryParameter : ledNum - LED��sw - ����(0-�رգ�1-��)
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
