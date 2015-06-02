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


#define CAN_FUN_SYNC_TIME			1	// ͬ��ʱ��  �㲥
#define CAN_FUN_SYNC_CHARGE		    2	// ͬ���շѱ�׼  �㲥
#define CAN_FUN_QUERY_STATUS		3	// ���ͱ���  �㲥
#define CAN_FUN_UNLOCK				4	// ǿ�ƿ���   ��Ե�
#define CAN_FUN_CALC_EAD			5	// ����CPU���ⲿ��֤����
#define CAN_FUN_REPORT_TRADE_START  6	// �ϱ��⳵/ͣ����ʼ
#define CAN_FUN_REPORT_TRADE_END	7   // �ϱ��⳵/ͣ������
#define CAN_FUN_REPORT_STATUS		8   // �����ϱ�״̬

#define CAN_SUBFUN0					0   // �ӹ���0
#define CAN_SUBFUN1					1   // �ӹ���1
#define CAN_SUBFUN2					2   // �ӹ���2
#define CAN_SUBFUN3					3   // �ӹ���3

#define CAN_SUBFUN_CALC_RENT      	CAN_SUBFUN1   // �⳵��֤
#define CAN_SUBFUN_CALC_RETURN    	CAN_SUBFUN2   // ������֤

#define CAN_ERROR_NONE				0	// ��ȷ
#define CAN_ERROR_UNKNOWN_FUNCTION	1	// δ֪��������
#define CAN_ERROR_BAD_DATA_LENGTH	2	// ��������ݳ���
#define CAN_ERROR_INTERNAL_PSAM		3	// PSAM������ʧ��
#define CAN_ERROR_USERCARD_BLOCKED  4	// �û����������
#define CAN_ERROR_TERM_CLOSED       5   // վ���ѹر�
#define CAN_ERROR_NOCARD            6   // δ�����û���


#define CAN_LEN_SYNC_TIME			7	// ͬ��ʱ��  �㲥
#define CAN_LEN_SYNC_CHARGE		    10	// ͬ���շѱ�׼  �㲥
#define CAN_LEN_QUERY_STATUS		4	// ���ͱ���  �㲥
#define CAN_LEN_UNLOCK				1	// ǿ�ƿ���   ��Ե�
#define CAN_LEN_CALC_EAD			8	// ����CPU���ⲿ��֤����
#define CAN_LEN_REPORT_TRADE_START  1	// �ϱ��⳵/ͣ����ʼ
#define CAN_LEN_REPORT_TRADE_END	1   // �ϱ��⳵/ͣ������
#define CAN_LEN_REPORT_STATUS		1   // �����ϱ�״̬

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


// �û�����Ϣ
__packed typedef struct {
	u8  CardType;		// ������ 0-CPU��  1-M1��  2-�⿨ 		
	u32 CardID;			// CPU�� 4�ֽ�ʮ�����ƿ���   M1�� 4�ֽ�M1���������к�	
	u8  AppID;			// CPU��Ӧ������ 0: "JH.APP.1" ,1: "JH.APP.2"
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
	u8 EAD[8];		// 8�ֽ��ⲿ��֤���ݣ���������Ч���ٻ�ȡ�û���������������ⲿ��֤����Ҫ���¼���
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
	u8   bStatus;		// 1-�г��ɹ�����  2-�޳�  3-��ͣ����ͣ��Ӧ�ã� 4-�쳣  
	u32  BikeID;		
} Can1_Tx_Query_Status_P;



__packed typedef struct {	
	u8 data;
} Can1_Tx_Unlock_P;


__packed typedef struct {
	u32 CardID;
	u8  APPID;
	u8  Rnd[8];		// 8�ֽ���������û���GetChallenge��� 
} Can1_Tx_EAD_RentBike;


__packed typedef struct {
	u32 CardID;
	u8  APPID;
	u8  Rnd[8];		// 8�ֽ���������û���GetChallenge��� 
} Can1_Tx_EAD_ReturnBike;


__packed typedef struct {
	u8   nType;		// ��������  1-�⳵  2-ͣ��      
	Can1_Time_Struct    startTime;	// ��ʼʱ��
	Can1_CardID_Struct  cardID;		// ����
	u32  bikeID;       // �⳵��Ч�����г���� 
} Can1_Tx_Report_Trade_Start;


__packed typedef struct {
	u8   nType;			// ��������  1-�⳵  2-ͣ�� 
	Can1_CardID_Struct cardID;
	u32  startTID;
	Can1_Time_Struct   startTime;
	u8   startParkID;
	Can1_Time_Struct   endTime;
	u32  bikeID;
	u16  dwAmount;		// �շѽ�� ��λ		
} Can1_Tx_Report_Trade_End;


__packed typedef struct {
	u8   bStatus;		// 1-�г��ɹ�����  2-�޳�  3-��ͣ����ͣ��Ӧ�ã� 4-�쳣  
	u32  BikeID;		
} Can1_Tx_Report_Status;


void CANRxDeal(CanRxMsg *pRxMsg);
void CAN1_SendData(u8 fun, u8 subfun, u8 contex, u8 status, u8 *data, u8 len);
void RcvCan1BufClearAll(CAN1_RX *prmt);
void CanCommDeal(void *rxBuff, u16 *rxLength);

#endif
/* [] END OF FILE */
