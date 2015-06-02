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

#define  IDIAPREQUEST       0xB0
#define  IDIAPERASE     	0xB1
#define  IDIAPPROGRAM		0xB2
#define  IDIAPRESET      	0xB3
#define  IDIAPUPDATE        0xB4


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


/*----------------- iap启动(0xB0) -----------------*/
__packed typedef struct {
	u8   totalNum;
	u8   reserved;
	u32  crc;
} Comm3_Rx_IapRequest;


/*----------------- 擦写(0xB1) -----------------*/
__packed typedef struct {
	u8   reserved[2];
	u32  crc;
} Comm3_Rx_Erase;



/*----------------- 编程(0xB2) -----------------*/
__packed typedef struct {
	u8   reserved[2];
	u32  crc;
} Comm3_Rx_Program;


/*----------------- 复位(0xB3) -----------------*/
__packed typedef struct {
	u8   reserved[2];
	u32  crc;
} Comm3_Rx_Reset;


/*----------------- 下载(0xB4) -----------------*/
__packed typedef struct {
	u16  index;			// 第几帧
	u16  len;			// 这一帧数据的长度
	u8   data[1024];
	u32  crc;
} Comm3_Rx_ProgramUpdate;



/*------------------- 接收的数据 ----------------*/
typedef struct {
	u8 flag;		    /* 设备机号            */
	u8 functionNum;		/* 功能号              */
	
	union
	{
		u8		buf[1060];
		Comm3_Rx_IapRequest     r_iapRequestInfo;
		Comm3_Rx_ProgramUpdate  r_programUpdateInfo;
	}Union;
} Comm3_Rx_Struct;


/*------------------- 发送的数据 ----------------*/
typedef struct {
	u8 flag;		    /* 设备机号            */
	u8 functionNum;		/* 功能号              */
	
	union
	{
		u8		buf[1060];
	} Union;
} Comm3_Tx_Struct;


/*------------------- 串口控制结构体 ----------------*/
typedef struct {
	u8 functionId;
	void (*Function)(void);
} Comm3_Control_Struct;

void comm3_Analyse(void *rxBuff, u16 *rxLength);

#endif
/* [] END OF FILE */
