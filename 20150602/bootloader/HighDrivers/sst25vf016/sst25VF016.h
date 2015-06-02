#ifndef __SST25VF016_H
#define __SST25VF016_H

#define Select_Flash()  	MYSPI_NSS_0
#define NotSelect_Flash()	MYSPI_NSS_1

#define   PROGRAMADDR   0x6000
#define   ZIKUADDR   	0x100000
#define   CHARADDR   	(0x100000 + 4096*78)


void SST25_Init(void);
void FlashReadID(void);
void SST25_ReadFlash(u32 addr, u8 *readbuff, u16 length);
void SST25_WriteFlash(u32 addr, u8 *readbuff, u16 length);
void SPI_Flash_Write(u32 WriteAddr, u8* pBuffer, u16 NumByteToWrite);   
void SST25_WriteFlash2(u32 addr, u8 *readbuff, u16 length);

u8 testFlash(void);

#endif
