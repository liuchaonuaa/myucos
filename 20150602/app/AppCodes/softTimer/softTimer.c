#include "sys.h"
#include "delay.h"
#include "led.h"
#include "softTimer.h "

static void SFT_CARDLEDFlash(u8 num);
static void SFTStopBlkTime(u8 num);


static SFT_TABLE SFT_LEDTlb[] =											// 定时器
{
    { SFT_CLOSE, 50, 50, SFT_CARDLEDFlash },							// 秒闪烁
	{ SFT_CLOSE, 500, 500, SFTStopBlkTime },							// 刷卡背光时间
};


/*
 ***************************************************************
 * 名称: SFT_Timer2_Irq 
 * 功能: 定时器2中断服务
 * 输入: NULL 
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
void SFT_Timer2_Irq(void)
{
    u8 i;

	for (i=0; i<(sizeof(SFT_LEDTlb)/sizeof(SFT_LEDTlb[0])); i++)					   // 循环判断
	{
	    if (SFT_LEDTlb[i].State == SFT_OPEN)
		{
			if (SFT_LEDTlb[i].CountValue)					                       // 不为零
			{
			    SFT_LEDTlb[i].CountValue--;
				if (SFT_LEDTlb[i].CountValue == 0)			                       // 定时时间到
				{
					SFT_LEDTlb[i].CountValue = SFT_LEDTlb[i].InitValue;
					SFT_LEDTlb[i].State = SFT_CLOSE;			                   // 标志清除
					SFT_LEDTlb[i].Fun(i);				                           // 运行任务
				}
			}
		}
	}
}



/*
 ***************************************************************
 * 名称: SFT_CARDLEDFlash 
 * 功能: 背光闪烁
 * 输入: 1. num - 在定时器中的序号
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
static void SFT_CARDLEDFlash(u8 num)
{
	static u8 i;
	
	i++;
	SFT_LEDTlb[num].State = SFT_OPEN;	// 重新打开定时器

	if (i % 2)
	{
		LEDSwitchCtrl(LEDRED, LED_OFF); 
	}
	else
	{
		LEDSwitchCtrl(LEDRED, LED_ON); 		
	}		
	
}


void SFT_LED_Start(u8 num)
{
	SFT_LEDTlb[num].CountValue = SFT_LEDTlb[num].InitValue;		// 赋值
    SFT_LEDTlb[num].State = SFT_OPEN;							// 开定时器
}


void SFT_LED_Stop(u8 num)
{
    SFT_LEDTlb[num].State = SFT_CLOSE;
}


void SFTStartBlkTime(void)
{
	SFT_LED_Start(1);
	LEDSwitchCtrl(LEDGREEN, LED_ON); 
}												 


/*
 ***************************************************************
 * 名称: SFTStartBlkTime 
 * 功能: 关闭背光控制定时器
 * 输入: 1. num - 在定时器中的序号
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */
static void SFTStopBlkTime(u8 num)
{
	LEDSwitchCtrl(LEDGREEN, LED_OFF); 
}
