#include "sys.h"
#include "delay.h"
#include "lcd12864.h"
#include "sst25VF016.h"


/*
 ***************************************************************
 * 名称: delay 
 * 功能: 简单延时
 * 输入: 1. nCount: 延时的时间
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
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

/*
 ***************************************************************
 * 名称: LCD_Pin_Init 
 * 功能: lcd12864管脚初始化
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
void LCD_Pin_Init(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);	//PORTD、PORTE时钟使能 

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_11;		 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
}

/*
 ***************************************************************
 * 名称: writecommand 
 * 功能: lcd12864写命令
 * 输入: 1. cmd：命令字节
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
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


/*
 ***************************************************************
 * 名称: writedata 
 * 功能: lcd12864写数据
 * 输入: 1. dat：数据字节
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
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

/*
 ***************************************************************
 * 名称: disp_clear 
 * 功能: lcd12864清屏
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
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

/*
 ***************************************************************
 * 名称: disp_full 
 * 功能: lcd12864全显示
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
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


/*
 ***************************************************************
 * 名称: lcd12864_init 
 * 功能: lcd12864初始化
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: 驱动芯片初始化配置
 ***************************************************************
 */
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
	writecommand(0x3f);		// change contrast value	 
	
	writecommand(0x2F);	
	writecommand(0xB0);	
	writecommand(0xAF);	
	writecommand(0xA6);	
	
	disp_clear();
}


/*
 ***************************************************************
 * 名称: lcd_address 
 * 功能: lcd12864定位
 * 输入: 1. page: 页 0-7
 *       2. column:列 0-127
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
void lcd_address(u16 page, u16 column)
{	
	column = column + 1;
	
	if (page < 4)
		writecommand(0xb0 + page + 4);	/*设置页地址*/ //0-7
	else
		writecommand(0xb0 + page - 4);	/*设置页地址*/ //0-7
	
	writecommand(0x10 + ((column>>4) & 0x0f));	/*设置列地址的高4位*///0-128
	writecommand(column & 0x0f);				/*设置列地址的低4位*/	
}


/*
 ***************************************************************
 * 名称: display_graphic_8x16 
 * 功能: 显示8*16
 * 输入: 1. page: 页 0-7
 *       2. column: 列 0-127
 *       3. ch: 要写的字符
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
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
			writedata(*dp);	 	/*写数据到LCD,每写完一个8位的数据后列地址自动加1*/
			dp++;
		}
	}
}


/*
 ***************************************************************
 * 名称: display_graphic_16x16 
 * 功能: 显示16*16
 * 输入: 1. page: 页 0-7
 *       2. column: 列 0-127
 *       3. *code: 要写的汉字
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
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
			writedata(*dp);	 /*写数据到LCD,每写完一个8位的数据后列地址自动加1*/
			dp++;
		}
	}
}


/*
 ***************************************************************
 * 名称: Show_Str 
 * 功能: 以页显示字符串
 * 输入: 1. page: 页 0-7
 *       2. column: 列 0-127
 *       3. *str: 要写的字符
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
void Show_Str(u8 page, u16 col, u8* str)
{					
	u8 bHz = 0;			// 字符或者中文  	    				    				  	  
	
	while (*str != 0)	// 数据未结束
	{ 
		if (!bHz)
		{
			if (*str > 0x80)	// 中文 
				bHz = 1;
			else	// 字符
			{      				
				display_graphic_8x16(page, col, *str);//有效部分写入 
				str++; 
				col += 8; // 字符,为全字的一半 
			}
		}
		else	// 中文 
		{     
			bHz = 0;	// 有汉字库     						     
			display_graphic_16x16(page, col, str); //显示这个汉字,空心显示 
			str += 2; 
			col += 16;	// 下一个汉字偏移	    
		}						 
	}   
}  			 		

/*
 ***************************************************************
 * 名称: Show_StrInLine 
 * 功能: 以行显示字符串
 * 输入: 1. row: 行 1-4
 *       2. column: 列 0-127
 *       3. *str: 要写的字符
 * 输出: NULL
 * 返回: NULL
 * 描述: NULL
 ***************************************************************
 */
void Show_StrInLine(u8 row, u16 col, u8* str)
{
	if ((row>0) && (row<5) && (col < 120))
		Show_Str((row-1) * 2, col, str);
}

