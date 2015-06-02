#include "sys.h"
#include "can1.h"
#include "can1Comm.h"
#include "lcd12864.h"
#include "ucos_ii.h"
#include "os_cpu.h"

CanRxMsg rxMessage;

CanBaud BaudTable[20] = {
	{										// 10K
		CAN_SJW_1tq,
		CAN_BS1_3tq,
		CAN_BS2_2tq,
		600,
	},
	{										// 20K
		CAN_SJW_1tq,
		CAN_BS1_3tq,
		CAN_BS2_2tq,
		300,
	},
	{										// 30K
		CAN_SJW_1tq,
		CAN_BS1_6tq,
		CAN_BS2_3tq,
		120,
	},
	{										// 40K
		CAN_SJW_1tq,
		CAN_BS1_3tq,
		CAN_BS2_2tq,
		150,
	},
	{										// 50K
		CAN_SJW_1tq,
		CAN_BS1_3tq,
		CAN_BS2_2tq,
		120,
	},
	{										// 60K
		CAN_SJW_1tq,
		CAN_BS1_6tq,
		CAN_BS2_3tq,
		60,
	},
	{										// 80K
		CAN_SJW_1tq,
		CAN_BS1_4tq,
		CAN_BS2_5tq,
		75,
	},
	{										// 90K
		CAN_SJW_1tq,
		CAN_BS1_3tq,
		CAN_BS2_2tq,
		50,
	},
	{										// 100K
		CAN_SJW_1tq,
		CAN_BS1_3tq,
		CAN_BS2_2tq,
		60,
	},
	{										// 125K
		CAN_SJW_1tq,
		CAN_BS1_3tq,
		CAN_BS2_2tq,
		48,
	},
	{										// 150K
		CAN_SJW_1tq,
		CAN_BS1_3tq,
		CAN_BS2_2tq,
		40,
	},
	{										// 200K
		CAN_SJW_1tq,
		CAN_BS1_3tq,
		CAN_BS2_2tq,
		30,
	},
	{										// 250K
		CAN_SJW_1tq,
		CAN_BS1_3tq,
		CAN_BS2_2tq,
		24,
	},
	{										// 300K
		CAN_SJW_1tq,
		CAN_BS1_3tq,
		CAN_BS2_2tq,
		20,
	},
	{										// 400K
		CAN_SJW_1tq,
		CAN_BS1_5tq,
		CAN_BS2_3tq,
		10,
	},
	{										// 500K
		CAN_SJW_1tq,
		CAN_BS1_3tq,
		CAN_BS2_2tq,
		12,
	},
	{										// 600K
		CAN_SJW_1tq,
		CAN_BS1_6tq,
		CAN_BS2_3tq,
		6,
	},
	{										// 800K
		CAN_SJW_1tq,
		CAN_BS1_5tq,
		CAN_BS2_3tq,
		5,
	},
	{										// 900K
		CAN_SJW_1tq,
		CAN_BS1_4tq,
		CAN_BS2_3tq,
		5,
	},
	{										// 1000K
		CAN_SJW_1tq,
		CAN_BS1_3tq,
		CAN_BS2_2tq,
		6,
	},
};


void CAN_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*外设时钟设置*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	/* Configure CAN pin: RX PA11*/									          
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	// 上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure CAN pin: TX PA12 */									               
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	// 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void CAN_Address_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
}


/*
 * 函数名：Read_CAN_Address
 * 描述  ：读取CAN 地址
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
u8 Read_CAN_Address(void)
{
	u8 CANadd;

	CANadd = ((A6<<6) + (A5<<5) + (A4<<4) + (A3<<3) + (A2<<2) + (A1<<1) + A0);
		
	return CANadd;
}


/**
  * @brief  CAN接收中断配置
  * @param  None
  * @retval None
  */
void CAN_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable CAN1 RX0 interrupt IRQ channel */	
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void CAN_ConfigFilter(uint8_t FilterNumber, uint16_t can_addr)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	//设置CAN接收过滤器
	CAN_FilterInitStructure.CAN_FilterNumber = FilterNumber;			// 过滤器1
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;		// 屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	// 32bit模式
	
	//以下4个为0表示接收任何数据
	CAN_FilterInitStructure.CAN_FilterIdHigh = can_addr << (18-16 + 3);		//(can_addr&0x7ff)<<2;
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_ID_EXT | CAN_RTR_DATA;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x7ff) << (18-16 + 3);	//0x7ff<<2;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_ID_EXT | CAN_RTR_DATA;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;	// 使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);	
}


/*
 ***************************************************************
 * 名称: can1_init 
 * 功能: can1初始化
 * 输入: 1.baud_num
 * 输出: NULL 
 * 返回: NULL 
 * 描述: 无 
 ***************************************************************
 */  
void can1_init(u32 baud_num)
{
	CAN_InitTypeDef CAN_InitStructure;
	
	/* CAN register init */
	CAN_NVIC_Configuration();
	CAN_GPIO_Configuration();
	CAN_Address_GPIO_Config();

	CAN_StructInit(&CAN_InitStructure);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;	// 非时间触发通信模式 
	CAN_InitStructure.CAN_ABOM = DISABLE;	// 软件自动离线管理	
	CAN_InitStructure.CAN_AWUM = DISABLE;	// 睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART = DISABLE;	// 禁止报文自动传送  
	CAN_InitStructure.CAN_RFLM = DISABLE;	// 报文不锁定,新的覆盖旧的 
	CAN_InitStructure.CAN_TXFP = ENABLE;	// 优先级由报文标识符决定
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = BaudTable[baud_num].SJW;
	CAN_InitStructure.CAN_BS1 = BaudTable[baud_num].BS1;
	CAN_InitStructure.CAN_BS2 = BaudTable[baud_num].BS2;
	CAN_InitStructure.CAN_Prescaler = BaudTable[baud_num].PreScale;

	CAN_Init(CAN1, &CAN_InitStructure);
	
	//设置CAN接收过滤器
 	CAN_ConfigFilter(0, 0x7ff);	// 广播地址，接受广播命令
	CAN_ConfigFilter(1, Read_CAN_Address()&0x7f);	// 本节点真实地址

	//使能接收中断
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}


void USB_LP_CAN1_RX0_IRQHandler(void)
{
	OS_CPU_SR cpu_sr = 0;
	
	OS_ENTER_CRITICAL();			// 进入临界区(无法被中断打断)    
	
	CAN_Receive(CAN1, CAN_FIFO0, &rxMessage);
	CANRxDeal(&rxMessage);
	CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		
	OS_EXIT_CRITICAL();				// 退出临界区(可以被中断打断)	
}


/**
  * @brief  发送一帧CAN数据
  * @param  CANx CAN通道号
  * @param  TxMessage CAN消息指针
  * @retval None
  */
u8 CAN1_SendOneFrame(CanTxMsg *TxMessage)
{
	u8 TransmitMailbox;   
	u32	TimeOut = 0;
	
	TransmitMailbox = CAN_Transmit(CAN1, TxMessage);
	while (CAN_TransmitStatus(CAN1, TransmitMailbox) != CAN_TxStatus_Ok)
	{
		TimeOut++;
		if (TimeOut > 10000000)
		{
			return 1;
		}
	}
	
	return 0;
}

