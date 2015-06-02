#include "stm32spi2.h"
#include "userRC522.h"
#include "userCard.h"
#include "delay.h"

u8 userID[4];

u8 defaultKey2[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

u8 MF[]  = { 0x3f, 0x00 };	// 主文件
u8 MIF[] = { 0x00, 0x01 };	// 应用主信息文件
u8 DF[]  = { 0X3f, 0x01 };	// 应用文件1
u8 DIF[] = { 0x00, 0x01 };	// 应用主信息文件


u8 UserCard_Check(void)
{
	u8 flag = 0;
	u8 cardType[2];
	u8 Jbuff[16];

	if (flag == 0)
	{
		userRC522_PcdAntennaOn();
		delay_ms(10);
		flag = 1; 
	}

	if (flag == 1)
	{
		if (userRC522_PcdRequest(PICC_REQIDL, cardType) == MI_OK) 
			flag = 2;
	}
	
	if (flag == 2) 
	{
		if (userRC522_PcdAnticoll(userID) == MI_OK) 
			flag = 3;
	}

	if (flag == 3) 
	{ 
		if (userRC522_PcdSelect(userID) == MI_OK)
			flag = 5;
	}
 
	if (flag == 5) 
	{
		if (userRC522_PcdAuthState(PICC_AUTHENT1A, USER_BLOCK, defaultKey2, userID) == MI_OK) 
			flag = 6;
	}

	
	if (flag == 6)
	{
		if (userRC522_PcdRead(USER_BLOCK, Jbuff) == MI_OK)
		{
			if (Jbuff[0] == 0x00)
			{
				flag = 8;
			}
		}
		else 
			flag = 0;
	}
	 
	if (flag == 8) 
	{
		userRC522_PcdHalt();
		userRC522_PcdAntennaOff();
		flag = 0;
		return 1;
	}
	else 
		return 0;
}


u8 userCardCheck(void)
{
	u8 flag = 0;
	u8 cardType[2];
	u8 buf[16];

	if (flag == 0)
	{	
		userRC522_PcdAntennaOn();
		flag = 1;
	} 	
	
	if (flag == 1)
	{
		if (userRC522_PcdRequest(0x52, cardType) == MI_OK)		// liuchao ？ 为什么这行去掉就不行 
			flag = 2;	
	}
	
	if (flag == 2)
	{
		if (userRC522_PcdAnticoll(userID) == MI_OK) 
			flag = 3;
	}
	
	if (flag == 3)
	{ 
		if (userRC522_PcdSelect(userID) == MI_OK) 
			flag = 4;
	}
	
	if (flag == 4)
	{
		if (userRC522_PcdRats() == MI_OK) 
		{
			flag = 5;
		}
	}

	if (flag == 5)
	{
		if (userRC522_PcdSelectFile(MF))
		{
			userRC522_PcdSelectFile(MIF);	
			userRC522_PcdReadBinary(0x00, 4, buf);
		
			userRC522_PcdSelectFile(DF);
			userRC522_PcdSelectFile(DIF);
			userRC522_PcdReadBinary(0x00, 4, buf);			
			
			flag = 7;
		}	
	}

	if (flag == 7) 
	{
		flag = 0;
		return 1;
	}
	else 
		return 0;
}
