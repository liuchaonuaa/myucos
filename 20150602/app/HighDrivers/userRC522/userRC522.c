#include "stm32spi2.h"
#include "userRC522.h"
#include "delay.h"
#include "string.h"

u8 Pcb = 0;
u8 sw[2];

/*
 ***************************************************************
 * 名称: userRC522_delay_ns 
 * 功能: 延时
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
void userRC522_delay_ns(u32 ns)
{
	u32 i;

	for (i = 0; i < ns; i++)
	{
		__nop();
		__nop();
		__nop();
	}
}


/*
 ***************************************************************
 * 名称: userRC522_ReadRawRC 
 * 功能: 读RC522寄存器
 * 输入: 1. Address：寄存器地址
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
u8 userRC522_ReadRawRC(u8 Address)
{
	u8   ucAddr;
	u8   ucResult = 0;
	
	USERRC522_NSS_0;

	ucAddr = ((Address<<1)&0x7E) | 0x80;
	
	SPI2_ReadWriteByte(ucAddr);
	ucResult = SPI2_ReadWriteByte(0x00);

	USERRC522_NSS_1;

	return ucResult;
}


/*
 ***************************************************************
 * 名称: userRC522_WriteRawRC 
 * 功能: 写RC522寄存器
 * 输入: 1. Address：寄存器地址
 *       2. value: 写入的值
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
void userRC522_WriteRawRC(u8 Address, u8 value)
{  
    u8 ucAddr;

	USERRC522_NSS_0;
	
    ucAddr = ((Address<<1)&0x7E);
	SPI2_ReadWriteByte(ucAddr);
	SPI2_ReadWriteByte(value);
	
	USERRC522_NSS_1;
}


/*
 ***************************************************************
 * 名称: userRC522_SetBitMask 
 * 功能: 置RC522寄存器位
 * 输入: 1. reg：寄存器地址
 *       2. mask: 置位值
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
void userRC522_SetBitMask(u8 reg, u8 mask)  
{
    u8 tmp = 0x0;
	
    tmp = userRC522_ReadRawRC(reg);
    userRC522_WriteRawRC(reg, tmp | mask);  // set bit mask
}


/*
 ***************************************************************
 * 名称: userRC522_ClearBitMask 
 * 功能: 清RC522寄存器位
 * 输入: 1. reg：寄存器地址
 *       2. mask: 清位值
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
void userRC522_ClearBitMask(u8 reg, u8 mask)  
{
    u8 tmp = 0x0;
	
    tmp = userRC522_ReadRawRC(reg);
    userRC522_WriteRawRC(reg, tmp & (~mask));  // clear bit mask
} 


/*
 ***************************************************************
 * 名称: userRC522_PcdAntennaOn 
 * 功能: 开启天线  
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: 每次启动或关闭天险发射之间应至少有1ms的间隔
 ***************************************************************
 */
void userRC522_PcdAntennaOn(void)
{
    u8 i;
   
    i = userRC522_ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        userRC522_SetBitMask(TxControlReg, 0x03);
    }
}


/*
 ***************************************************************
 * 名称: userRC522_PcdAntennaOff 
 * 功能: 关闭天线  
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: 每次启动或关闭天险发射之间应至少有1ms的间隔
 ***************************************************************
 */
void userRC522_PcdAntennaOff(void)
{
	userRC522_ClearBitMask(TxControlReg, 0x03);
}


/*
 ***************************************************************
 * 名称: bikeRC522_PcdReset
 * 功能: 复位RC522
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
u8 userRC522_PcdReset(void)
{
	USERRC522_RST_1;
    userRC522_delay_ns(10);
	USERRC522_RST_0;
    userRC522_delay_ns(10);
	USERRC522_RST_1;
    userRC522_delay_ns(10);
    userRC522_WriteRawRC(CommandReg, PCD_RESETPHASE);
	userRC522_WriteRawRC(CommandReg, PCD_RESETPHASE);
    userRC522_delay_ns(10);
    
    userRC522_WriteRawRC(ModeReg, 0x3D);	// 和Mifare卡通讯，CRC初始值0x6363
    userRC522_WriteRawRC(TReloadRegL, 30);           
    userRC522_WriteRawRC(TReloadRegH, 0);
    userRC522_WriteRawRC(TModeReg, 0x8D);
    userRC522_WriteRawRC(TPrescalerReg, 0x3E);	
	userRC522_WriteRawRC(TxAutoReg, 0x40);//必须要
   
    return MI_OK;
}


/*
 ***************************************************************
 * 名称: userRC522_M500PcdConfigISOType
 * 功能: 设置RC522的工作方式 
 * 输入: 1. type: 选择RC522工作方式
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
u8 userRC522_M500PcdConfigISOType(u8 type)
{
	if (type == 'A')	// ISO14443_A
	{ 
		userRC522_ClearBitMask(Status2Reg, 0x08);
		userRC522_WriteRawRC(ModeReg, 0x3D);	// 3F
		userRC522_WriteRawRC(RxSelReg, 0x86);	// 84
		userRC522_WriteRawRC(RFCfgReg, 0x7F);   // 4F
		userRC522_WriteRawRC(TReloadRegL, 30);	// tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
		userRC522_WriteRawRC(TReloadRegH, 0);
		userRC522_WriteRawRC(TModeReg, 0x8D);
		userRC522_WriteRawRC(TPrescalerReg, 0x3E);
		userRC522_delay_ns(1000);
		userRC522_PcdAntennaOn();
	}
	else
	{ 
		return 1; 
	}
	
	return MI_OK;
}


/*
 ***************************************************************
 * 名称: userRC522_InitRc522
 * 功能: 初始化读用户卡的RC522
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
void userRC522_InitRc522(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;     // NRST
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	SPI2_Init();
	
	userRC522_PcdReset();
	userRC522_PcdAntennaOff();
	delay_ms(2);  
	userRC522_PcdAntennaOn();
	userRC522_M500PcdConfigISOType('A');
}


/*
 ***************************************************************
 * 名称: userRC522_Reset_RC522
 * 功能: 复位读用户卡标签的RC522并开启天线
 * 输入: NULL
 * 输出: NULL
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
void userRC522_Reset_RC522(void)
{
	userRC522_PcdReset();
	userRC522_PcdAntennaOff();
	delay_ms(2);  
	userRC522_PcdAntennaOn();
}


/*
 ***************************************************************
 * 名称: userRC522_PcdSwitchPCB
 * 功能: 用MF522计算CRC16函数
 * 输入: 1. *pIn: 输入数据
 *       2. len: 长度
 * 输出: 1. *pOut: 输出数据
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
void userRC522_PcdSwitchPCB(void)
{
	switch (Pcb)
	{
		case 0x00:
			Pcb = 0x0A;
			break;
		case 0x0A:
			Pcb = 0x0B;
			break;
		case 0x0B:
			Pcb = 0x0A;
			break;
	}
}


/*
 ***************************************************************
 * 名称: userRC522_CalulateCRC
 * 功能: 用MF522计算CRC16函数
 * 输入: 1. *pIn: 输入数据
 *       2. len: 长度
 * 输出: 1. *pOut: 输出数据
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
void userRC522_CalulateCRC(u8 *pIn, u8 len, u8 *pOut)
{
    u8   i, n;
   
    userRC522_ClearBitMask(DivIrqReg, 0x04);
    userRC522_WriteRawRC(CommandReg, PCD_IDLE);
    userRC522_SetBitMask(FIFOLevelReg, 0x80);
 
    for (i = 0; i < len; i++)
    {   
		userRC522_WriteRawRC(FIFODataReg, *(pIn +i));   
	}
    
	userRC522_WriteRawRC(CommandReg, PCD_CALCCRC);
   
    i = 0xFF;
    do 
    {
        n = userRC522_ReadRawRC(DivIrqReg);
        i--;
    } while ((i!=0) && !(n&0x04));
  
    pOut[0] = userRC522_ReadRawRC(CRCResultRegL);
    pOut[1] = userRC522_ReadRawRC(CRCResultRegM);
}


/*
 ***************************************************************
 * 名称: userRC522_PcdComMF522
 * 功能: 通过RC522和ISO14443卡通讯
 * 输入: 1. Command: 命令 验证或传输
 *       2. *pIn: 输入数据
 *       3. InLenByte：发送数据的字节长度
 * 输出: 1. *pOut: 输出数据
 *       2. pOutLenBit：返回数据的位长度
 * 返回: NULL
 * 描述: 无
 ***************************************************************
 */
char userRC522_PcdComMF522(u8 Command, 
                 u8 *pIn, 
                 u8 InLenByte,
                 u8 *pOut, 
                 u16 *pOutLenBit)
{
    u8 status = MI_ERR;
    u8 irqEn   = 0x00;
    u8 waitFor = 0x00;
    u8 lastBits;
    u8 n;
    u16 i;

    switch (Command)
    {
        case PCD_AUTHENT:
			irqEn   = 0x12;
			waitFor = 0x10;
			break;
		case PCD_TRANSCEIVE:
			irqEn   = 0x77;
			waitFor = 0x30;
			break;
		default:
			break;
    }
   
    userRC522_WriteRawRC(ComIEnReg, irqEn | 0x80);
    userRC522_ClearBitMask(ComIrqReg, 0x80);	//清所有中断位
    userRC522_WriteRawRC(CommandReg, PCD_IDLE);
    userRC522_SetBitMask(FIFOLevelReg, 0x80);	 	//清FIFO缓存
    
    for (i = 0; i < InLenByte; i++)
    {   
		userRC522_WriteRawRC(FIFODataReg, pIn [i]);    
	}
    	
	userRC522_WriteRawRC(CommandReg, Command);	  

	n = userRC522_ReadRawRC(CommandReg);
    
    if (Command == PCD_TRANSCEIVE)	// 开始传送
    {    
		userRC522_SetBitMask(BitFramingReg, 0x80);  
	}	 
    										 
    //i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
	i = 2000;
    do 
    {
        n = userRC522_ReadRawRC(ComIrqReg);
        i--;
    } while ((i!=0) && !(n&0x01) && !(n&waitFor));
  
    userRC522_ClearBitMask(BitFramingReg, 0x80);

    if (i != 0)
    {    
        if (!(userRC522_ReadRawRC(ErrorReg)&0x1B))
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   
				status = MI_NOTAGERR;   
			}
            if (Command == PCD_TRANSCEIVE)
            {
               	n = userRC522_ReadRawRC(FIFOLevelReg);
              	lastBits = userRC522_ReadRawRC(ControlReg) & 0x07;
                if (lastBits)
                {   
					*pOutLenBit = (n-1)*8 + lastBits;   
				}
                else
                {   
					*pOutLenBit = n*8;   
				}
                if (n == 0)
                {   
					n = 1;    
				}
                if (n > MAXRLEN)
                {   
					n = MAXRLEN;   
				}
                for (i = 0; i < n; i++)
                {   
					pOut [i] = userRC522_ReadRawRC(FIFODataReg);    
				}
            }
        }
        else
        {   
			status = MI_ERR;   
		}
        
    }

    userRC522_SetBitMask(ControlReg, 0x80);           // stop timer now
    userRC522_WriteRawRC(CommandReg, PCD_IDLE); 
    
	return status;
}


/*
 ***************************************************************
 * 名称: userRC522_PcdRequest
 * 功能: 寻卡
 * 输入: 1. req_code: 寻卡方式
 *		    0x52 = 寻感应区内所有符合14443A标准的卡
 * 		    0x26 = 寻未进入休眠状态的卡
 * 输出: 1. pTagType: 卡片类型代码
 *        	0x4400 = Mifare_UltraLight
 *        	0x0400 = Mifare_One(S50)
 *      	0x0200 = Mifare_One(S70)
 *     		0x0800 = Mifare_Pro(X)
 *      	0x4403 = Mifare_DESFire
 * 返回: MI_OK
 * 描述: 无
 ***************************************************************
 */
u8 userRC522_PcdRequest(u8 req_code, u8 *pTagType)
{
	u8 status;  
	u16 unLen;
	u8 ucComMF522Buf[MAXRLEN]; 

	userRC522_ClearBitMask(Status2Reg, 0x08);
	userRC522_WriteRawRC(BitFramingReg, 0x07);
	userRC522_SetBitMask(TxControlReg, 0x03);
 
	ucComMF522Buf[0] = req_code;

	status = userRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 1, ucComMF522Buf, &unLen);

	if ((status == MI_OK) && (unLen == 0x10))
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
	{   
		status = MI_ERR;   
	}
   
	return status;
}


/*
 ***************************************************************
 * 名称: userRC522_PcdAnticoll
 * 功能: 防冲突
 * 输入: 1. *pSnr: 卡片序列号，4字节
 * 输出: 无
 * 返回: MI_OK
 * 描述: 无
 ***************************************************************
 */
u8 userRC522_PcdAnticoll(u8 *pSnr)
{
    u8 status;
    u8 i, snr_check = 0;
    u16 unLen;
    u8 ucComMF522Buf[MAXRLEN]; 
    
    userRC522_ClearBitMask(Status2Reg, 0x08);
    userRC522_WriteRawRC(BitFramingReg, 0x00);
    userRC522_ClearBitMask(CollReg, 0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = userRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, &unLen);

    if (status == MI_OK)
    {
    	 for (i = 0; i < 4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])
         {   
		 	status = MI_ERR;    
		 }
    }
    
    userRC522_SetBitMask(CollReg, 0x80);
  
    return status;
}


/*
 ***************************************************************
 * 名称: userRC522_PcdSelect
 * 功能: 选定卡片
 * 输入: 1. *pSnr: 卡片序列号，4字节
 * 输出: 无
 * 返回: MI_OK
 * 描述: 无
 ***************************************************************
 */
u8 userRC522_PcdSelect(u8 *pSnr)
{
    u8 status;
    u8 i;
    u16 unLen;
    u8 ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    
	for (i = 0; i < 4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }

    userRC522_CalulateCRC(ucComMF522Buf, 7, &ucComMF522Buf[7]);
  
    userRC522_ClearBitMask(Status2Reg, 0x08);

    status = userRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 9, ucComMF522Buf, &unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   
		status = MI_OK;  
	}
    else
    {   
		status = MI_ERR;    
	}

    return status;
}


/*
 ***************************************************************
 * 名称: userRC522_PcdAuthState
 * 功能: 验证卡片密码
 * 输入: 1. auth_mode: 密码验证模式
 *                     0x60 = 验证A密钥
 *                     0x61 = 验证B密钥    
 * 		 2. addr: 块地址
 *       3. *pKey: 密码
 *       4. *pSnr: 卡序列号
 * 输出: 无
 * 返回: MI_OK
 * 描述: 无
 ***************************************************************
 */
u8 userRC522_PcdAuthState(u8 auth_mode, u8 addr, u8 *pKey, u8 *pSnr)
{
    u8 status;
    u16 unLen;
    u8 ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;

    memcpy(&ucComMF522Buf[2], pKey, 6); 
    memcpy(&ucComMF522Buf[8], pSnr, 4); 
    
    status = userRC522_PcdComMF522(PCD_AUTHENT, ucComMF522Buf, 12, ucComMF522Buf, &unLen);
    if ((status != MI_OK) || (!(userRC522_ReadRawRC(Status2Reg) & 0x08)))
    {   
		status = MI_ERR;   
	}
		
    return status;
}


/*
 ***************************************************************
 * 名称: userRC522_PcdRead
 * 功能: 读取M1卡一块数据
 * 输入: 1. addr: 要读的块地址
 * 输出: 1. *p：读出的数据
 * 返回: MI_OK
 * 描述: 无
 ***************************************************************
 */
u8 userRC522_PcdRead(u8 addr, u8 *p)
{
    u8 status;
    u16 unLen;
    u8 i, ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    userRC522_CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);
   
    status = userRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);
    if ((status == MI_OK) && (unLen == 0x90))
    {
        for (i=0; i<16; i++)
        {    
			*(p + i) = ucComMF522Buf[i];   
		}
    }
    else
    {   
		status = MI_ERR;   
	}
    
    return status;
}


/*
 ***************************************************************
 * 名称: userRC522_PcdWrite
 * 功能: 写数据到M1卡一块
 * 输入: 1. addr: 要读的块地址
 *       2. *p：要写的数据
 * 输出: 无
 * 返回: MI_OK
 * 描述: 无
 ***************************************************************
 */
u8 userRC522_PcdWrite(u8 addr, u8 *p)
{
    u8 status;
    u16 unLen;
    u8 i, ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    userRC522_CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);
 
    status = userRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   
		status = MI_ERR;   
	}
        
    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, p , 16);
        for (i = 0; i < 16; i++)
        {    
        	ucComMF522Buf[i] = *(p + i);   
        }
		
        userRC522_CalulateCRC(ucComMF522Buf, 16, &ucComMF522Buf[16]);

        status = userRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 18, ucComMF522Buf, &unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   
			status = MI_ERR;   
		}
    }
    
    return status;
}


/*
 ***************************************************************
 * 名称: userRC522_PcdHalt
 * 功能: 命令卡片进入休眠状态
 * 输入: 无
 * 输出: 无
 * 返回: MI_OK
 * 描述: 无
 ***************************************************************
 */
u8 userRC522_PcdHalt(void)
{
    u8 status;
    u16 unLen;
    u8 ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    userRC522_CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);
 
    status = userRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);

    return status;
}


/*
 ***************************************************************
 * 名称: userRC522_PcdRats
 * 功能: 转入APDU模式
 * 输入: 无
 * 输出: 无
 * 返回: MI_OK
 * 描述: 无
 ***************************************************************
 */
s8 userRC522_PcdRats(void)	
{
	s8 status = MI_ERR;  
	u16 unLen;
	u8 ucComMF522Buf[MAXRLEN]; 

	userRC522_ClearBitMask(Status2Reg, 0x08);

	memset(ucComMF522Buf, 0x00, MAXRLEN);

	ucComMF522Buf[0] = 0xE0;		
	ucComMF522Buf[1] = 0x51;				

    userRC522_CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

	status = userRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);

	if (status == MI_OK)
	{
		status = MI_OK;
	}	 
	else
		status = MI_ERR;	 
	
	return  status;				
}


/*
 ***************************************************************
 * 名称: userRC522_PcdGetChallenge
 * 功能: 加密随机数
 * 输入: 无
 * 输出: 1. pRan: 8个加密随机数
 * 返回: MI_OK
 * 描述: 无
 ***************************************************************
 */
s8 userRC522_PcdGetChallenge(u8* pRan)
{
	s8 status = MI_ERR;
	u16 unLen;
	u8 i, ucComMF522Buf[MAXRLEN];

	userRC522_ClearBitMask(Status2Reg, 0x08);
 
	memset(ucComMF522Buf, 0x00, MAXRLEN);
	
 	userRC522_PcdSwitchPCB();
	
	ucComMF522Buf[0] = Pcb;
	ucComMF522Buf[1] = 0x01;
	ucComMF522Buf[2] = 0x00;		
	ucComMF522Buf[3] = 0x84;				
	ucComMF522Buf[4] = 0x00;
	ucComMF522Buf[5] = 0x00;			
	ucComMF522Buf[6] = 0x08;			

    userRC522_CalulateCRC(ucComMF522Buf, 7, &ucComMF522Buf[7]);
	
	status = userRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 9, ucComMF522Buf, &unLen);	
	
	if (status == MI_OK)
	{
		sw[0] = ucComMF522Buf[10];           
		sw[1] = ucComMF522Buf[11];
		if ((sw[0] == 0x90) && (sw[1] == 0x00))
		{
			for (i = 0; i < 8; i++)
				*(pRan+i) = ucComMF522Buf[2+i];
		}
	}
	else
	{
		sw[0] = 0xFF;
		sw[1] = 0xFF; 
	}
	
	return status;
}


/*
 ***************************************************************
 * 名称: userRC522_PcdExAuth
 * 功能: 外部认证
 * 输入: 1. keysign: 外部认证密钥标识号
 *       2. *pRan: 8字节加密随机数
 * 输出: 无
 * 返回: MI_OK
 * 描述: 无
 ***************************************************************
 */
s8 userRC522_PcdExAuth(u8 keysign, u8 *pRan)
{
	s8 status = MI_ERR;  
	u16 unLen;
	u8 ucComMF522Buf[MAXRLEN]; 

	userRC522_ClearBitMask(Status2Reg, 0x08);	// 清空校验成功标志，清除MFCrypto1On位
	
	memset(ucComMF522Buf, 0x00, MAXRLEN);
	
 	userRC522_PcdSwitchPCB();
	
	ucComMF522Buf[0]  = Pcb;	
	ucComMF522Buf[1]  = 0x01;
	ucComMF522Buf[2]  = 0x00;	
	ucComMF522Buf[3]  = 0x82;				
	ucComMF522Buf[4]  = 0x00;
	ucComMF522Buf[5]  = keysign;	// 认证的密码标识号		
	ucComMF522Buf[6]  = 0x08;
	ucComMF522Buf[7]  = pRan[0];	// 8字节随机数
	ucComMF522Buf[8]  = pRan[1];
	ucComMF522Buf[9]  = pRan[2];
	ucComMF522Buf[10] = pRan[3];
	ucComMF522Buf[11] = pRan[4];
	ucComMF522Buf[12] = pRan[5];
	ucComMF522Buf[13] = pRan[6];
	ucComMF522Buf[14] = pRan[7];

    userRC522_CalulateCRC(ucComMF522Buf, 15, &ucComMF522Buf[15]);	
										
	status = userRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 17, ucComMF522Buf, &unLen);	// 将收到的卡片类型号保存
	
	if (status == MI_OK)
	{
		sw[0] = ucComMF522Buf[2];
		sw[1] = ucComMF522Buf[3]; 
		
		if ((sw[0]==0x90) && (sw[1]==0x00))
		{
			status = MI_OK;
		}
		else 
			status = MI_ERR;
		
	}
	else
	{ 
 		sw[0] = 0xFF;
		sw[1] = 0xFF; 
	}

	return status;
}


/*
 ***************************************************************
 * 名称: userRC522_PcdSelectFile
 * 功能: 选择文件 
 * 输入: 1. *pDataIn: 文件标识符或DF名称
 * 输出: 无
 * 返回: MI_OK
 * 描述: 无
 ***************************************************************
 */
s8 userRC522_PcdSelectFile(u8* pDataIn)
{
	s8 status = MI_ERR;  
	u16 unLen;
	u8 ucComMF522Buf[MAXRLEN]; 

	userRC522_ClearBitMask(Status2Reg, 0x08);	// 清除校验成功标志，清除MFCrypto1On位

	memset(ucComMF522Buf, 0x00, MAXRLEN);

 	userRC522_PcdSwitchPCB();
	
	ucComMF522Buf[0] = Pcb;
	ucComMF522Buf[1] = 0x01;
	ucComMF522Buf[2] = 0x00;
	ucComMF522Buf[3] = 0xA4;				
	ucComMF522Buf[4] = 0x00;	// 计算种类							
	ucComMF522Buf[5] = 0x00;	// 认证的密码标识号			
	ucComMF522Buf[6] = 0x02;									
	ucComMF522Buf[7] = pDataIn[0];	// 写入内容
	ucComMF522Buf[8] = pDataIn[1];	
	
    userRC522_CalulateCRC(ucComMF522Buf, 9, &ucComMF522Buf[9]);	

	status = userRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 11, ucComMF522Buf, &unLen);	// 将收到的卡片类型号保存			
	
	if (status == MI_OK)
	{
		sw[0] = ucComMF522Buf[unLen/8 - 4];
		sw[1] = ucComMF522Buf[unLen/8 - 3];
	}
	else
	{
		sw[0] = 0xFF;
		sw[1] = 0xFF; 
	}
	
	if ((sw[0] == 0x90) && (sw[1] == 0x00))	// 这个是什么意思 liuchao ?
		return 1;
	else 
		return 0;
}


/*
 ***************************************************************
 * 名称: userRC522_PcdReadBinary
 * 功能: 读取二进制文件
 * 输入: 1. offset: 偏移
 *       2. Len
 * 输出: 1. *pDataOut
 * 返回: MI_OK
 * 描述: 无
 ***************************************************************
 */
s8 userRC522_PcdReadBinary(u8 offset, u8 Len, u8 *pDataOut)
{
	s8 status = MI_ERR;  
	u16 unLen;
	u8 i, ucComMF522Buf[MAXRLEN]; 

	userRC522_ClearBitMask(Status2Reg, 0x08);
		
	memset(ucComMF522Buf, 0x00, MAXRLEN);
	
 	userRC522_PcdSwitchPCB();

	ucComMF522Buf[0] = Pcb;
	ucComMF522Buf[1] = 0x01;
	ucComMF522Buf[2] = 0x00;
	ucComMF522Buf[3] = 0xB0;				
	ucComMF522Buf[4] = 0x00;		// 当前文件
	ucComMF522Buf[5] = offset;		// 偏移起始位置
	ucComMF522Buf[6] = Len;			// 欲读取信息长度
	
	userRC522_CalulateCRC(ucComMF522Buf, 7, &ucComMF522Buf[7]);		
		
	status = userRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 9, ucComMF522Buf, &unLen);

	if (status == MI_OK)
	{
		sw[0] = ucComMF522Buf[Len+2];
		sw[1] = ucComMF522Buf[Len+3];
		if ((sw[0] == 0x90) && (sw[1] == 0x00))
		{
			for (i = 0; i < Len; i++)
        		*(pDataOut+i) = ucComMF522Buf[i+2];		
		}
	}
	else
	{
		sw[0] = 0xFF;
		sw[1] = 0xFF; 
	}
	
	return status;
}


/*
 ***************************************************************
 * 名称: userRC522_PcdUpdateBinary
 * 功能: 写入二进制文件
 * 输入: 1. offset: 最高位不为1，P1P2是欲读文件的偏移量
 *          doffset:P1高3位100，P1低5位是短文件标识符，P2为读偏移量
 *       2. Len: 写入信息字节数，最大64-7=57
 *       3. *pDataIn: 写入文件数据
 * 输出: 无
 * 返回: MI_OK
 * 描述: 无
 ***************************************************************
 */
s8 userRC522_PcdUpdateBinary(u8 offset, u8 Len, u8* pDataIn)
{
	s8 status = MI_ERR;  
	u16 unLen;
	u8 i, ucComMF522Buf[MAXRLEN]; 

	userRC522_ClearBitMask(Status2Reg, 0x08);

	memset(ucComMF522Buf, 0x00, MAXRLEN);

 	userRC522_PcdSwitchPCB();
	
	ucComMF522Buf[0] = Pcb;
	ucComMF522Buf[1] = 0x01;
	ucComMF522Buf[2] = 0x00;
	ucComMF522Buf[3] = 0xD6;				
	ucComMF522Buf[4] = 0x00;		
	ucComMF522Buf[5] = offset;										
	ucComMF522Buf[6] = Len;											
	
	for (i = 0; i < Len; i++)										
		ucComMF522Buf[7+i] = *(pDataIn+i);							
		
	userRC522_CalulateCRC(ucComMF522Buf, 7+Len, &ucComMF522Buf[7+Len]);
	
	status = userRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 9+Len, ucComMF522Buf, &unLen);

	if (status == MI_OK)
	{
		sw[0] = ucComMF522Buf[2];
		sw[1] = ucComMF522Buf[3];
		
		if ((sw[0] == 0x90) && (sw[1] == 0x00))
			status = MI_OK;
		else
			status = MI_ERR;
	}
	else
	{
		sw[0] = 0xFF;
		sw[1] = 0xFF; 
	}
		
	return status;
}

