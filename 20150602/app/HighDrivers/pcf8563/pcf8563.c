#include "sys.h"
#include "delay.h"
#include "myiic.h"
#include "pcf8563.h"


/*
 ***************************************************************
 * ����: PCF8563_SS 
 * ����: ������ֹͣpcf8563
 * ����: 1. cmd��PCF8563Start - ����
 *       		 PCF8563Stop - ֹͣ
 * ���: NULL
 * ����: NULL
 * ����: ��
 ***************************************************************
 */
void PCF8563_SS(u8 cmd)
{
   u8 temp;

   if (cmd == PCF8563Start)
      temp = 0;
   else
      temp = 0x20;
   
    IIC_WriteNByte(PCF8563ADDR, 0x00, &temp, 1);
}


/*
 ***************************************************************
 * ����: InitSetTime 
 * ����: ��ʼ����ʱ��
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ��
 ***************************************************************
 */
void InitSetTime(void)
{
	PCF8563_TIME RTCSet;

	RTCSet.year	  = 15; 
	RTCSet.month  = 6;    
	RTCSet.day    = 1;   
	RTCSet.week   = 1;    
	RTCSet.hour   = 9;   
	RTCSet.minute = 30;   
	RTCSet.second = 0;    
	
	PCF8563SetTime(RTCSet);
}


/*
 ***************************************************************
 * ����: PCF8563_Init 
 * ����: PCF8563��ʼ��
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ��
 ***************************************************************
 */
void PCF8563_Init(void)
{	
	IIC_Init();
		
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xa0a0)	// ��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����
	{	 				
		InitSetTime();
		BKP_WriteBackupRegister(BKP_DR1, 0xa0a0);	// ��ָ���ĺ󱸼Ĵ�����д���û���������
	}
		
}


/*
 ***************************************************************
 * ����: PCF8563SetTime 
 * ����: PCF8563����ʱ��
 * ����: 1. Time ��Ҫ���õ�ʱ��
 * ���: NULL
 * ����: NULL
 * ����: ��
 ***************************************************************
 */
void PCF8563SetTime(PCF8563_TIME Time) 
{
	u8 Temp[7];
	
	PCF8563_SS(PCF8563Stop);
	
	Temp[0] = HEC_to_BCD(Time.second);
  	Temp[1] = HEC_to_BCD(Time.minute);
	Temp[2] = HEC_to_BCD(Time.hour);
  	Temp[3] = HEC_to_BCD(Time.day);
  	Temp[4] = HEC_to_BCD(Time.week);
  	Temp[5] = HEC_to_BCD(Time.month);
	Temp[6] = HEC_to_BCD(Time.year);		

	IIC_WriteNByte(PCF8563ADDR, 2, Temp, 7);
}


/*
 ***************************************************************
 * ����: RTCGetTime 
 * ����: PCF8563��ȡʱ��
 * ����: ��
 * ���: ������ʱ��
 * ����: NULL
 * ����: ��
 ***************************************************************
 */
PCF8563_TIME RTCGetTime(void)
{
	u8 Temp[7];
 	PCF8563_TIME LocalTime;
	
	IIC_ReadNByte(PCF8563ADDR, 2, Temp, 7);
		
	LocalTime.second = BCD_to_HEC(Temp[0]);
	LocalTime.minute = BCD_to_HEC(Temp[1]);
	LocalTime.hour   = BCD_to_HEC(Temp[2]);
	LocalTime.day	 = BCD_to_HEC(Temp[3]);
	LocalTime.week   = BCD_to_HEC(Temp[4]);
	LocalTime.month  = BCD_to_HEC(Temp[5]);
	LocalTime.year   = BCD_to_HEC(Temp[6]);
	
	return (LocalTime);	
}


/*
 ***************************************************************
 * ����: GetWeek 
 * ����: �����ڻ�ȡʱ��
 * ����: 1. year: ��
 *       2. month: ��
 *       3. day����
 * ���: ����
 * ����: NULL
 * ����: 0-������ 
 ***************************************************************
 */
u8 GetWeek(u8 year, u8 month, u8 day)
{
	u8 y, m, d, w; 
	u8 c = 20;	
									  
    /*	���գ�Zeller����ʽ: w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1

	��ʽ�еķ��ź������£�w�����ڣ�c������-1��y���꣨��λ������m���£�m���ڵ���3��С�ڵ���14�����ڲ��չ�ʽ�У�
	ĳ���1��2��Ҫ������һ���13��14�������㣬����2003��1��1��Ҫ����2002���13��1�������㣩��d���գ�[ ]����ȡ����
	��ֻҪ�������֡�(C����������һ��y����ݺ���λ��M���·ݣ�d��������1�º�2��Ҫ����һ���13�º�14�����㣬��ʱC��
	y������һ��ȡֵ��) 	*/
	
	y = year;
	m = month;
	d = day;
	
	if (m < 2)	// 1,2����һ���13,14��ʾ
	{
	    m += 12;
		y -= 1;
	}

	w = y+(y/4)+(c/4)+(26*(m+1)/10)+d-1-(2*c);	// ��������
	
	return (w %= 7);
}	
