#ifndef __OLED_H
#define __OLED_H

/*
 ******************************
 * 1--CS   <-----> PD0
 * 2--RST  <-----> PD15
 * 3--D/C# <-----> PD14
 * 4--SCL  <-----> PD13
 * 5--SDA  <-----> PD10
 * 6--VDD  <-----> 3.3V
 * 7--VSS  <-----> GND
 ******************************
 */
#define OLED_NSS_1  	GPIOD->BSRR = (1<<0)
#define OLED_NSS_0  	GPIOD->BRR = (1<<0)

#define OLED_RST_1  	GPIOD->BSRR = (1<<15)
#define OLED_RST_0  	GPIOD->BRR = (1<<15)

#define OLED_W_Dat  	GPIOD->BSRR = (1<<14)
#define OLED_W_Com  	GPIOD->BRR = (1<<14)

#define OLED_SCK_1  	GPIOD->BSRR = (1<<13)
#define OLED_SCK_0  	GPIOD->BRR = (1<<13)

#define OLED_MOSI_1 	GPIOD->BSRR = (1<<10)
#define OLED_MOSI_0 	GPIOD->BRR = (1<<10)

void oled_init(void);
void oled_disp_clear(void);
void oled_disp_full(void);   
void oled_Show_StrInLine(u8 row, u16 col, u8* str);

#endif
