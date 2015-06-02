#include "sys.h"
#include "delay.h"
#include "lcd12864.h"
#include "sst25VF016.h"


u8 cheng[] = {/*--  ����:  ��  --*/
	/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
// 	0x00, 0x00, 0xF8, 0x48, 0x48, 0x48, 0xC8, 0x08, 
// 	0xFF, 0x08, 0x09, 0x0A, 0xC8, 0x88, 0x08, 0x00,
// 	0x40, 0x30, 0x0F, 0x00, 0x08, 0x50, 0x4F, 0x20, 
// 	0x10, 0x0B, 0x0C, 0x12, 0x21, 0x40, 0xF0, 0x00

	// 0x23000
	0x00, 0x40, 0x44, 0x54, 0x55, 0x57, 0xd6, 0xfc, 
	0x7c, 0x54, 0x56, 0x57, 0x55, 0x54, 0x44, 0x40,
	0x00, 0x20, 0x30, 0x58, 0x4c, 0x4f, 0x4b, 0x7e, 
	0x7e, 0x4a, 0x4a, 0x7e, 0x7e, 0x40, 0x40, 0x40	
};


// (94*(High8bit-0xa0-1)+(Low8bit-0xa0-1))*32


u8 ji1[] = {/*--  ����:  ��  --*/
	/*--  ������12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
	0x00,0x10,0x08,0xFC,0x57,0x54,0x54,0x54,0xFD,0x56,0x54,0x54,0x54,0x00,0x00,0x00,
	0x00,0x44,0x44,0x25,0x25,0x15,0x0D,0xFF,0x05,0x0D,0x15,0x25,0x65,0x25,0x04,0x00
};


#if 0
const unsigned char asc2_1608[95][16] = {	  
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},// 0
    {0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00},//!1
    {0x00,0x10,0x0C,0x06,0x10,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//"2
    {0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00},//#3
    {0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00,0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00},//$4
    {0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00,0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00},//%5
    {0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10},//&6
    {0x10,0x16,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//'7
    {0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00},//(8
    {0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00},//)9
    {0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00},//*10
    {0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00},//+11
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00},//,12
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01},//-13
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00},//.14
    {0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04,0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00},///15
    {0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00},//016
    {0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},//117
    {0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00},//218
    {0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00},//319
    {0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00},//420
    {0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00},//521
    {0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00},//622
    {0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00},//723
    {0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00},//824
    {0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00},//925
    {0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00},//:26
    {0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00},//;27
    {0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00},//<28
    {0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00},//=29
    {0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00},//>30
    {0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00,0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00},//?31
    {0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00},//@32
    {0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20},//A33
    {0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00},//B34
    {0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00},//C35
    {0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00},//D36
    {0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00},//E37
    {0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00},//F38
    {0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00},//G39
    {0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20},//H40
    {0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},//I41
    {0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00},//J42
    {0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00},//K43
    {0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00},//L44
    {0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00},//M45
    {0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00},//N46
    {0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00},//O47
    {0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00},//P48
    {0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00},//Q49
    {0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20},//R50
    {0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00},//S51
    {0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00},//T52
    {0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00},//U53
    {0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00},//V54
    {0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00},//W55
    {0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20},//X56
    {0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00},//Y57
    {0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00},//Z58
    {0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00},//[59
    {0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00},//\60
    {0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00},//]61
    {0x00,0x00,0x04,0x02,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//^62
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80},//_63
    {0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//`64
    {0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20},//a65
    {0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00},//b66
    {0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00},//c67
    {0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20},//d68
    {0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00},//e69
    {0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},//f70
    {0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00},//g71
    {0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20},//h72
    {0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},//i73
    {0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00},//j74
    {0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00},//k75
    {0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},//l76
    {0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F},//m77
    {0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20},//n78
    {0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00},//o79
    {0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00},//p80
    {0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80},//q81
    {0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00},//r82
    {0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00},//s83
    {0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00},//t84
    {0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20},//u85
    {0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00},//v86
    {0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00},//w87
    {0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00},//x88
    {0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00},//y89
    {0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00},//z90
    {0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40},//{91
    {0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00},//|92
    {0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00,0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00},//}93
    {0x00,0x06,0x01,0x01,0x02,0x02,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//~94
};
#endif


void delay(vu32 nCount)
{
	nCount *= 5;
	for (; nCount != 0; nCount--);
}


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
void LCD_Pin_Init(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);	//PORTD��PORTEʱ��ʹ�� 

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_11;		 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
}


void writecommand(u8 cmd) 
{
	u8 i;
		
	LCD_NSS_0;
	LCD_W_Com;
	
	for (i = 0; i < 8; i++)
	{
		if (cmd & 0x80)
			LCD_MOSI_1;
		else
			LCD_MOSI_0;
		
		LCD_SCK_0;
		LCD_SCK_1;
		
		cmd = cmd << 1;
	}
	
	LCD_NSS_1;
}


void writedata(u8 dat) 
{
	u8 i;
		
	LCD_NSS_0;
	LCD_W_Dat;
	
	for (i = 0; i < 8; i++)
	{
		if (dat & 0x80)
			LCD_MOSI_1;
		else
			LCD_MOSI_0;
		
		LCD_SCK_0;
		LCD_SCK_1;
		
		dat = dat << 1;
	}
	
	LCD_NSS_1;
}


void disp_clear(void)   
{
	u8 i, j;
	
	for (i = 0; i < 8; i++)
	{
		writecommand(0x00);
		writecommand(0xb0 + i);
		writecommand(0x00);
		writecommand(0x10);
		
		for (j = 0; j < 129; j++)
			writedata(0x00);
	}
}


void disp_full(void)
{
	u8 i, j;
	
	for (i = 0; i < 8; i++)
	{
		writecommand(0x00);
		writecommand(0xb0 + i);
		writecommand(0x00);
		writecommand(0x10);
		
		for (j = 0; j < 128; j++)
			writedata(0xf0);
	}
}


void lcd12864_init(void)
{	
	LCD_Pin_Init();
	
	LCD_RST_1;
	delay(30);
	LCD_RST_0;
	delay(30);
	LCD_RST_1;
	delay(100);

	writecommand(0xE2);
	delay(10);
	writecommand(0xA2);	
	writecommand(0xA0);	
	writecommand(0xC8);	
	writecommand(0xF8);
	writecommand(0x00);
	writecommand(0x23);	
	
	// contrast adjust
	writecommand(0x81);	
	writecommand(0x3f);		// change	
	
	writecommand(0x2F);	
	writecommand(0xB0);	
	writecommand(0xAF);	
	writecommand(0xA6);	
	
	disp_clear();
}


void lcd_address(u16 page, u16 column)
{	
	column = column + 1;
	
	if (page < 4)
		writecommand(0xb0 + page + 4);	/*����ҳ��ַ*/ //0-7
	else
		writecommand(0xb0 + page - 4);	/*����ҳ��ַ*/ //0-7
	
	writecommand(0x10 + ((column>>4) & 0x0f));	/*�����е�ַ�ĸ�4λ*///0-128
	writecommand(column & 0x0f);				/*�����е�ַ�ĵ�4λ*/	
}


/*��ʾ8x16����ͼ��ASCII, ��8x16����������ַ�������ͼ��*/
void display_graphic_8x16(u16 page, u8 column, u8 ch)
{
	u8 i, j;
	u8 CharCode[16];
	u8 *dp;
	
	SST25_ReadFlash(CHARADDR+(ch-' ')*16, CharCode, 16);
	dp = CharCode;
		
	for (j = 0; j < 2; j++)
	{
		lcd_address(page+j, column);
		
		for (i = 0; i < 8; i++)
		{	
			writedata(*dp);	 	/*д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1*/
			dp++;
		}
	}
}


/*��ʾ16x16����ͼ�񡢺��֡���Ƨ�ֻ�16x16���������ͼ��*/
void display_graphic_16x16(u16 page, u16 column, u8 *code)
{
	u16 i, j;
	u8 HanziCode[32];
	u8 *dp;
	u32 loff;
	
	loff = (94*(code[0]-0xa0-1)+(code[1]-0xa0-1))*32 + ZIKUADDR;
		
	SST25_ReadFlash(loff, HanziCode, 32);
	dp = HanziCode;
	
	for (j = 0; j < 2; j++)
	{
		lcd_address(page+j, column);
		
		for (i = 0; i < 16; i++)
		{	
			writedata(*dp);	 /*д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1*/
			dp++;
		}
	}
}


void Show_Str(u8 page, u16 col, u8* str)
{					
	u8 bHz = 0;			// �ַ���������  	    				    				  	  
	
	while (*str != 0)	// ����δ����
	{ 
		if (!bHz)
		{
			if (*str > 0x80)	// ���� 
				bHz = 1;
			else	// �ַ�
			{      				
				display_graphic_8x16(page, col, *str);//��Ч����д�� 
				str++; 
				col += 8; // �ַ�,Ϊȫ�ֵ�һ�� 
			}
		}
		else	// ���� 
		{     
			bHz = 0;	// �к��ֿ�     						     
			display_graphic_16x16(page, col, str); //��ʾ�������,������ʾ 
			str += 2; 
			col += 16;	// ��һ������ƫ��	    
		}						 
	}   
}  			 		


void Show_StrInLine(u8 row, u16 col, u8* str)
{
	if ((row>1) && (row<5) && (col < 120))
		Show_Str((row-1) * 2, col, str);
}


