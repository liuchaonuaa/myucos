/******************************************************************************************************** 
*                           
*                                    Address
*  
*                    (c) Copyright 20xx, Company Name, City, State
*                               All Rights Reserved
*
*
* FileName       : App.c
* Version        : V1.0
* Programmer(s)  : liuchao
* Date           : 2015/5
* Parameters     : STM32 
* DebugTools     : Realview MDK V4.72  JLINK V8(SWD)
* Description    :
*
*
********************************************************************************************************/

/********************************************************************************************************
* HeaderFiles                            
********************************************************************************************************/ 
#include "includes.h"
#include "delay.h"

// High Drivers
#include "led.h"
#include "beep.h"
#include "speaker.h"
#include "wt588d16.h"
#include "sst25VF016.h"
#include "lcd12864.h"
#include "oled.h"
#include "CRC.h"
#include "rtc.h"
#include "bikeRC522.h"
#include "userRC522.h"
#include "can1.h"
#include "usart3.h"
#include "myTimer.h"

/*
 ***************************************************************
 * ����: App_DelayMs 
 * ����: ��ʱ
 * ����: 1. ms  ��ʱ�ĺ�����
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void App_DelayMs(u16 ms)
{
    u16 i;

	while (ms--)
	{
	    for (i = 12000; i > 0; i--)	 // i = 14 Ϊus
		{
		    ;
		}
	}
}


/*
 ***************************************************************
 * ����: AppInit 
 * ����: ��ʼ��������
 * ����: NULL
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void AppInit(void)
{
	Led_Init();
	Beep_Init();			// ��������ʼ��	
	Speaker_Pin_Init();
	Wt588d16_Pin_Init();
	SST25_Init();
	lcd12864_init();
	oled_init();
	RTC_Init();
	CRC_Config();
	bikeRC522_InitRc522();
	userRC522_InitRc522();
	can1_init(Baudrate_500K);
	Time2_Init();
	Time3_Init();
}

/*
 ***************************************************************
 * ����: AppInitSysPrmt 
 * ����: ��ʼ��ϵͳ����
 * ����: NULL
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void AppInitSysPrmt(void)
{
	Rcv3BufInit();
}

/********************************************************************************************************
*                              End Of File         
********************************************************************************************************/
