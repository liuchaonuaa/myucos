/******************************************************************************************************** 
*                           
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : Task.c
* Version        : V1.0
* Programmer(s)  : liuchao
* Date           : 2015/5
* Parameters     : STM32
* DebugTools     : Realview MDK V4.72  JLINK V8(SWD)
* Description    :
********************************************************************************************************/
#include "includes.h"
#include "App.h"
#include "Task.h"
#include "led.h"
#include "beep.h"
#include "speaker.h"
#include "wt588d16.h"
#include "sst25VF016.h"
#include "lcd12864.h"
#include "oled.h"
#include "bikeRC522.h"
#include "rtc.h"
#include "ziku.h"
#include "usart3.h"
#include "can1Comm.h"
#include "can1.h"
#include "softTimer.h"

#include "bikeCard.h"
#include "userCard.h"


OS_STK START_TASK_STK[START_STK_SIZE];	   
OS_STK BEEP_TASK_STK[BEEP_STK_SIZE];
OS_STK UART3_TASK_STK[UART3_STK_SIZE];
OS_STK CAN1_TASK_STK[CAN1_STK_SIZE];


OS_EVENT	*Comm3Sem;
OS_EVENT	*Can1Sem;

u8  UART3_Rx[1060];
u16 UART3_RxLen;

u8  CAN1_Rx[1060];
u16 CAN1_RxLen;

u8  cardtype[2];

u8 timeshow[30];	
u8 dateshow[30];



/*
 ***************************************************************
 * 名称: TaskInit 
 * 功能: 初始化
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void TaskInit(void)
{
    AppInit();			// 内外设初始化
    AppInitSysPrmt();	// 初始化参数
}


/*
 ***************************************************************
 * 名称: start_task 
 * 功能: 启动任务
 * 输入: 1. *pdata：任务参数
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr = 0;
	
	pdata = pdata; 		  
	
	TaskInit();
	
	OSStatInit();					// 初始化统计任务.这里会延时1秒钟左右	
 	
	OS_ENTER_CRITICAL();			// 进入临界区(无法被中断打断)    
 	
	OSTaskCreate(uart3_task, 
				(void *)0, 
				(OS_STK*)&UART3_TASK_STK[UART3_STK_SIZE-1], 
				UART3_TASK_PRIO);
	
	OSTaskCreate(can1_task, 
				(void *)0, 
				(OS_STK*)&CAN1_TASK_STK[CAN1_STK_SIZE-1], 
				CAN1_TASK_PRIO);	
 
	OSTaskCreate(beep_task, 
				(void *)0, 
				(OS_STK*)&BEEP_TASK_STK[BEEP_STK_SIZE-1], 
				BEEP_TASK_PRIO);
 	
	OSTaskSuspend(START_TASK_PRIO);	// 挂起起始任务.
	
	OS_EXIT_CRITICAL();				// 退出临界区(可以被中断打断)
}	  


/*
 ***************************************************************
 * 名称: beep_task 
 * 功能: 蜂鸣器任务
 * 输入: 1. *pdata：任务参数
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
static void beep_task(void *pdata)
{		
	u8 flag = 0;
	
	Beep_On(100);
	Wt588d16_Play(0);	

	Show_StrInLine(1, 0, "江苏晶浩有限");
	Show_StrInLine(2, 0, "申通快递详情单");
	Show_StrInLine(3, 0, "延中盐汽水");
	Show_StrInLine(4, 0, "liuchao");
	
	oled_Show_StrInLine(1, 0, "江苏晶浩有限");
	oled_Show_StrInLine(2, 0, "申通快递详情单");
	oled_Show_StrInLine(3, 0, "延中盐汽水");
	oled_Show_StrInLine(4, 0, "liuchao:刘超");
	
	if (flag == 1)
		WriteCharToFlash();
	
	if (flag == 1)
		RTC_Set(2015, 5, 13, 16, 50, 00);
		
	while (1)
	{
		userCardCheck();
		
		sprintf(dateshow, "%4d-%2d-%2d", calendar.w_year, calendar.w_month, calendar.w_date);
		sprintf(timeshow, "%2d:%2d:%2d", calendar.hour, calendar.min, calendar.sec);
		Show_StrInLine(3, 0, dateshow);
		Show_StrInLine(4, 0, timeshow);		

		OSTimeDlyHMSM(0, 0, 0, 300);
	}									 
}


/*
 ***************************************************************
 * 名称: uart3_task 
 * 功能: 串口3任务
 * 输入: 1. *pdata：任务参数
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
static void uart3_task(void *pdata)
{		
	u8 err;
	
	pdata = pdata;
	
	Comm3Sem = OSSemCreate(0);
		
// 	SFT_LED_Start(0);
	SFT_LED_Start(1);
	SFTStartBlkTime();

	while (1)
	{ 		
		OSSemPend(Comm3Sem, 0, &err);
		
		UART3_RxLen = uart3_Rx.len;
		memcpy(UART3_Rx, uart3_Rx.buf, UART3_RxLen);
		comm3_Analyse(&UART3_Rx, &UART3_RxLen);
	}
}


/*
 ***************************************************************
 * 名称: can1_task 
 * 功能: CAN1任务
 * 输入: 1. *pdata：任务参数
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
static void can1_task(void *pdata)
{		
	u8 err;

	pdata = pdata;
	
	Can1Sem = OSSemCreate(0);
			
	while (1)
	{ 		
		OSSemPend(Can1Sem, 0, &err);
		
		CAN1_RxLen = can1_Rx.len;
		memcpy(CAN1_Rx, can1_Rx.buf, CAN1_RxLen);		
		CanCommDeal(&CAN1_Rx, &CAN1_RxLen);
	}
}


/********************************************************************************************************
* End Of File         
********************************************************************************************************/
