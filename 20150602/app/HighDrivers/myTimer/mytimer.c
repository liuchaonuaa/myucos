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
 * ����: Time2_Init 
 * ����: ��ʱ��2��ʼ��
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ��
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
 * ����: TIM2_IRQHandler 
 * ����: ��ʱ��2�жϷ���
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ��
 ***************************************************************
 */
void TIM2_IRQHandler(void)   
{
	OS_CPU_SR cpu_sr = 0;

	OS_ENTER_CRITICAL();			// �����ٽ���(�޷����жϴ��)    

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  
		SFT_Timer2_Irq();
		timer2_Count++;		
	}
	
	OS_EXIT_CRITICAL();				// �˳��ٽ���(���Ա��жϴ��)	
}


/*
 ***************************************************************
 * ����: Time3_Init 
 * ����: ��ʱ��3��ʼ��
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ��
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
 * ����: TIM3_IRQHandler 
 * ����: ��ʱ��3�жϷ���
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ��
 ***************************************************************
 */
void TIM3_IRQHandler(void)   
{
	OS_CPU_SR cpu_sr = 0;

	OS_ENTER_CRITICAL();			// �����ٽ���(�޷����жϴ��)    
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  
		
		timer3_Count++;		
	}
	
	OS_EXIT_CRITICAL();				// �˳��ٽ���(���Ա��жϴ��)	
}


/*
 ***************************************************************
 * ����: timer3_GetCount 
 * ����: ��ʱ��3��ȡ����
 * ����: NULL
 * ���: NULL
 * ����: ���ض�ʱ��3�ļ���ֵ
 * ����: ��
 ***************************************************************
 */
u32 timer3_GetCount(void)
{
	return timer3_Count;
}


/*
 ***************************************************************
 * ����: timer3_ResetCount 
 * ����: ��λ��ʱ��3����
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ��
 ***************************************************************
 */
void timer3_ResetCount(void)
{
	timer3_Count = 0;
}

