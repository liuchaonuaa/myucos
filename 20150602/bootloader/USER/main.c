#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "common.h"
#include "usart3.h"
#include "sst25VF016.h"
#include "crc.h"
#include "iap.h"
#include "lcd12864.h"


/* ±äÁ¿ ----------------------------------------------------------------------*/
extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;

u8 	iap_flag;
u8  ID_write = 0;

u8 dataTest[] = "liuchaonuaa";

u8  UART3_Rx[1060];
u16 UART3_RxLen;

u8  programIndex;

u8  F_startIAP = 0;
u8  F_programStart = 0;
u8  F_programFinish = 0;
u8  F_erase = 0;
u8  F_reset = 0;

extern Comm3_Rx_IapRequest   R_iapRequestInfo;


int main(void)
{		
	delay_init();
	SST25_Init();
	uart3_init(115200);
	Rcv3BufInit();
	CRC_Config();
	FlashReadID();
	lcd12864_init();
	
	Show_Str(0, 0, dataTest);

	iap_flag = FlashRead(FLASH_ADR_1) & 0xff;
	programIndex = FlashRead(FLASH_ADR_2) & 0xff;
	
	if ((iap_flag == 0x55) || (iap_flag == 0xff))
	{
		while (1)
		{
			if (uart3_flag)
			{					
				UART3_RxLen = uart3_Rx.len;
				memcpy(UART3_Rx, uart3_Rx.buf, UART3_RxLen);

				comm3_Analyse(&UART3_Rx, &UART3_RxLen);
		
				uart3_flag = 0;
			}
			
			if (F_startIAP == 1)
			{
				F_startIAP = 0;
				FlashWrite(FLASH_ADR_1, 0x55);
				FlashWrite(FLASH_ADR_2, R_iapRequestInfo.totalNum);
				NVIC_SystemReset();
			}
			
			
			if (F_programStart == 1)
			{
				F_programStart = 0;
				IAP_WRITE();	
			}

			if (F_programFinish == 1)
			{
				F_programFinish = 0;
				USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);	// ¹Ø±ÕÖÐ¶Ï

				NVIC_SystemReset();
			}
			
			if (F_erase == 1)
			{
				FLASH_Unlock();	// ½âËøflash±àÐ´²Á³ý¿ØÖÆÆ÷
				FLASH_ErasePage(ApplicationAddress);
				FLASH_Lock();	// Ëø¶¨flash±àÐ´²Á³ý¿ØÖÆÆ÷				
				F_erase = 0;
			}
			
			if (F_reset == 1)
			{
				FlashWrite(FLASH_ADR_1, 0xaa);
				F_reset = 0;
				NVIC_SystemReset();
			}			
			
		}

	}
	else
	{
		if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000) == 0x20000000)
		{
			JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
			Jump_To_Application = (pFunction) JumpAddress;
			__set_MSP(*(__IO uint32_t*) ApplicationAddress);
			Jump_To_Application();
		}
		else
		{
			FlashWrite(FLASH_ADR_1, 0x55);
		}
	}

	while (1)
	{
		delay_ms(10);
		
		if (uart3_flag)
		{					
			UART3_RxLen = uart3_Rx.len;
			memcpy(UART3_Rx, uart3_Rx.buf, UART3_RxLen);

			comm3_Analyse(&UART3_Rx, &UART3_RxLen);
	
			uart3_flag = 0;
		}
	}
}

