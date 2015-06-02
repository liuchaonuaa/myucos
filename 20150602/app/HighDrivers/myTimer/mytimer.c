#include "sys.h"
#include "os_cpu.h"
#include "delay.h"
#include "stm32f10x_tim.h"
#include "led.h"
#include "softTimer.h"

static volatile u32 timer2_Count = 0;
static volatile u32 timer3_Count = 0;


/*
 ***************************************************************
 * 名称: Time2_Init 
 * 功能: 定时器2初始化
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
void Time2_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 100; 
	TIM_TimeBaseStructure.TIM_Prescaler = (7200-1); 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM2, ENABLE);  
}


/*
 ***************************************************************
 * 名称: TIM2_IRQHandler 
 * 功能: 定时器2中断服务
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
void TIM2_IRQHandler(void)   
{
	OS_CPU_SR cpu_sr = 0;

	OS_ENTER_CRITICAL();			// 进入临界区(无法被中断打断)    

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  
		SFT_Timer2_Irq();
		timer2_Count++;		
	}
	
	OS_EXIT_CRITICAL();				// 退出临界区(可以被中断打断)	
}


/*
 ***************************************************************
 * 名称: Time3_Init 
 * 功能: 定时器3初始化
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
void Time3_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 100; 
	TIM_TimeBaseStructure.TIM_Prescaler = (7200-1); 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM3, ENABLE);  
}


/*
 ***************************************************************
 * 名称: TIM3_IRQHandler 
 * 功能: 定时器3中断服务
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
void TIM3_IRQHandler(void)   
{
	OS_CPU_SR cpu_sr = 0;

	OS_ENTER_CRITICAL();			// 进入临界区(无法被中断打断)    
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  
		
		timer3_Count++;		
	}
	
	OS_EXIT_CRITICAL();				// 退出临界区(可以被中断打断)	
}


/*
 ***************************************************************
 * 名称: timer3_GetCount 
 * 功能: 定时器3获取计数
 * 输入: NULL
 * 输出: NULL
 * 返回: 返回定时器3的计数值
 * 描述: 无
 ***************************************************************
 */
u32 timer3_GetCount(void)
{
	return timer3_Count;
}


/*
 ***************************************************************
 * 名称: timer3_ResetCount 
 * 功能: 复位定时器3计数
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
void timer3_ResetCount(void)
{
	timer3_Count = 0;
}

