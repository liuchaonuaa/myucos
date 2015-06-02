#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"


//IO��������
#define SDA_IN()  {GPIOE->CRH&=0XFFFFFF0F;GPIOE->CRH|=8<<(1*4);}
#define SDA_OUT() {GPIOE->CRH&=0XFFFFFF0F;GPIOE->CRH|=3<<(1*4);}

//IO��������	 
#define IIC_SCL    PEout(8) // SCL
#define IIC_SDA    PEout(9) // SDA	 
#define READ_SDA   PEin(9)  // ����SDA 

//IIC���в�������
void IIC_Init(void);                	// ��ʼ��IIC��IO��				 
void IIC_Start(void);					// ����IIC��ʼ�ź�
void IIC_Stop(void);	  				// ����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);				// IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);	// IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 					// IIC�ȴ�ACK�ź�
void IIC_Ack(void);						// IIC����ACK�ź�
void IIC_NAck(void);					// IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 IIC_Read_One_Byte(u8 daddr, u8 addr);	  
void IIC_WriteNByte(u8 sla, u8 suba, u8 *data, u8 len);
void IIC_ReadNByte(u8 sla, u8 suba, u8 *data, u8 len);

#endif
