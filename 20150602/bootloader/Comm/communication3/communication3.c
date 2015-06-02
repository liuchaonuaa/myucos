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
#include "CRC.h"
#include "prog.h"


static Comm3_Rx_Struct	*comm3_Rx;

static	u16 *comm3_lenRx;

static void comm3_iapRequest(void);	
static void comm3_iapErase(void);		
static void comm3_iapProgram(void);		
static void comm3_iapReset(void);		
static void comm3_iapUpdate(void);		


extern  u8  F_startIAP;
extern  u8  F_programStart;
extern  u8  F_programFinish;
extern  u8  F_erase;
extern  u8  F_reset;


static Comm3_Control_Struct comm3_Control[] = {
	{ IDIAPREQUEST,   comm3_iapRequest },
	{ IDIAPERASE,     comm3_iapErase    },
	{ IDIAPPROGRAM,   comm3_iapProgram  },
	{ IDIAPRESET,     comm3_iapReset    },
	{ IDIAPUPDATE,    comm3_iapUpdate  },
};


#define COMM3_MAX_FUNC   (sizeof(comm3_Control) / sizeof(comm3_Control[0]))

Comm3_Rx_IapRequest      R_iapRequestInfo;
Comm3_Rx_ProgramUpdate   R_programUpdateInfo;


/*
 ****************************************************************************************
 * 函数名称：comm3_iapRequest	
 * 功能描述：
 * 输入参数：无
 * 输出参数：无
 * 返回参数：无
 * 补充说明：
 ****************************************************************************************
 */
static void comm3_iapRequest(void)
{
	memcpy(&R_iapRequestInfo, &(comm3_Rx->Union.r_iapRequestInfo), sizeof(Comm3_Rx_IapRequest));
	F_startIAP = 1;
}


/*
 ****************************************************************************************
 * 函数名称：comm3_iapErase	
 * 功能描述：字库更新
 * 输入参数：无
 * 输出参数：无
 * 返回参数：无
 * 补充说明：
 ****************************************************************************************
 */
static void comm3_iapErase(void)
{
	F_erase = 1;
}


/*
 ****************************************************************************************
 * 函数名称：comm3_iapProgram	
 * 功能描述：
 * 输入参数：无
 * 输出参数：无
 * 返回参数：无
 * 补充说明：
 ****************************************************************************************
 */
static void comm3_iapProgram(void)
{
	F_programStart = 1;
}


/*
 ****************************************************************************************
 * 函数名称：comm3_iapReset	
 * 功能描述：字库更新
 * 输入参数：无
 * 输出参数：无
 * 返回参数：无
 * 补充说明：
 ****************************************************************************************
 */
static void comm3_iapReset(void)
{
	F_reset = 1;
}


/*
 ****************************************************************************************
 * 函数名称：comm3_iapUpdate	
 * 功能描述：
 * 输入参数：无
 * 输出参数：无
 * 返回参数：无
 * 补充说明：
 ****************************************************************************************
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
 ****************************************************************************************
 * 函数名称：comm3_FunctionHandle
 * 功能描述：通讯命令处理函数
 * 输入参数：无
 * 输出参数：无
 * 返回参数：无
 * 补充说明：
 ****************************************************************************************
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
 ****************************************************************************************
 * 函数名称：comm3_Analyse
 * 功能描述：通讯协议解析服务函数
 * 输入参数：无
 * 输出参数：无
 * 返回参数：无
 * 补充说明：
 ****************************************************************************************
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
