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
 * ����: TaskInit 
 * ����: ��ʼ��
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void TaskInit(void)
{
    AppInit();			// �������ʼ��
    AppInitSysPrmt();	// ��ʼ������
}


/*
 ***************************************************************
 * ����: start_task 
 * ����: ��������
 * ����: 1. *pdata���������
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr = 0;
	
	pdata = pdata; 		  
	
	TaskInit();
	
	OSStatInit();					// ��ʼ��ͳ������.�������ʱ1��������	
 	
	OS_ENTER_CRITICAL();			// �����ٽ���(�޷����жϴ��)    
 	
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
 	
	OSTaskSuspend(START_TASK_PRIO);	// ������ʼ����.
	
	OS_EXIT_CRITICAL();				// �˳��ٽ���(���Ա��жϴ��)
}	  


/*
 ***************************************************************
 * ����: beep_task 
 * ����: ����������
 * ����: 1. *pdata���������
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
static void beep_task(void *pdata)
{		
	u8 flag = 0;
	
	Beep_On(100);
	Wt588d16_Play(0);	

	Show_StrInLine(1, 0, "���վ�������");
	Show_StrInLine(2, 0, "��ͨ������鵥");
	Show_StrInLine(3, 0, "��������ˮ");
	Show_StrInLine(4, 0, "liuchao");
	
	oled_Show_StrInLine(1, 0, "���վ�������");
	oled_Show_StrInLine(2, 0, "��ͨ������鵥");
	oled_Show_StrInLine(3, 0, "��������ˮ");
	oled_Show_StrInLine(4, 0, "liuchao:����");
	
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
 * ����: uart3_task 
 * ����: ����3����
 * ����: 1. *pdata���������
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
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
 * ����: can1_task 
 * ����: CAN1����
 * ����: 1. *pdata���������
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
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
