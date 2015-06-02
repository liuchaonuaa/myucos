#include "sys.h"
#include "delay.h"
#include "oled.h"
#include "sst25VF016.h"


/*
 ***************************************************************
 * ����: oled_delay 
 * ����: ����ʱ
 * ����: 1. nCount: ��ʱ��ʱ��
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
void oled_delay(vu32 nCount)
{
	nCount *= 5;
	for (; nCount != 0; nCount--);
}


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
/*
 ***************************************************************
 * ����: OLED_Pin_Init 
 * ����: oled�ܽų�ʼ��
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
void OLED_Pin_Init(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	//PORTDʱ��ʹ�� 

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_10 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	OLED_NSS_1;
}


/*
 ***************************************************************
 * ����: oled_writecommand 
 * ����: oledд����
 * ����: 1. cmd�������ֽ�
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
void oled_writecommand(u8 cmd) 
{
	u8 i;
		
	OLED_NSS_0;
	OLED_W_Com;
	
	for (i = 0; i < 8; i++)
	{
		if (cmd & 0x80)
			OLED_MOSI_1;
		else
			OLED_MOSI_0;
		
		OLED_SCK_0;
		OLED_SCK_1;
		
		cmd = cmd << 1;
	}
	
	OLED_NSS_1;
}


/*
 ***************************************************************
 * ����: oled_writedata 
 * ����: oledд����
 * ����: 1. dat�������ֽ�
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
void oled_writedata(u8 dat) 
{
	u8 i;
		
	OLED_NSS_0;
	OLED_W_Dat;
	
	for (i = 0; i < 8; i++)
	{
		if (dat & 0x80)
			OLED_MOSI_1;
		else
			OLED_MOSI_0;
		
		OLED_SCK_0;
		OLED_SCK_1;
		
		dat = dat << 1;
	}
	
	OLED_NSS_1;
}


/*
 ***************************************************************
 * ����: oled_disp_clear 
 * ����: oled����
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
void oled_disp_clear(void)   
{
	u8 i, j;
	
	for (i = 0; i < 8; i++)
	{
		oled_writecommand(0xb0 + i);
		oled_writecommand(0x10);
		oled_writecommand(0x00);
		
		for (j = 0; j < 128; j++)
			oled_writedata(0x00);
	}
}


/*
 ***************************************************************
 * ����: oled_disp_full 
 * ����: oledȫ��ʾ
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
void oled_disp_full(void)
{
	u8 i, j;
	
	for (i = 0; i < 8; i++)
	{
		oled_writecommand(0xb0 + i);
		oled_writecommand(0x10);
		oled_writecommand(0x00);
		
		for (j = 0; j < 128; j++)
			oled_writedata(0xff);
	}
}


/*
 ***************************************************************
 * ����: oled_init 
 * ����: oled��ʼ��
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: SSD1305����оƬ��ʼ������
 ***************************************************************
 */
void oled_init(void)
{	
	OLED_Pin_Init();
	
	OLED_RST_1;
	oled_delay(30);
	OLED_RST_0;
	oled_delay(30);
	OLED_RST_1;
	oled_delay(100);

	oled_writecommand(0xae);
	oled_writecommand(0xa1);	// segment remap

	oled_writecommand(0xda);	// common pads hardware: alternative
	oled_writecommand(0x12);

	oled_writecommand(0xc8);	// common output scan direction:com63~com0

	oled_writecommand(0xa8);	// multiplex ration mode:63
	oled_writecommand(0x3f);

	oled_writecommand(0xd5);	// display divide ratio/osc. freq. mode
	
	oled_writecommand(0x70);	// Osc. Freq:320kHz,DivideRation:1

	oled_writecommand(0x81);	// contrast control 
	oled_writecommand(0x70);	// mode:64

	oled_writecommand(0xd9);	// set pre-charge period
	oled_writecommand(0x22);	// set period 1:1;period 2:15

	oled_writecommand(0x20);	// Set Memory Addressing Mode
	oled_writecommand(0x02);	// page addressing mode

	oled_writecommand(0xdb);	// VCOM deselect level mode
	oled_writecommand(0x3c);	// set Vvcomh=0.83*Vcc
	oled_writecommand(0xad);	// master configuration 
	oled_writecommand(0x8e);	// external VCC supply
	oled_writecommand(0xa4);	// out follows RAM content
	oled_writecommand(0xa6);	// set normal display
	oled_writecommand(0xaf);
	
	oled_disp_clear();
}


/*
 ***************************************************************
 * ����: oled_address 
 * ����: oled��λ
 * ����: 1. page: ҳ 0-7
 *       2. column:�� 0-127
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
void oled_address(u16 page, u16 column)
{	
	column = column + 1;
	
	oled_writecommand(0xb0 + page);	/*����ҳ��ַ*/ //0-7
	oled_writecommand(0x10 + ((column>>4) & 0x0f));	/*�����е�ַ�ĸ�4λ*///0-128
	oled_writecommand(column & 0x0f);				/*�����е�ַ�ĵ�4λ*/	
}


/*
 ***************************************************************
 * ����: oled_display_graphic_8x16 
 * ����: ��ʾ8*16
 * ����: 1. page: ҳ 0-7
 *       2. column: �� 0-127
 *       3. ch: Ҫд���ַ�
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
void oled_display_graphic_8x16(u16 page, u8 column, u8 ch)
{
	u8 i, j;
	u8 CharCode[16];
	u8 *dp;
	
	SST25_ReadFlash(CHARADDR+(ch-' ')*16, CharCode, 16);
	dp = CharCode;
		
	for (j = 0; j < 2; j++)
	{
		oled_address(page+j, column);
		
		for (i = 0; i < 8; i++)
		{	
			oled_writedata(*dp);	 	/*д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1*/
			dp++;
		}
	}
}


/*
 ***************************************************************
 * ����: oled_display_graphic_16x16 
 * ����: ��ʾ16*16
 * ����: 1. page: ҳ 0-7
 *       2. column: �� 0-127
 *       3. *code: Ҫд�ĺ���
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
void oled_display_graphic_16x16(u16 page, u16 column, u8 *code)
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
		oled_address(page+j, column);
		
		for (i = 0; i < 16; i++)
		{	
			oled_writedata(*dp);	 /*д���ݵ�LCD,ÿд��һ��8λ�����ݺ��е�ַ�Զ���1*/
			dp++;
		}
	}
}


/*
 ***************************************************************
 * ����: oled_Show_Str 
 * ����: ��ҳ��ʾ�ַ���
 * ����: 1. page: ҳ 0-7
 *       2. column: �� 0-127
 *       3. *str: Ҫд���ַ�
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
void oled_Show_Str(u8 page, u16 col, u8* str)
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
				oled_display_graphic_8x16(page, col, *str);//��Ч����д�� 
				str++; 
				col += 8; // �ַ�,Ϊȫ�ֵ�һ�� 
			}
		}
		else	// ���� 
		{     
			bHz = 0;	// �к��ֿ�     						     
			oled_display_graphic_16x16(page, col, str); //��ʾ�������,������ʾ 
			str += 2; 
			col += 16;	// ��һ������ƫ��	    
		}						 
	}   
}  	


/*
 ***************************************************************
 * ����: oled_Show_StrInLine 
 * ����: ������ʾ�ַ���
 * ����: 1. row: �� 1-4
 *       2. column: �� 0-127
 *       3. *str: Ҫд���ַ�
 * ���: NULL
 * ����: NULL
 * ����: NULL
 ***************************************************************
 */
void oled_Show_StrInLine(u8 row, u16 col, u8* str)
{
	if ((row>0) && (row<5) && (col < 120))
		oled_Show_Str((row-1) * 2, col, str);
}

