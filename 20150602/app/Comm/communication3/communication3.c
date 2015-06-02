/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "sys.h"
#include "usart3.h"
#include "communication3.h"
#include "delay.h"

#include "wt588d16.h"
#include "myflash.h"

#include "CRC.h"
#include "prog.h"
#include "ziku.h"


static Comm3_Rx_Struct	*comm3_Rx;

static	u16 *comm3_lenRx;

static void comm3_zikuRequest(void);		
static void comm3_zikuUpdate(void);		
static void comm3_iapRequest(void);	
static void comm3_iapUpdate(void);	
static void comm3_voiceUpdate(void);		


static Comm3_Control_Struct comm3_Control[] = {
	{ IDZIKUREQUEST,  comm3_zikuRequest },
	{ IDZIKUUPDATE,   comm3_zikuUpdate  },
	{ IDIAPREQUEST,   comm3_iapRequest  },
	{ IDIAPUPDATE,    comm3_iapUpdate   },
	{ IDVOICEUPDATE,  comm3_voiceUpdate },
};


#define COMM3_MAX_FUNC   (sizeof(comm3_Control) / sizeof(comm3_Control[0]))

Comm3_Rx_ZikuUpdate      R_zikuUpdateInfo;
Comm3_Rx_IapRequest      R_iapRequestInfo;
Comm3_Rx_ProgramUpdate   R_programUpdateInfo;
Comm3_Rx_VoiceUpdate     R_voiceUpdateInfo;


/*
 ***************************************************************
 * 名称: comm3_zikuRequest 
 * 功能: 字库申请更新
 * 输入: NULL
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
static void comm3_zikuRequest(void)
{
	eraseZikuFlash();
	uart3_putString("Erase");
}


/*
 ***************************************************************
 * 名称: comm3_zikuUpdate 
 * 功能: 字库更新
 * 输入: NULL
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
static void comm3_zikuUpdate(void)
{
	u32 crc;
	
	memcpy(&R_zikuUpdateInfo, &(comm3_Rx->Union.r_zikuUpdateInfo), sizeof(Comm3_Rx_ZikuUpdate));

	crc = R_zikuUpdateInfo.crc;
	
	if (!CRC_GetVal((uint32_t *)(&R_zikuUpdateInfo), 257, crc))
		uart3_putString("Again");
	else
		ziku_Write(R_zikuUpdateInfo.index, R_zikuUpdateInfo.len, R_zikuUpdateInfo.data);
}


/*
 ***************************************************************
 * 名称: comm3_iapRequest 
 * 功能: iap请求
 * 输入: NULL
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
static void comm3_iapRequest(void)
{
	memcpy(&R_iapRequestInfo, &(comm3_Rx->Union.r_iapRequestInfo), sizeof(Comm3_Rx_IapRequest));
	FlashWrite(FLASH_ADR_1, 0x55);
	FlashWrite(FLASH_ADR_2, R_iapRequestInfo.totalNum);
	NVIC_SystemReset();
}


/*
 ***************************************************************
 * 名称: comm3_iapUpdate 
 * 功能: iap更新
 * 输入: NULL
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
static void comm3_iapUpdate(void)
{
	u32 crc;
	
	memcpy(&R_programUpdateInfo, &(comm3_Rx->Union.r_programUpdateInfo), sizeof(Comm3_Rx_ProgramUpdate));

	crc = R_programUpdateInfo.crc;
	
	if (!CRC_GetVal((uint32_t *)(&R_programUpdateInfo), 257, crc))
		uart3_putString("Again");
	else
		progWrite(R_programUpdateInfo.index, R_programUpdateInfo.len, (u8*)&(R_programUpdateInfo));
}


/*
 ***************************************************************
 * 名称: comm3_voiceUpdate 
 * 功能: 语音更新
 * 输入: NULL
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
static void comm3_voiceUpdate(void)
{
	u32 crc;
	
	memcpy(&R_voiceUpdateInfo, &(comm3_Rx->Union.r_voiceUpdateInfo), sizeof(Comm3_Rx_VoiceUpdate));

	crc = R_voiceUpdateInfo.crc;
	
	if (voiceFlashEraseFlag == 0)
	{
		VoiceUpdate_Pin_Init();
		delay_ms(1000);
		voiceFlashErase();
		voiceFlashEraseFlag = 1;
	}	
	
	if (!CRC_GetVal((uint32_t *)(&R_voiceUpdateInfo), 257, crc))
		uart3_putString("Again");
	else	
		voiceWrite(R_voiceUpdateInfo.index, R_voiceUpdateInfo.len, R_voiceUpdateInfo.data);
}


/*
 ***************************************************************
 * 名称: comm3_FunctionHandle 
 * 功能: 通讯命令处理函数
 * 输入: NULL
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
static void comm3_FunctionHandle(void)
{
	u8 i;
	
	for (i = 0; i < COMM3_MAX_FUNC; i ++)
	{
		/* 寻找功能代码相等的处理函数                  */
		if (comm3_Rx->functionNum == comm3_Control[i].functionId)
		{
			comm3_Control[i].Function();
			
			break;
		}
	}
}


/*
 ***************************************************************
 * 名称: comm3_Analyse 
 * 功能: 通讯协议解析服务函数
 * 输入: 1. *rxBuff：接收的数据
 *       2. *rxLength: 接收数据的长度
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void comm3_Analyse(void *rxBuff, u16 *rxLength)
{	
	comm3_Rx = rxBuff;

	comm3_lenRx = rxLength;
	
	if (*comm3_lenRx >= 4)
	{
		comm3_FunctionHandle();
	}
}

/* [] END OF FILE */
