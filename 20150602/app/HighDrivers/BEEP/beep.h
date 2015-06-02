#ifndef __BEEP_H
#define __BEEP_H	

#include "sys.h"

//蜂鸣器端口定义
#define BEEP_PORT			GPIOD
#define BEEP_PIN			GPIO_Pin_12
#define BEEP_APB2Periph		RCC_APB2Periph_GPIOD

#define BEEP_SetHigh()		GPIO_SetBits(BEEP_PORT, BEEP_PIN)
#define BEEP_SetLow()		GPIO_ResetBits(BEEP_PORT, BEEP_PIN)

void Beep_Init(void);	//初始化
void Beep_On(u16 nms);
	 				    
#endif

