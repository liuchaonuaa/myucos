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
#ifndef _CAN1COMM_H_
#define	_CAN1COMM_H_

#define  CAN1_RX_BUFLEN    1060

typedef struct {
	u16   len;
	u16   count;
	u8    buf[CAN1_RX_BUFLEN];
} CAN1_RX;

extern CAN1_RX can1_Rx;

typedef struct {
	u16 can_addr;
	u8  bSubFun;	
	u8  bFun;
	u8  bContex;
	u8  bStatus;
	u8  bLast;
	u8  bFirst;
} CAN1_EXID;


#define CAN_FUN_SYNC_TIME			1	// 同步时间  广播
#define CAN_FUN_SYNC_CHARGE		    2	// 同步收费标准  广播
#define CAN_FUN_QUERY_STATUS		3	// 发送报告  广播
#define CAN_FUN_UNLOCK				4	// 强制开锁   点对点
#define CAN_FUN_CALC_EAD			5	// 计算CPU卡外部认证函数
#define CAN_FUN_REPORT_TRADE_START  6	// 上报租车/停车开始
#define CAN_FUN_REPORT_TRADE_END	7   // 上报租车/停车结束
#define CAN_FUN_REPORT_STATUS		8   // 主动上报状态

#define CAN_SUBFUN0					0   // 子功能0
#define CAN_SUBFUN1					1   // 子功能1
#define CAN_SUBFUN2					2   // 子功能2
#define CAN_SUBFUN3					3   // 子功能3

#define CAN_SUBFUN_CALC_RENT      	CAN_SUBFUN1   // 租车认证
#define CAN_SUBFUN_CALC_RETURN    	CAN_SUBFUN2   // 还车认证

#define CAN_ERROR_NONE				0	// 正确
#define CAN_ERROR_UNKNOWN_FUNCTION	1	// 未知请求类型
#define CAN_ERROR_BAD_DATA_LENGTH	2	// 错误的数据长度
#define CAN_ERROR_INTERNAL_PSAM		3	// PSAM卡访问失败
#define CAN_ERROR_USERCARD_BLOCKED  4	// 用户列入黑名单
#define CAN_ERROR_TERM_CLOSED       5   // 站点已关闭
#define CAN_ERROR_NOCARD            6   // 未读到用户卡


#define CAN_LEN_SYNC_TIME			7	// 同步时间  广播
#define CAN_LEN_SYNC_CHARGE		    10	// 同步收费标准  广播
#define CAN_LEN_QUERY_STATUS		4	// 发送报告  广播
#define CAN_LEN_UNLOCK				1	// 强制开锁   点对点
#define CAN_LEN_CALC_EAD			8	// 计算CPU卡外部认证函数
#define CAN_LEN_REPORT_TRADE_START  1	// 上报租车/停车开始
#define CAN_LEN_REPORT_TRADE_END	1   // 上报租车/停车结束
#define CAN_LEN_REPORT_STATUS		1   // 主动上报状态

#define CAN_REV_ACTIVE		    	0x0e	
#define CAN_REV_PASSIVE				0x0f	


__packed typedef struct {	
	u16 mYear;
	u8  mMonth;
	u8  mDay;
	u8  mHour;
	u8  mMin;
	u8  mSec;
} Can1_Time_Struct;


// 用户卡信息
__packed typedef struct {
	u8  CardType;		// 卡类型 0-CPU卡  1-M1卡  2-外卡 		
	u32 CardID;			// CPU卡 4字节十六进制卡号   M1卡 4字节M1卡自身序列号	
	u8  AppID;			// CPU卡应用索引 0: "JH.APP.1" ,1: "JH.APP.2"
} Can1_CardID_Struct;


__packed typedef struct {	
	u8  rate[10];
} Can1_Rx_Sync_Charge;


__packed typedef struct {	
	u8 terminal[4];
} Can1_Rx_Query_Status;


__packed typedef struct {	
	u8 data;
} Can1_Rx_Unlock;


__packed typedef struct {
	u8 EAD[8];		// 8字节外部认证数据，仅当次有效。再获取用户卡随机数或者做外部认证后需要重新计算
} Can1_Rx_Calc_EAD;


__packed typedef struct {
	u8 data;	
} Can1_Rx_Report_Trade_Start;

__packed typedef struct {
	u8 data;		
} Can1_Rx_Report_Trade_End;


__packed typedef struct {
	u8 data;	
} Can1_Rx_Report_Status;


typedef struct {
	union
	{
		u8		buf[CAN1_RX_BUFLEN];
 		Can1_Time_Struct            r_canTime;
		Can1_Rx_Sync_Charge         r_charge;
		Can1_Rx_Query_Status        r_terminal;
		Can1_Rx_Unlock              r_unlock;
		Can1_Rx_Calc_EAD            r_calcEad;
		Can1_Rx_Report_Trade_Start  r_tradeStart;
		Can1_Rx_Report_Trade_End	r_tradeEnd;
		Can1_Rx_Report_Status       r_reportStatus;
	} Union;
} Can1_Rx_Struct;



__packed typedef struct {	
	u8 data;
} Can1_Tx_Sync_Time;


__packed typedef struct {	
	u8  data;
} Can1_Tx_Sync_Charge;


__packed typedef struct {	
	u8 data;
} Can1_Tx_Query_Status_A;


__packed typedef struct {	
	u8   bStatus;		// 1-有车可供出租  2-无车  3-已停车（停车应用） 4-异常  
	u32  BikeID;		
} Can1_Tx_Query_Status_P;



__packed typedef struct {	
	u8 data;
} Can1_Tx_Unlock_P;


__packed typedef struct {
	u32 CardID;
	u8  APPID;
	u8  Rnd[8];		// 8字节随机数，用户卡GetChallenge获得 
} Can1_Tx_EAD_RentBike;


__packed typedef struct {
	u32 CardID;
	u8  APPID;
	u8  Rnd[8];		// 8字节随机数，用户卡GetChallenge获得 
} Can1_Tx_EAD_ReturnBike;


__packed typedef struct {
	u8   nType;		// 交易类型  1-租车  2-停车      
	Can1_Time_Struct    startTime;	// 开始时间
	Can1_CardID_Struct  cardID;		// 卡号
	u32  bikeID;       // 租车有效，自行车编号 
} Can1_Tx_Report_Trade_Start;


__packed typedef struct {
	u8   nType;			// 交易类型  1-租车  2-停车 
	Can1_CardID_Struct cardID;
	u32  startTID;
	Can1_Time_Struct   startTime;
	u8   startParkID;
	Can1_Time_Struct   endTime;
	u32  bikeID;
	u16  dwAmount;		// 收费金额 单位		
} Can1_Tx_Report_Trade_End;


__packed typedef struct {
	u8   bStatus;		// 1-有车可供出租  2-无车  3-已停车（停车应用） 4-异常  
	u32  BikeID;		
} Can1_Tx_Report_Status;


void CANRxDeal(CanRxMsg *pRxMsg);
void CAN1_SendData(u8 fun, u8 subfun, u8 contex, u8 status, u8 *data, u8 len);
void RcvCan1BufClearAll(CAN1_RX *prmt);
void CanCommDeal(void *rxBuff, u16 *rxLength);

#endif
/* [] END OF FILE */
