#ifndef __WT588D16_H
#define __WT588D16_H

extern u8 voiceFlashEraseFlag;

/*
 *****************************
 * SPK2_RST   <-----> PE1
 * SPK2_MOSI  <-----> PD4
 * SPK2_MISO  <-----> PD5
 * SPK2_CLK   <-----> PD7
 * SPK2_CS    <-----> PD11
 * SPK2_DATA  <-----> PE10
 *****************************
 */
#define SPK2_RST_PORT	GPIOE
#define SPK2_RST_PIN	GPIO_Pin_1

#define SPK2_MOSI_PORT	GPIOD
#define SPK2_MOSI_PIN	GPIO_Pin_4

#define SPK2_MISO_PORT	GPIOD
#define SPK2_MISO_PIN	GPIO_Pin_5

#define SPK2_CLK_PORT	GPIOD
#define SPK2_CLK_PIN	GPIO_Pin_7

#define SPK2_CS_PORT	GPIOD
#define SPK2_CS_PIN		GPIO_Pin_11

#define SPK2_DATA_PORT	GPIOE
#define SPK2_DATA_PIN	GPIO_Pin_10


#define SPK2_RESET_0	GPIO_ResetBits(SPK2_RST_PORT, SPK2_RST_PIN)
#define SPK2_RESET_1	GPIO_SetBits(SPK2_RST_PORT, SPK2_RST_PIN)

#define MYSPI2_SCK_0  	GPIO_ResetBits(SPK2_CLK_PORT, SPK2_CLK_PIN)
#define MYSPI2_SCK_1  	GPIO_SetBits(SPK2_CLK_PORT, SPK2_CLK_PIN)

#define MYSPI2_MISO_0 	GPIO_ResetBits(SPK2_MISO_PORT, SPK2_MISO_PIN)
#define MYSPI2_MISO_1 	GPIO_SetBits(SPK2_MISO_PORT, SPK2_MISO_PIN)

#define MYSPI2_MOSI_0 	GPIO_ResetBits(SPK2_MOSI_PORT, SPK2_MOSI_PIN)
#define MYSPI2_MOSI_1 	GPIO_SetBits(SPK2_MOSI_PORT, SPK2_MOSI_PIN)

#define MYSPI2_NSS_0  	GPIO_ResetBits(SPK2_CS_PORT, SPK2_CS_PIN)
#define MYSPI2_NSS_1  	GPIO_SetBits(SPK2_CS_PORT, SPK2_CS_PIN)

#define MYSPI2_MISO		GPIO_ReadInputDataBit(SPK2_MISO_PORT, SPK2_MISO_PIN)

#define VOICE2_DATA_0  	GPIO_ResetBits(SPK2_DATA_PORT, SPK2_DATA_PIN)
#define VOICE2_DATA_1  	GPIO_SetBits(SPK2_DATA_PORT, SPK2_DATA_PIN)


/*** ”Ô“Ùœ¬‘ÿ√¸¡Ó√Ë ˆ ***/
#define 	WriteEnable 		0x06					/* Write Enable 			*/
#define 	WriteDisable 		0x04                    /* Write Disable			*/
#define 	ReadStatusReg		0x05                    /* Read Status Register 	*/
#define 	ReadStatusRegH		0x35                    /* Read Status Register- 1 	*/
#define 	ReadData			0x03                    /* Read Data				*/
#define 	PageProgram			0x02                    /* Page Program 			*/
#define 	SectorErase			0x20                    /* Sector Erase 			*/
#define 	ChipErase 			0x60                    /* Chip Erase 				*/
#define 	BlockErase 			0xD8                    /* Block Erase(64K) 		*/


/*** ”Ô“Ù–æ∆¨œ‡πÿ≤Œ ˝√Ë ˆ ***/
#define 	PAGE_SIZE   		(256)
#define 	SECTOR_SIZE   		(4096)
#define 	BLOCK_SIZE   		(64*1024)
#define 	PAGE_PER_SECTOR		(SECTOR_SIZE/PAGE_SIZE)
#define 	SECTOR_PER_BLOCK	(BLOCK_SIZE/SECTOR_SIZE)
#define 	PAGE_PER_BLOCK		(BLOCK_SIZE/PAGE_SIZE)
#define 	FLASH_SIZE			(32*BLOCK_SIZE)


void Wt588d16_Pin_Init(void);
void Wt588d16_Play(u8 seg);
void VoiceUpdate_Pin_Init(void);
void VoiceNormal_Pin_Config(void);	
void SPIFLASH_ChipInfo(void);
void voiceWrite(u16 index, u16 datalen, u8 *data);
void SPIFLASH_EraseChip(void);  
void voiceFlashErase(void);
u8   EraseDataProgram(void);
void voiceFlashTest(void);

#endif
