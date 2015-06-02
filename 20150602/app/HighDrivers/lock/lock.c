#include "lock.h"
#include "delay.h"
#include "sys.h"

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
void Lock_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);		// ʹ�ܶ˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;		// 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			// �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 		// �ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	 

	GPIO_ResetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


/*
 ***************************************************************
 * ����: Lock_Open 
 * ����: ����
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void Lock_Open(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	GPIO_SetBits(GPIOC, GPIO_Pin_0);
	delay_ms(250);
	GPIO_ResetBits(GPIOC, GPIO_Pin_0);
}


/*
 ***************************************************************
 * ����: Lock_Close 
 * ����: �ر���
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void Lock_Close(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_0);
	GPIO_SetBits(GPIOC, GPIO_Pin_1);
	delay_ms(250);
	GPIO_ResetBits(GPIOC, GPIO_Pin_1);
}


/*
 ***************************************************************
 * ����: CheckLockState 
 * ����: �����״̬
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
u8 CheckLockState(void)
{
	u8 flag;

	if ((PCin(2) == 0) && (PCin(3) == 1))		// �ر�״̬    2qian3hou 0biaoshidangzhu
		flag = 0;		
	else if ((PCin(2) == 1) && (PCin(3) == 0))	// ��״̬
		flag = 1;		

	if (flag == 0) 
		return 0;
	else 
		return 1;
}

