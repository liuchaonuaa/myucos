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

	/*����ʱ������*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	/* Configure CAN pin: RX PA11*/									          
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	// ��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure CAN pin: TX PA12 */									               
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	// �����������
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
 * ��������Read_CAN_Address
 * ����  ����ȡCAN ��ַ
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
u8 Read_CAN_Address(void)
{
	u8 CANadd;

	CANadd = ((A6<<6) + (A5<<5) + (A4<<4) + (A3<<3) + (A2<<2) + (A1<<1) + A0);
		
	return CANadd;
}


/**
  * @brief  CAN�����ж�����
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

	//����CAN���չ�����
	CAN_FilterInitStructure.CAN_FilterNumber = FilterNumber;			// ������1
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;		// ����λģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	// 32bitģʽ
	
	//����4��Ϊ0��ʾ�����κ�����
	CAN_FilterInitStructure.CAN_FilterIdHigh = can_addr << (18-16 + 3);		//(can_addr&0x7ff)<<2;
	CAN_FilterInitStructure.CAN_FilterIdLow = CAN_ID_EXT | CAN_RTR_DATA;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x7ff) << (18-16 + 3);	//0x7ff<<2;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = CAN_ID_EXT | CAN_RTR_DATA;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;	// ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);	
}


/*
 ***************************************************************
 * ����: can1_init 
 * ����: can1��ʼ��
 * ����: 1.baud_num
 * ���: NULL 
 * ����: NULL 
 * ����: �� 
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
	CAN_InitStructure.CAN_TTCM = DISABLE;	// ��ʱ�䴥��ͨ��ģʽ 
	CAN_InitStructure.CAN_ABOM = DISABLE;	// ����Զ����߹���	
	CAN_InitStructure.CAN_AWUM = DISABLE;	// ˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
	CAN_InitStructure.CAN_NART = DISABLE;	// ��ֹ�����Զ�����  
	CAN_InitStructure.CAN_RFLM = DISABLE;	// ���Ĳ�����,�µĸ��Ǿɵ� 
	CAN_InitStructure.CAN_TXFP = ENABLE;	// ���ȼ��ɱ��ı�ʶ������
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = BaudTable[baud_num].SJW;
	CAN_InitStructure.CAN_BS1 = BaudTable[baud_num].BS1;
	CAN_InitStructure.CAN_BS2 = BaudTable[baud_num].BS2;
	CAN_InitStructure.CAN_Prescaler = BaudTable[baud_num].PreScale;

	CAN_Init(CAN1, &CAN_InitStructure);
	
	//����CAN���չ�����
 	CAN_ConfigFilter(0, 0x7ff);	// �㲥��ַ�����ܹ㲥����
	CAN_ConfigFilter(1, Read_CAN_Address()&0x7f);	// ���ڵ���ʵ��ַ

	//ʹ�ܽ����ж�
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}


void USB_LP_CAN1_RX0_IRQHandler(void)
{
	OS_CPU_SR cpu_sr = 0;
	
	OS_ENTER_CRITICAL();			// �����ٽ���(�޷����жϴ��)    
	
	CAN_Receive(CAN1, CAN_FIFO0, &rxMessage);
	CANRxDeal(&rxMessage);
	CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		
	OS_EXIT_CRITICAL();				// �˳��ٽ���(���Ա��жϴ��)	
}


/**
  * @brief  ����һ֡CAN����
  * @param  CANx CANͨ����
  * @param  TxMessage CAN��Ϣָ��
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

