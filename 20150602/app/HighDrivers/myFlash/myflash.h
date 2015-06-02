#ifndef __MYFLASH_H
#define __MYFLASH_H 

#include "sys.h"

#define FLASH_ADR_7      0x0800fc0e
#define FLASH_ADR_6      0x0800fc0c
#define FLASH_ADR_5      0x0800fc0a
#define FLASH_ADR_4      0x0800fc08
#define FLASH_ADR_3      0x0800fc06
#define FLASH_ADR_2      0x0800fc04
#define FLASH_ADR_1      0x0800fc02
#define FLASH_ADR_0      0x0800fc00

#define FLASH_Page       0x0800fc00

void FlashWrite(u32 flash_add, u16 DATA);
u16  FlashRead(u32 flash_add);

#endif
