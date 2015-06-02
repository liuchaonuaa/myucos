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
#include "ucos_ii.h"
#include "os_cpu.h"

#include "can1.h"
#include "can1Comm.h"

#include "delay.h"
#include "rtc.h"

#include "sst25VF016.h"


CAN1_RX can1_Rx; 	
extern OS_EVENT	*Can1Sem;
CAN1_EXID can1_Exid;

static Can1_Rx_Struct  *can1_Rx_Comm;


Can1_Rx_Calc_EAD CardEad;

/*
 ***************************************************************
 * 名称: RcvCan1BufClearAll 
 * 功能: 清零参
 * 输入: 1. *prmt：参数
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */  
void RcvCan1BufClearAll(CAN1_RX *prmt)
{
    memset(prmt->buf, 0, CAN1_RX_BUFLEN);
	prmt->count = 0;
	prmt->len   = 0;
}


u8 myMin(u8 a, u8 b)
{
	return (a > b) ? b : a; 
}


void CAN1_SendData(u8 fun, u8 subfun, u8 contex, u8 status, u8 *data, u8 len)
{
	CanTxMsg txMsg;
	u8  left = len;
	u8  parkID = Read_CAN_Address();
	u32 exid = ((parkID&0x7f)<<18) | ((fun&0x0f)<<12) | ((subfun&0x03)<<16) | 
				((contex&0x0f)<<8) | ((status&0x3f)<<2);
	u8  dlc;
	
	exid |= (1<<0);
	
	while (1)
	{
		if (left <= 8)
		{
			exid |= (1<<1);
		}
		else
		{
			exid &= ~(1<<1);
		}
		
		dlc = myMin(left, 8);	
		
		txMsg.ExtId = exid;	
		txMsg.IDE   = CAN_Id_Extended;
		txMsg.RTR   = CAN_RTR_Data;
		txMsg.DLC   = dlc;
		memcpy(txMsg.Data, data, dlc);
		CAN1_SendOneFrame(&txMsg);
		
		if ((exid&0x01) == 0x01)
			exid &= ~(1<<0);
		
		if (((exid>>1)&0x01) == 0x01)
			break;
		
		left -= 8;
		data += 8;
	}
}


void CANRxDeal(CanRxMsg *pRxMsg)
{
	static u8 flag = 0;
	
	can1_Exid.can_addr = (pRxMsg->ExtId >> 18) & 0x7ff;
	can1_Exid.bSubFun  = (pRxMsg->ExtId >> 16) & 0x03;	
	can1_Exid.bFun     = (pRxMsg->ExtId >> 12) & 0x0f;
	can1_Exid.bContex  = (pRxMsg->ExtId >> 8) & 0x03;
	can1_Exid.bStatus  = (pRxMsg->ExtId >> 2) & 0x3f;
	can1_Exid.bLast    = (pRxMsg->ExtId >> 1) & 0x01;
	can1_Exid.bFirst   = (pRxMsg->ExtId >> 0) & 0x01;	
	
	if ((can1_Exid.can_addr != Read_CAN_Address()) && (can1_Exid.can_addr != 0x7ff))
	{		
		return;
	}
	
	if ((can1_Exid.bFirst) && (flag == 0))
	{
		RcvCan1BufClearAll(&can1_Rx);
		memcpy(&can1_Rx.buf[can1_Rx.count], pRxMsg->Data, pRxMsg->DLC);
		can1_Rx.count += pRxMsg->DLC;
		flag = 1;
	}
	else
	{
		if (can1_Rx.count > CAN1_RX_BUFLEN)
		{
			RcvCan1BufClearAll(&can1_Rx);
			flag = 0;
		}
		else
		{
			memcpy(&can1_Rx.buf[can1_Rx.count], pRxMsg->Data, pRxMsg->DLC);
			can1_Rx.count += pRxMsg->DLC;
		}
	}
	
	if (can1_Exid.bLast)
	{
		if (flag == 1)
		{
			can1_Rx.len = can1_Rx.count;
			OSSemPost(Can1Sem);
			flag = 0;	
		}
		else
		{
			RcvCan1BufClearAll(&can1_Rx);
		}
	}
	
}


void CanCommDeal(void *rxBuff, u16 *rxLength)
{	
	can1_Rx_Comm = rxBuff;

	switch (can1_Exid.bFun)
	{
		case CAN_FUN_SYNC_TIME:				// 1 
			if (*rxLength == CAN_LEN_SYNC_TIME)
			{
				RTC_Set(can1_Rx_Comm->Union.r_canTime.mYear, can1_Rx_Comm->Union.r_canTime.mMonth, 
						can1_Rx_Comm->Union.r_canTime.mDay, can1_Rx_Comm->Union.r_canTime.mHour, 
						can1_Rx_Comm->Union.r_canTime.mMin, can1_Rx_Comm->Union.r_canTime.mSec);
			}
			break;
		
		case CAN_FUN_SYNC_CHARGE:			// 2 
			if (*rxLength == CAN_LEN_SYNC_CHARGE)
			{
				SST25_WriteFlash(CHARGEADDR, can1_Rx_Comm->Union.r_charge.rate, 10);	
			}
			break;
		
		case CAN_FUN_QUERY_STATUS:			// 3  
			if (*rxLength == CAN_LEN_QUERY_STATUS)
			{
				SST25_WriteFlash(TERMINALADDR, can1_Rx_Comm->Union.r_terminal.terminal, 4);
			}
			break;
		
		case CAN_FUN_UNLOCK:				// 4 
			if (*rxLength == CAN_LEN_UNLOCK)
			{
				if (can1_Exid.bContex == CAN_REV_PASSIVE)
				{
					can1_Exid.bContex = can1_Exid.bContex;
				}			
			}
			break;	

		case CAN_FUN_CALC_EAD:				// 5  
			if (*rxLength == CAN_LEN_CALC_EAD)
			{
				if (can1_Exid.bContex == CAN_REV_ACTIVE)
				{
					if (can1_Exid.bStatus == CAN_ERROR_USERCARD_BLOCKED)
					{
						// 报黑名单错误
					}	
					else
					{
						memcpy(CardEad.EAD, can1_Rx_Comm->Union.r_calcEad.EAD, 8);
					}			
				}		
			}
			break;
		
		case CAN_FUN_REPORT_TRADE_START:	// 6 
			if (*rxLength == CAN_LEN_REPORT_TRADE_START)
			{
				if (can1_Exid.bContex == CAN_REV_ACTIVE)
				{
					can1_Exid.bContex = can1_Exid.bContex;
				}			
			}			
			break;
		
		case CAN_FUN_REPORT_TRADE_END:		// 7  
			if (*rxLength == CAN_LEN_REPORT_TRADE_END)
			{
				if (can1_Exid.bContex == CAN_REV_ACTIVE)
				{
					can1_Exid.bContex = can1_Exid.bContex;
				}			
			}			
			break;
		
		case CAN_FUN_REPORT_STATUS:			// 8  
			if (*rxLength == CAN_LEN_REPORT_STATUS)
			{
				if (can1_Exid.bContex == CAN_REV_ACTIVE)
				{
					can1_Exid.bContex = can1_Exid.bContex;
				}			
			}			
			break;	

		default:
			break;
	}
	
}


void Can1_Send_SyncTime(u8 contex, u8 status, u8 *buf)
{
	CAN1_SendData(CAN_FUN_SYNC_TIME, CAN_SUBFUN0, contex, status, buf, sizeof(Can1_Tx_Sync_Time));
}


void Can1_Send_SyncCharge(u8 contex, u8 status, u8 *buf)
{
	CAN1_SendData(CAN_FUN_SYNC_CHARGE, CAN_SUBFUN0, contex, status, buf, sizeof(Can1_Tx_Sync_Time));
}


void Can1_Send_QueryStatus(u8 contex, u8 status, u8 *buf)
{
	if (contex == CAN_REV_ACTIVE)
		CAN1_SendData(CAN_FUN_QUERY_STATUS, CAN_SUBFUN0, CAN_REV_ACTIVE, status, buf, sizeof(Can1_Tx_Query_Status_A));
	else if (contex == CAN_REV_PASSIVE)	
		CAN1_SendData(CAN_FUN_QUERY_STATUS, CAN_SUBFUN0, CAN_REV_PASSIVE, status, buf, sizeof(Can1_Tx_Query_Status_P));
}


void Can1_Send_Unlock(u8 contex, u8 status, u8 *buf)
{
	CAN1_SendData(CAN_FUN_UNLOCK, CAN_SUBFUN0, CAN_REV_PASSIVE, status, buf, sizeof(Can1_Tx_Unlock_P));
}


void Can1_Send_EAD_RentBike(u8 contex, u8 status, u8 *buf)
{
	CAN1_SendData(CAN_FUN_CALC_EAD, CAN_SUBFUN_CALC_RENT, CAN_REV_ACTIVE, status, buf, sizeof(Can1_Tx_EAD_RentBike));
}


void Can1_Send_EAD_ReturnBike(u8 contex, u8 status, u8 *buf)
{
	CAN1_SendData(CAN_FUN_CALC_EAD, CAN_SUBFUN_CALC_RETURN, CAN_REV_ACTIVE, status, buf, sizeof(Can1_Tx_EAD_ReturnBike));
}


void Can1_Send_ReportTradeStart(u8 contex, u8 status, u8 *buf)
{
	CAN1_SendData(CAN_FUN_REPORT_TRADE_START, CAN_SUBFUN0, CAN_REV_ACTIVE, status, buf, sizeof(Can1_Tx_Report_Trade_Start));
}


void Can1_Send_ReportTradeEnd(u8 contex, u8 status, u8 *buf)
{
	CAN1_SendData(CAN_FUN_REPORT_TRADE_END, CAN_SUBFUN0, CAN_REV_ACTIVE, status, buf, sizeof(Can1_Tx_Report_Trade_End));
}


void Can1_Send_ReportStatus(u8 contex, u8 status, u8 *buf)
{
	CAN1_SendData(CAN_FUN_REPORT_STATUS, CAN_SUBFUN0, CAN_REV_ACTIVE, status, buf, sizeof(Can1_Tx_Report_Status));
}


/* [] END OF FILE */
