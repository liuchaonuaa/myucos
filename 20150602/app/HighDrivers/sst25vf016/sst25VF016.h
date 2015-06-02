#ifndef __SST25VF016_H
#define __SST25VF016_H

#define Select_Flash()  	MYSPI_NSS_0
#define NotSelect_Flash()	MYSPI_NSS_1


#define   CHARGEADDR   	 0x1000
#define   BIKEIDADDR   	 0x2000
#define   TERMINALADDR   0x3000


#define   PROGRAMADDR   0x6000
#define   ZIKUADDR   	0x100000
#define   CHARADDR   	(0x100000 + 4096*78)



void SST25_Init(void);
void FlashReadID(void);
void SST25_ReadFlash(u32 addr, u8 *readbuff, u16 length);
void SST25_WriteFlash(u32 addr, u8 *writebuff, u16 length);
void SST25_WriteFlash2(u32 addr, u8 *writebuff, u16 length);
void SPI_Flash_Write(u32 WriteAddr, u8 *pBuffer, u16 NumByteToWrite);   
void EraseSectors(u16 sec, u16 num);
u8 writeInfoToSpiFlash(u32 addr, u8 *data, u8 len);
u8 readInfoFromSpiFlash(u32 addr, u8 *data, u8 len);

u8 testFlash(void);

#endif
