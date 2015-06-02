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
 * 名称: App_DelayMs 
 * 功能: 延时
 * 输入: 1. ms  延时的毫秒数
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void App_DelayMs(u16 ms)
{
    u16 i;

	while (ms--)
	{
	    for (i = 12000; i > 0; i--)	 // i = 14 为us
		{
		    ;
		}
	}
}


/*
 ***************************************************************
 * 名称: AppInit 
 * 功能: 初始化内外设
 * 输入: NULL
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void AppInit(void)
{
	Led_Init();
	Beep_Init();			// 蜂鸣器初始化	
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
 * 名称: AppInitSysPrmt 
 * 功能: 初始化系统参数
 * 输入: NULL
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void AppInitSysPrmt(void)
{
	Rcv3BufInit();
}

/********************************************************************************************************
*                              End Of File         
********************************************************************************************************/
