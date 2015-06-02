#ifndef __SOFTTIMER_H
#define __SOFTTIMER_H

typedef struct {
    u8   State;				// 定时状态(1定时，0不定时)
	u32  InitValue;			// 计数初始值
	u32  CountValue;		// 计数值
	void (*Fun)(u8 num);	// 要运行的任务函数
} SFT_TABLE;


typedef enum {
    SFT_CLOSE = 0x00,		// 关定时
	SFT_OPEN  = 0x01,		// 开定时
} SFT_SFT;

void SFT_Timer2_Irq(void);
void SFT_LED_Start(u8 num);
void SFTStartBlkTime(void);

#endif
