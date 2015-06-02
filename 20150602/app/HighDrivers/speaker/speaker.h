#ifndef __SPEAKER_H
#define __SPEAKER_H

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
#define SPK_SBT_PORT	GPIOA
#define SPK_SBT_PIN	    GPIO_Pin_0

#define SPK_ADDR0_PORT	GPIOA
#define SPK_ADDR0_PIN	GPIO_Pin_1

#define SPK_ADDR1_PORT	GPIOA
#define SPK_ADDR1_PIN	GPIO_Pin_2

#define SPK_ADDR2_PORT	GPIOA
#define SPK_ADDR2_PIN	GPIO_Pin_3

#define SPK_ADDR3_PORT	GPIOA
#define SPK_ADDR3_PIN	GPIO_Pin_9

#define SPK_ADDR4_PORT	GPIOA
#define SPK_ADDR4_PIN	GPIO_Pin_10

#define SPK_ADDR5_PORT	GPIOB
#define SPK_ADDR5_PIN	GPIO_Pin_8

#define SPK_ADDR6_PORT	GPIOB
#define SPK_ADDR6_PIN	GPIO_Pin_9 
 
 
#define SPK_RESET_PORT	GPIOC
#define SPK_RESET_PIN	GPIO_Pin_9 
 
#define SPK_BUSY_PORT	GPIOA
#define SPK_BUSY_PIN	GPIO_Pin_8 


#define SPK_SBT_0		GPIO_ResetBits(SPK_SBT_PORT, SPK_SBT_PIN)
#define SPK_SBT_1		GPIO_SetBits(SPK_SBT_PORT, SPK_SBT_PIN)
#define SPK_ADDR0_0		GPIO_ResetBits(SPK_ADDR0_PORT, SPK_ADDR0_PIN)
#define SPK_ADDR0_1		GPIO_SetBits(SPK_ADDR0_PORT, SPK_ADDR0_PIN)
#define SPK_ADDR1_0		GPIO_ResetBits(SPK_ADDR1_PORT, SPK_ADDR1_PIN)
#define SPK_ADDR1_1		GPIO_SetBits(SPK_ADDR1_PORT, SPK_ADDR1_PIN)
#define SPK_ADDR2_0		GPIO_ResetBits(SPK_ADDR2_PORT, SPK_ADDR2_PIN)
#define SPK_ADDR2_1		GPIO_SetBits(SPK_ADDR2_PORT, SPK_ADDR2_PIN)
#define SPK_ADDR3_0		GPIO_ResetBits(SPK_ADDR3_PORT, SPK_ADDR3_PIN)
#define SPK_ADDR3_1		GPIO_SetBits(SPK_ADDR3_PORT, SPK_ADDR3_PIN)
#define SPK_ADDR4_0		GPIO_ResetBits(SPK_ADDR4_PORT, SPK_ADDR4_PIN)
#define SPK_ADDR4_1		GPIO_SetBits(SPK_ADDR4_PORT, SPK_ADDR4_PIN)
#define SPK_ADDR5_0		GPIO_ResetBits(SPK_ADDR5_PORT, SPK_ADDR5_PIN)
#define SPK_ADDR5_1		GPIO_SetBits(SPK_ADDR5_PORT, SPK_ADDR5_PIN)
#define SPK_ADDR6_0		GPIO_ResetBits(SPK_ADDR6_PORT, SPK_ADDR6_PIN)
#define SPK_ADDR6_1		GPIO_SetBits(SPK_ADDR6_PORT, SPK_ADDR6_PIN)
#define SPK_RESET_0		GPIO_ResetBits(SPK_RESET_PORT, SPK_RESET_PIN)
#define SPK_RESET_1		GPIO_SetBits(SPK_RESET_PORT, SPK_RESET_PIN)
#define SPK_BUSY_0		GPIO_ResetBits(SPK_BUSY_PORT, SPK_BUSY_PIN)
#define SPK_BUSY_1		GPIO_SetBits(SPK_BUSY_PORT, SPK_BUSY_PIN)


#define SPK_ADDR0(sw)  sw ?  SPK_ADDR0_1 : SPK_ADDR0_0
#define SPK_ADDR1(sw)  sw ?  SPK_ADDR1_1 : SPK_ADDR1_0
#define SPK_ADDR2(sw)  sw ?  SPK_ADDR2_1 : SPK_ADDR2_0
#define SPK_ADDR3(sw)  sw ?  SPK_ADDR3_1 : SPK_ADDR3_0
#define SPK_ADDR4(sw)  sw ?  SPK_ADDR4_1 : SPK_ADDR4_0
#define SPK_ADDR5(sw)  sw ?  SPK_ADDR5_1 : SPK_ADDR5_0
#define SPK_ADDR6(sw)  sw ?  SPK_ADDR6_1 : SPK_ADDR6_0


void Speaker_Pin_Init(void);
void Voice_Segment(u8 seg);

#endif
