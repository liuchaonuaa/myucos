#ifndef __STM32SPI2_H
#define __STM32SPI2_H

#include "sys.h"

void SPI2_Init(void);			 	// 初始化SPI1口										  
void SPI2_SetSpeed(u8 SpeedSet); 	// 设置SPI2速度
u8 SPI2_ReadWriteByte(u8 TxData);	// SPI2读写一个字节

#endif
