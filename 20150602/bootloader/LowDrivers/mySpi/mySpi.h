#ifndef __MYSPI_H
#define __MYSPI_H

/*
 ******************************
 *1--SCK  <-----> PC10
 *7--MISO <-----> PC11 
 *2--MOSI <-----> PC12
 *3--NSS  <-----> PE0
 ******************************
*/

#define MYSPI_SCK_1  	GPIOC->BSRR = (1<<10)
#define MYSPI_SCK_0  	GPIOC->BRR = (1<<10)

#define MYSPI_MISO_1 	GPIOC->BSRR = (1<<11)
#define MYSPI_MISO_0 	GPIOC->BRR = (1<<11)

#define MYSPI_MOSI_1 	GPIOC->BSRR = (1<<12)
#define MYSPI_MOSI_0 	GPIOC->BRR = (1<<12)

#define MYSPI_NSS_1  	GPIOE->BSRR = (1<<0)
#define MYSPI_NSS_0  	GPIOE->BRR = (1<<0)


#define MYSPI_MISO		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)


void MYSPI_Init(void);
void Send_1_Char(unsigned char Data);
unsigned char  Rec_1_Char(void);

#endif
