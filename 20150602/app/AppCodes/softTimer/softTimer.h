#ifndef __SOFTTIMER_H
#define __SOFTTIMER_H

typedef struct {
    u8   State;				// ��ʱ״̬(1��ʱ��0����ʱ)
	u32  InitValue;			// ������ʼֵ
	u32  CountValue;		// ����ֵ
	void (*Fun)(u8 num);	// Ҫ���е�������
} SFT_TABLE;


typedef enum {
    SFT_CLOSE = 0x00,		// �ض�ʱ
	SFT_OPEN  = 0x01,		// ����ʱ
} SFT_SFT;

void SFT_Timer2_Irq(void);
void SFT_LED_Start(u8 num);
void SFTStartBlkTime(void);

#endif
