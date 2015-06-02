#include "beep.h"
#include "delay.h"

/*
 ***************************************************************
 * ����: Beep_DelayMs 
 * ����: ��������ʱ
 * ����: 1. ms �����ʱ��
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
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
 * ����: Beep_Init 
 * ����: ��������ʼ��
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void Beep_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(BEEP_APB2Periph, ENABLE);		// ʹ�ܶ˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;					// BEEP 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		// �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 	// �ٶ�Ϊ50MHz
	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);	 // ���ݲ�����ʼ��GPIOD12

	GPIO_ResetBits(GPIOD, GPIO_Pin_12);		 // ���0���رշ��������
}

void Beep_On(u16 nms)
{	
	BEEP_SetHigh();
	Beep_DelayMs(nms);
	BEEP_SetLow();
}
