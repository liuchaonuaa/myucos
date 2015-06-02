#include "lock.h"
#include "delay.h"
#include "sys.h"

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
void Lock_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);		// 使能端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;		// 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			// 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 		// 速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	 

	GPIO_ResetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


/*
 ***************************************************************
 * 名称: Lock_Open 
 * 功能: 打开锁
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
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
 * 名称: Lock_Close 
 * 功能: 关闭锁
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
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
 * 名称: CheckLockState 
 * 功能: 检测锁状态
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
u8 CheckLockState(void)
{
	u8 flag;

	if ((PCin(2) == 0) && (PCin(3) == 1))		// 关闭状态    2qian3hou 0biaoshidangzhu
		flag = 0;		
	else if ((PCin(2) == 1) && (PCin(3) == 0))	// 打开状态
		flag = 1;		

	if (flag == 0) 
		return 0;
	else 
		return 1;
}

