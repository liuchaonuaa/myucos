#include "stm32spi1.h"
#include "bikeRC522.h"
#include "bikeCard.h"
#include "delay.h"

u8 bikeID[4];

u8 defaultKey[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};


u8 BikeCard_Check(void)
{
	u8 flag = 0;
	u8 cardType[2];
	u8 Jbuff[16];

	if (flag == 0)
	{
		bikeRC522_PcdAntennaOn();
		delay_ms(10);
		flag = 1; 
	}

	if (flag == 1)
	{
		if (bikeRC522_PcdRequest(PICC_REQIDL, cardType) == MI_OK) 
			flag = 2;
	}
	
	if (flag == 2) 
	{
		if (bikeRC522_PcdAnticoll(bikeID) == MI_OK) 
			flag = 3;
	}

	if (flag == 3) 
	{ 
		if (bikeRC522_PcdSelect(bikeID) == MI_OK)
			flag = 5;
	}
 
	if (flag == 5) 
	{
		if (bikeRC522_PcdAuthState(PICC_AUTHENT1A, BIKE_BLOCK, defaultKey, bikeID) == MI_OK) 
			flag = 6;
	}

	
	if (flag == 6)
	{
		if (bikeRC522_PcdRead(BIKE_BLOCK, Jbuff) == MI_OK)
		{
			if (Jbuff[0] == 0x01)
			{
				flag = 8;
			}
		}
		else 
			flag = 0;
	}
	 
	if (flag == 8) 
	{
		bikeRC522_PcdHalt();
		bikeRC522_PcdAntennaOff();
		flag = 0;
		return 1;
	}
	else 
		return 0;
}


u8 BikeCard_Check1(void)
{
	u8 flag = 0;
	u8 cardType[2];
	u8 Jbuff[16];

	if (flag == 0)
	{
		bikeRC522_PcdAntennaOn();
		delay_ms(10);
		flag = 1; 
	}

	if (flag == 1)
	{
		if (bikeRC522_PcdRequest(PICC_REQIDL, cardType) == MI_OK) 
			flag = 2;
	}
	
	if (flag == 2) 
	{
		if (bikeRC522_PcdAnticoll(bikeID) == MI_OK) 
			flag = 3;
	}

	if (flag == 3) 
	{ 
		if (bikeRC522_PcdSelect(bikeID) == MI_OK)
			flag = 5;
	}
 
	if (flag == 5) 
	{
		if (bikeRC522_PcdAuthState(PICC_AUTHENT1A, BIKE_BLOCK, defaultKey, bikeID) == MI_OK) 
			flag = 6;
	}

	
	if (flag == 6)
	{
		if (bikeRC522_PcdRead(BIKE_BLOCK, Jbuff) == MI_OK)
		{
			if (Jbuff[0] == 0x01)
			{
				flag = 8;
			}
		}
		else 
			flag = 0;
	}
	 
	if (flag == 8) 
	{
		bikeRC522_PcdHalt();
		bikeRC522_PcdAntennaOff();
		flag = 0;
		return 1;
	}
	else 
		return 0;
}
