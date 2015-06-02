#ifndef __LCD12864_H
#define __LCD12864_H

/*
 ******************************
 * 1--CS   <-----> PE14
 * 2--RST  <-----> PE13
 * 3--A0   <-----> PE12
 * 4--SCL  <-----> PD9
 * 5--SDA  <-----> PE11
 * 6--VDD  <-----> 3.3V
 * 7--VSS  <-----> GND
 * 8--VO   <-----> NC
 * 9--XVO  <-----> NC
 * 10--VG  <-----> NC
 ******************************
*/

#define LCD_NSS_1  GPIOE->BSRR = (1<<14)
#define LCD_NSS_0  GPIOE->BRR = (1<<14)

#define LCD_RST_1  GPIOE->BSRR = (1<<13)
#define LCD_RST_0  GPIOE->BRR = (1<<13)

#define LCD_W_Dat  GPIOE->BSRR = (1<<12)
#define LCD_W_Com  GPIOE->BRR = (1<<12)

#define LCD_SCK_1  GPIOD->BSRR = (1<<9)
#define LCD_SCK_0  GPIOD->BRR = (1<<9)

#define LCD_MOSI_1 GPIOE->BSRR = (1<<11)
#define LCD_MOSI_0 GPIOE->BRR = (1<<11)


void lcd12864_init(void);
void disp_clear(void);   
void disp_full(void);
void dispgraphiCS(void);   
void display_graphic_16x16(uint16_t page,uint16_t column,uint8_t *dp);
void display_graphic_8x16(u16 page, u8 column, u8 ch);
void Show_Str(u8 page, u16 col, u8*str);
void WriteCharToFlash(void);
void Show_StrInLine(u8 row, u16 col, u8* str);

#endif
