#ifndef __CAN1_H
#define __CAN1_H

#include "stdio.h"	
#include "sys.h" 

#define A0  GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2)
#define A1  GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)
#define A2  GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)
#define A3  GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)
#define A4  GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)
#define A5  GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5)
#define A6  GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6)

typedef struct  {
	u8  SJW;
	u8  BS1;                                     
	u8  BS2;
	u16 PreScale;
} CanBaud;

#define	Baudrate_10K		0
#define	Baudrate_20K		1
#define	Baudrate_30K		2
#define	Baudrate_40K		3
#define	Baudrate_50K		4
#define	Baudrate_60K		5
#define	Baudrate_80K		6
#define	Baudrate_90K		7
#define	Baudrate_100K		8
#define	Baudrate_125K		9
#define	Baudrate_150K		10
#define	Baudrate_200K		11
#define	Baudrate_250K		12
#define	Baudrate_300K		13
#define	Baudrate_400K		14
#define	Baudrate_500K		15
#define	Baudrate_600K		16
#define	Baudrate_800K		17
#define	Baudrate_900K		18
#define	Baudrate_1000K		19

extern CanRxMsg rxMessage;



void can1_init(u32 baud_num);
u8 CAN1_SendOneFrame(CanTxMsg *TxMessage);
u8 Read_CAN_Address(void);

#endif


