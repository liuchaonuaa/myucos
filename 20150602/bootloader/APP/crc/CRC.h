#ifndef __CRC_H
#define __CRC_H

#include "stm32f10x.h"

void CRC_Config(void);
u8   CRC_GetVal(u32 *pdata, u16 len, u32 CRC_Val);

#endif
