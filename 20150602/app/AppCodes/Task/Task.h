/********************************************************************************************************    
*                                                                      
*                                     Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                                All Rights Reserved
*
*
* FileName       : Task.h
* Version        : V1.0
* Programmer(s)  : liuchao
* Date           : 2015/5
* Parameters     : STM32 
* DebugTools     : Realview MDK V4.72  JLINK V8(SWD)
* Description    :
*
********************************************************************************************************/

#ifndef  __TASK__H__
#define  __TASK__H__	 

/////////////////////////UCOSII�����ջ����///////////////////////////////////
#define START_TASK_PRIO      			9 
#define UART3_TASK_PRIO       			3 
#define BEEP_TASK_PRIO       			4 
#define CAN1_TASK_PRIO       			5 

#define START_STK_SIZE  				128
#define BEEP_STK_SIZE  					128
#define UART3_STK_SIZE  				128
#define CAN1_STK_SIZE  					128


/********************************************************************************************************
* Global variables                          
********************************************************************************************************/

extern OS_STK START_TASK_STK[START_STK_SIZE];	   


void start_task(void *pdata);	
static void beep_task(void *pdata);
static void uart3_task(void *pdata);
static void can1_task(void *pdata);




/********************************************************************************************************
* Internal function                           
********************************************************************************************************/


/********************************************************************************************************
* Global function                           
********************************************************************************************************/
extern void TaskInit(void);									    //
extern void TaskStart(void* pdata);							    // ��������

/*******************************************************************************************************/
#endif

/********************************************************************************************************
*                              End Of File         
********************************************************************************************************/