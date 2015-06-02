#include "sys.h"
#include "delay.h"
#include "speaker.h"

/*
 *****************************
 * SPK_SBT     <-----> PA0
 * SPK_ADDR0   <-----> PA1
 * SPK_ADDR1   <-----> PA2
 * SPK_ADDR2   <-----> PA3
 * SPK_ADDR3   <-----> PA9
 * SPK_ADDR4   <-----> PA10
 * SPK_ADDR5   <-----> PB8
 * SPK_ADDR6   <-----> PB9
 * SPK_RESET   <-----> PC9
 * SPK_BUSY    <-----> PA8
 ******************************
 */
/*
 ***************************************************************
 * 名称: Speaker_Pin_Init 
 * 功能: WTV040语音芯片初始化
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void Speaker_Pin_Init(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
							  
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
									GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;		 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;		 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 	
	
	SPK_RESET_1;
	SPK_SBT_1;	
}


/*
 ***************************************************************
 * 名称: Speaker_Reset 
 * 功能: WTV040语音芯片软件复位
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void Speaker_Reset(void)
{
	SPK_RESET_0;
	delay_ms(10);
	SPK_RESET_1;	
}


/*
 ***************************************************************
 * 名称: Voice_Segment 
 * 功能: WTV040语音芯片播放指定某一段
 * 输入: 1. seg：要播放的那段语音 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void Voice_Segment(u8 seg)
{
	Speaker_Reset();

	SPK_ADDR0(seg & 0x01);
	SPK_ADDR1(seg & 0x02);
	SPK_ADDR2(seg & 0x04);
	SPK_ADDR3(seg & 0x08);
	SPK_ADDR4(seg & 0x10);
	SPK_ADDR5(seg & 0x20);
	SPK_ADDR6(seg & 0x40);
		
	delay_ms(10);
	SPK_SBT_0;
	delay_ms(500);
	SPK_SBT_1;	
}
