#include "sys.h"
#include "delay.h"
#include "myiic.h"
#include "pcf8563.h"


/*
 ***************************************************************
 * 名称: PCF8563_SS 
 * 功能: 启动或停止pcf8563
 * 输入: 1. cmd：PCF8563Start - 启动
 *       		 PCF8563Stop - 停止
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
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
 * 名称: InitSetTime 
 * 功能: 初始设置时间
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
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
 * 名称: PCF8563_Init 
 * 功能: PCF8563初始化
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
void PCF8563_Init(void)
{	
	IIC_Init();
		
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xa0a0)	// 从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
	{	 				
		InitSetTime();
		BKP_WriteBackupRegister(BKP_DR1, 0xa0a0);	// 向指定的后备寄存器中写入用户程序数据
	}
		
}


/*
 ***************************************************************
 * 名称: PCF8563SetTime 
 * 功能: PCF8563设置时间
 * 输入: 1. Time ：要设置的时间
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
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
 * 名称: RTCGetTime 
 * 功能: PCF8563获取时间
 * 输入: 无
 * 输出: 读到的时间
 * 返回: NULL
 * 描述: 无
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
 * 名称: GetWeek 
 * 功能: 由日期获取时间
 * 输入: 1. year: 年
 *       2. month: 月
 *       3. day：日
 * 输出: 星期
 * 返回: NULL
 * 描述: 0-星期天 
 ***************************************************************
 */
u8 GetWeek(u8 year, u8 month, u8 day)
{
	u8 y, m, d, w; 
	u8 c = 20;	
									  
    /*	蔡勒（Zeller）公式: w=y+[y/4]+[c/4]-2c+[26(m+1)/10]+d-1

	公式中的符号含义如下，w：星期；c：世纪-1；y：年（两位数）；m：月（m大于等于3，小于等于14，即在蔡勒公式中，
	某年的1、2月要看作上一年的13、14月来计算，比如2003年1月1日要看作2002年的13月1日来计算）；d：日；[ ]代表取整，
	即只要整数部分。(C是世纪数减一，y是年份后两位，M是月份，d是日数。1月和2月要按上一年的13月和14月来算，这时C和
	y均按上一年取值。) 	*/
	
	y = year;
	m = month;
	d = day;
	
	if (m < 2)	// 1,2用上一年的13,14表示
	{
	    m += 12;
		y -= 1;
	}

	w = y+(y/4)+(c/4)+(26*(m+1)/10)+d-1-(2*c);	// 计算星期
	
	return (w %= 7);
}	
