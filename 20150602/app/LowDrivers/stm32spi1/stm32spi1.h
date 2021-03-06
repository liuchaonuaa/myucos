#ifndef __STM32SPI1_H
#define __STM32SPI1_H

#include "sys.h"

void SPI1_Init(void);			 	// 初始化SPI1口										  
void SPI1_SetSpeed(u8 SpeedSet); 	// 设置SPI1速度
u8 SPI1_ReadWriteByte(u8 TxData);	// SPI1读写一个字节

#endif
