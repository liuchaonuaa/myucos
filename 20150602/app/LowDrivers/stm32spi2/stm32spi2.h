#ifndef __STM32SPI2_H
#define __STM32SPI2_H

#include "sys.h"

void SPI2_Init(void);			 	// ��ʼ��SPI1��										  
void SPI2_SetSpeed(u8 SpeedSet); 	// ����SPI2�ٶ�
u8 SPI2_ReadWriteByte(u8 TxData);	// SPI2��дһ���ֽ�

#endif
