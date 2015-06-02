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
#ifndef _COMMUNICATION3_H_
#define	_COMMUNICATION3_H_

#define  UART3_RX_BUFLEN    1060
#define  UART3_TX_BUFLEN    1060 

#define  IDZIKUREQUEST      0xA0
#define  IDZIKUUPDATE       0xA1

#define  IDIAPREQUEST       0xB0
#define  IDIAPUPDATE        0xB4

#define  IDVOICEUPDATE      0xC0


typedef struct {
	u16   len;
	u16   count;
	u8    buf[UART3_RX_BUFLEN];
} UART3_RX;

typedef struct {
	u16   len;
	u16   count;
	u8    buf[UART3_TX_BUFLEN];
} UART3_TX;


/*----------------- �ֿ�����(0xA0) -----------------*/
__packed typedef struct {
	u8   reserved[6];
	u32  crc;
} Comm3_Rx_ZikuRequest;


/*----------------- �ֿ���д(0xA1) -----------------*/
__packed typedef struct {
	u16  index;			// �ڼ�֡
	u16  len;			// ��һ֡���ݵĳ���
	u8   data[1024];
	u32  crc;
} Comm3_Rx_ZikuUpdate;


/*----------------- iap����(0xB0) -----------------*/
__packed typedef struct {
	u8   totalNum;
	u8   reserved;
	u32  crc;
} Comm3_Rx_IapRequest;

/*----------------- ����(0xB4) -----------------*/
__packed typedef struct {
	u16  index;			// �ڼ�֡
	u16  len;			// ��һ֡���ݵĳ���
	u8   data[1024];
	u32  crc;
} Comm3_Rx_ProgramUpdate;


/*----------------- ����(0xC0) -----------------*/
__packed typedef struct {
	u16  index;			// �ڼ�֡
	u16  len;			// ��һ֡���ݵĳ���
	u8   data[1024];
	u32  crc;
} Comm3_Rx_VoiceUpdate;


/*------------------- ���յ����� ----------------*/
typedef struct {
	u8 flag;		    /* �豸����            */
	u8 functionNum;		/* ���ܺ�              */
	
	union
	{
		u8		buf[1060];
		Comm3_Rx_ZikuRequest    r_zikuRequestInfo;
		Comm3_Rx_ZikuUpdate     r_zikuUpdateInfo;
		Comm3_Rx_IapRequest     r_iapRequestInfo;
		Comm3_Rx_ProgramUpdate  r_programUpdateInfo;
		Comm3_Rx_VoiceUpdate    r_voiceUpdateInfo;
	}Union;
} Comm3_Rx_Struct;


/*------------------- ���͵����� ----------------*/
typedef struct {
	u8 flag;		    /* �豸����            */
	u8 functionNum;		/* ���ܺ�              */
	
	union
	{
		u8		buf[1060];
	} Union;
} Comm3_Tx_Struct;


/*------------------- ���ڿ��ƽṹ�� ----------------*/
typedef struct {
	u8 functionId;
	void (*Function)(void);
} Comm3_Control_Struct;

void comm3_Analyse(void *rxBuff, u16 *rxLength);

#endif
/* [] END OF FILE */
