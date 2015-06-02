#ifndef __LED_H
#define __LED_H	

#include "sys.h"


#define REDLED_PORT			GPIOC
#define REDLED_PIN			GPIO_Pin_4

#define GREENLED_PORT		GPIOC
#define GREENLED_PIN		GPIO_Pin_5

#define redLed_on()			GPIO_SetBits(REDLED_PORT, REDLED_PIN)
#define redLed_off()		GPIO_ResetBits(REDLED_PORT, REDLED_PIN)

#define greenLed_on()		GPIO_SetBits(GREENLED_PORT, GREENLED_PIN)
#define greenLed_off()		GPIO_ResetBits(GREENLED_PORT, GREENLED_PIN)


typedef enum {
    LED_OFF	= 0x00,							                                     // LED关
    LED_ON  = 0x01,							                                     // LED开
} LED_CTRL;

typedef enum {
    LEDRED = (0x01 << 0),							                         
    LEDGREEN = (0x01 << 1),							                          
} LED_DB;


void Led_Init(void);	// 初始化
void LEDSwitchCtrl(u8 ledNum, u8 sw);
		

#endif

