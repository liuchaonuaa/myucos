#include "sys.h"
#include "delay.h"
#include "lcd12864.h"
#include "sst25VF016.h"


/*
 ***************************************************************
 * ����: delay 
 * ����: ����ʱ
 * ����: 1. nCount: ��ʱ��ʱ��
 * ���: NULL
 * ����: NULL
 * ����: NULL
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
 * ����: LCD_Pin_Init 
 * ����: lcd12864�ܽų�ʼ��
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
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

/*
 ***************************************************************
 * ����: writecommand 
 * ����: lcd12864д����
 * ����: 1. cmd�������ֽ�
 * ���: NULL
 * ����: NULL
 * ����: NULL
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
 * ����: writedata 
 * ����: lcd12864д����
 * ����: 1. dat�������ֽ�
 * ���: NULL
 * ����: NULL
 * ����: NULL
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
 * ����: disp_clear 
 * ����: lcd12864����
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: NULL
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
 * ����: disp_full 
 * ����: lcd12864ȫ��ʾ
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: NULL
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
 * ����: lcd12864_init 
 * ����: lcd12864��ʼ��
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ����оƬ��ʼ������
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
 * ����: lcd_address 
 * ����: lcd12864��λ
 * ����: 1. page: ҳ 0-7
 *       2. column:�� 0-127
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
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


/*
 ***************************************************************
 * ����: display_graphic_8x16 
 * ����: ��ʾ8*16
 * ����: 1. page: ҳ 0-7
 *       2. column: �� 0-127
 *       3. ch: Ҫд���ַ�
 * ���: NULL
 * ����: NULL
 * ����: NULL
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
			writedata(*dp);	 	/*д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1*/
			dp++;
		}
	}
}


/*
 ***************************************************************
 * ����: display_graphic_16x16 
 * ����: ��ʾ16*16
 * ����: 1. page: ҳ 0-7
 *       2. column: �� 0-127
 *       3. *code: Ҫд�ĺ���
 * ���: NULL
 * ����: NULL
 * ����: NULL
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
			writedata(*dp);	 /*д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1*/
			dp++;
		}
	}
}


/*
 ***************************************************************
 * ����: Show_Str 
 * ����: ��ҳ��ʾ�ַ���
 * ����: 1. page: ҳ 0-7
 *       2. column: �� 0-127
 *       3. *str: Ҫд���ַ�
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
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

/*
 ***************************************************************
 * ����: Show_StrInLine 
 * ����: ������ʾ�ַ���
 * ����: 1. row: �� 1-4
 *       2. column: �� 0-127
 *       3. *str: Ҫд���ַ�
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
void Show_StrInLine(u8 row, u16 col, u8* str)
{
	if ((row>0) && (row<5) && (col < 120))
		Show_Str((row-1) * 2, col, str);
}

