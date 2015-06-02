#include "sys.h"
#include "delay.h"
#include "led.h"
#include "softTimer.h "

static void SFT_CARDLEDFlash(u8 num);
static void SFTStopBlkTime(u8 num);


static SFT_TABLE SFT_LEDTlb[] =											// ��ʱ��
{
    { SFT_CLOSE, 50, 50, SFT_CARDLEDFlash },							// ����˸
	{ SFT_CLOSE, 500, 500, SFTStopBlkTime },							// ˢ������ʱ��
};


/*
 ***************************************************************
 * ����: SFT_Timer2_Irq 
 * ����: ��ʱ��2�жϷ���
 * ����: NULL 
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
void SFT_Timer2_Irq(void)
{
    u8 i;

	for (i=0; i<(sizeof(SFT_LEDTlb)/sizeof(SFT_LEDTlb[0])); i++)					   // ѭ���ж�
	{
	    if (SFT_LEDTlb[i].State == SFT_OPEN)
		{
			if (SFT_LEDTlb[i].CountValue)					                       // ��Ϊ��
			{
			    SFT_LEDTlb[i].CountValue--;
				if (SFT_LEDTlb[i].CountValue == 0)			                       // ��ʱʱ�䵽
				{
					SFT_LEDTlb[i].CountValue = SFT_LEDTlb[i].InitValue;
					SFT_LEDTlb[i].State = SFT_CLOSE;			                   // ��־���
					SFT_LEDTlb[i].Fun(i);				                           // ��������
				}
			}
		}
	}
}



/*
 ***************************************************************
 * ����: SFT_CARDLEDFlash 
 * ����: ������˸
 * ����: 1. num - �ڶ�ʱ���е����
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
static void SFT_CARDLEDFlash(u8 num)
{
	static u8 i;
	
	i++;
	SFT_LEDTlb[num].State = SFT_OPEN;	// ���´򿪶�ʱ��

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
	SFT_LEDTlb[num].CountValue = SFT_LEDTlb[num].InitValue;		// ��ֵ
    SFT_LEDTlb[num].State = SFT_OPEN;							// ����ʱ��
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
 * ����: SFTStartBlkTime 
 * ����: �رձ�����ƶ�ʱ��
 * ����: 1. num - �ڶ�ʱ���е����
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
 ***************************************************************
 */
static void SFTStopBlkTime(u8 num)
{
	LEDSwitchCtrl(LEDGREEN, LED_OFF); 
}
