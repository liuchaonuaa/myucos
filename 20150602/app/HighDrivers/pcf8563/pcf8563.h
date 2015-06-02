#ifndef __PCF8563_H
#define __PCF8563_H

#define PCF8563ADDR 0xa2

__packed typedef struct {
   u8	year;
   u8	month;
   u8	day;
   u8	week;
   u8	hour;
   u8	minute;
   u8	second;
} PCF8563_TIME;

#define	BCD_to_HEC(b)	(((b>>4)*10)+(b&0x0f))
#define	HEC_to_BCD(h)	(((h/10)<<4)|(h%10))

#define PCF8563Start	1
#define PCF8563Stop		0

void PCF8563_Init(void);
void PCF8563SetTime(PCF8563_TIME Time);
PCF8563_TIME RTCGetTime(void);
u8 GetWeek(u8 year, u8 month, u8 day);


#endif
