#include "stm32spi2.h"
#include "userRC522.h"
#include "delay.h"
#include "string.h"

u8 Pcb = 0;
u8 sw[2];

/*
 ***************************************************************
 * ����: userRC522_delay_ns 
 * ����: ��ʱ
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ��
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
 * ����: userRC522_ReadRawRC 
 * ����: ��RC522�Ĵ���
 * ����: 1. Address���Ĵ�����ַ
 * ���: NULL
 * ����: NULL
 * ����: ��
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
 * ����: userRC522_WriteRawRC 
 * ����: дRC522�Ĵ���
 * ����: 1. Address���Ĵ�����ַ
 *       2. value: д���ֵ
 * ���: NULL
 * ����: NULL
 * ����: ��
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
 * ����: userRC522_SetBitMask 
 * ����: ��RC522�Ĵ���λ
 * ����: 1. reg���Ĵ�����ַ
 *       2. mask: ��λֵ
 * ���: NULL
 * ����: NULL
 * ����: ��
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
 * ����: userRC522_ClearBitMask 
 * ����: ��RC522�Ĵ���λ
 * ����: 1. reg���Ĵ�����ַ
 *       2. mask: ��λֵ
 * ���: NULL
 * ����: NULL
 * ����: ��
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
 * ����: userRC522_PcdAntennaOn 
 * ����: ��������  
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
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
 * ����: userRC522_PcdAntennaOff 
 * ����: �ر�����  
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
 ***************************************************************
 */
void userRC522_PcdAntennaOff(void)
{
	userRC522_ClearBitMask(TxControlReg, 0x03);
}


/*
 ***************************************************************
 * ����: bikeRC522_PcdReset
 * ����: ��λRC522
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ��
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
    
    userRC522_WriteRawRC(ModeReg, 0x3D);	// ��Mifare��ͨѶ��CRC��ʼֵ0x6363
    userRC522_WriteRawRC(TReloadRegL, 30);           
    userRC522_WriteRawRC(TReloadRegH, 0);
    userRC522_WriteRawRC(TModeReg, 0x8D);
    userRC522_WriteRawRC(TPrescalerReg, 0x3E);	
	userRC522_WriteRawRC(TxAutoReg, 0x40);//����Ҫ
   
    return MI_OK;
}


/*
 ***************************************************************
 * ����: userRC522_M500PcdConfigISOType
 * ����: ����RC522�Ĺ�����ʽ 
 * ����: 1. type: ѡ��RC522������ʽ
 * ���: NULL
 * ����: NULL
 * ����: ��
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
 * ����: userRC522_InitRc522
 * ����: ��ʼ�����û�����RC522
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ��
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
 * ����: userRC522_Reset_RC522
 * ����: ��λ���û�����ǩ��RC522����������
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ��
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
 * ����: userRC522_PcdSwitchPCB
 * ����: ��MF522����CRC16����
 * ����: 1. *pIn: ��������
 *       2. len: ����
 * ���: 1. *pOut: �������
 * ����: NULL
 * ����: ��
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
 * ����: userRC522_CalulateCRC
 * ����: ��MF522����CRC16����
 * ����: 1. *pIn: ��������
 *       2. len: ����
 * ���: 1. *pOut: �������
 * ����: NULL
 * ����: ��
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
 * ����: userRC522_PcdComMF522
 * ����: ͨ��RC522��ISO14443��ͨѶ
 * ����: 1. Command: ���� ��֤����
 *       2. *pIn: ��������
 *       3. InLenByte���������ݵ��ֽڳ���
 * ���: 1. *pOut: �������
 *       2. pOutLenBit���������ݵ�λ����
 * ����: NULL
 * ����: ��
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
    userRC522_ClearBitMask(ComIrqReg, 0x80);	//�������ж�λ
    userRC522_WriteRawRC(CommandReg, PCD_IDLE);
    userRC522_SetBitMask(FIFOLevelReg, 0x80);	 	//��FIFO����
    
    for (i = 0; i < InLenByte; i++)
    {   
		userRC522_WriteRawRC(FIFODataReg, pIn [i]);    
	}
    	
	userRC522_WriteRawRC(CommandReg, Command);	  

	n = userRC522_ReadRawRC(CommandReg);
    
    if (Command == PCD_TRANSCEIVE)	// ��ʼ����
    {    
		userRC522_SetBitMask(BitFramingReg, 0x80);  
	}	 
    										 
    //i = 600;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
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
 * ����: userRC522_PcdRequest
 * ����: Ѱ��
 * ����: 1. req_code: Ѱ����ʽ
 *		    0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�
 * 		    0x26 = Ѱδ��������״̬�Ŀ�
 * ���: 1. pTagType: ��Ƭ���ʹ���
 *        	0x4400 = Mifare_UltraLight
 *        	0x0400 = Mifare_One(S50)
 *      	0x0200 = Mifare_One(S70)
 *     		0x0800 = Mifare_Pro(X)
 *      	0x4403 = Mifare_DESFire
 * ����: MI_OK
 * ����: ��
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
 * ����: userRC522_PcdAnticoll
 * ����: ����ͻ
 * ����: 1. *pSnr: ��Ƭ���кţ�4�ֽ�
 * ���: ��
 * ����: MI_OK
 * ����: ��
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
 * ����: userRC522_PcdSelect
 * ����: ѡ����Ƭ
 * ����: 1. *pSnr: ��Ƭ���кţ�4�ֽ�
 * ���: ��
 * ����: MI_OK
 * ����: ��
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
 * ����: userRC522_PcdAuthState
 * ����: ��֤��Ƭ����
 * ����: 1. auth_mode: ������֤ģʽ
 *                     0x60 = ��֤A��Կ
 *                     0x61 = ��֤B��Կ    
 * 		 2. addr: ���ַ
 *       3. *pKey: ����
 *       4. *pSnr: �����к�
 * ���: ��
 * ����: MI_OK
 * ����: ��
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
 * ����: userRC522_PcdRead
 * ����: ��ȡM1��һ������
 * ����: 1. addr: Ҫ���Ŀ��ַ
 * ���: 1. *p������������
 * ����: MI_OK
 * ����: ��
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
 * ����: userRC522_PcdWrite
 * ����: д���ݵ�M1��һ��
 * ����: 1. addr: Ҫ���Ŀ��ַ
 *       2. *p��Ҫд������
 * ���: ��
 * ����: MI_OK
 * ����: ��
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
 * ����: userRC522_PcdHalt
 * ����: ���Ƭ��������״̬
 * ����: ��
 * ���: ��
 * ����: MI_OK
 * ����: ��
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
 * ����: userRC522_PcdRats
 * ����: ת��APDUģʽ
 * ����: ��
 * ���: ��
 * ����: MI_OK
 * ����: ��
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
 * ����: userRC522_PcdGetChallenge
 * ����: ���������
 * ����: ��
 * ���: 1. pRan: 8�����������
 * ����: MI_OK
 * ����: ��
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
 * ����: userRC522_PcdExAuth
 * ����: �ⲿ��֤
 * ����: 1. keysign: �ⲿ��֤��Կ��ʶ��
 *       2. *pRan: 8�ֽڼ��������
 * ���: ��
 * ����: MI_OK
 * ����: ��
 ***************************************************************
 */
s8 userRC522_PcdExAuth(u8 keysign, u8 *pRan)
{
	s8 status = MI_ERR;  
	u16 unLen;
	u8 ucComMF522Buf[MAXRLEN]; 

	userRC522_ClearBitMask(Status2Reg, 0x08);	// ���У��ɹ���־�����MFCrypto1Onλ
	
	memset(ucComMF522Buf, 0x00, MAXRLEN);
	
 	userRC522_PcdSwitchPCB();
	
	ucComMF522Buf[0]  = Pcb;	
	ucComMF522Buf[1]  = 0x01;
	ucComMF522Buf[2]  = 0x00;	
	ucComMF522Buf[3]  = 0x82;				
	ucComMF522Buf[4]  = 0x00;
	ucComMF522Buf[5]  = keysign;	// ��֤�������ʶ��		
	ucComMF522Buf[6]  = 0x08;
	ucComMF522Buf[7]  = pRan[0];	// 8�ֽ������
	ucComMF522Buf[8]  = pRan[1];
	ucComMF522Buf[9]  = pRan[2];
	ucComMF522Buf[10] = pRan[3];
	ucComMF522Buf[11] = pRan[4];
	ucComMF522Buf[12] = pRan[5];
	ucComMF522Buf[13] = pRan[6];
	ucComMF522Buf[14] = pRan[7];

    userRC522_CalulateCRC(ucComMF522Buf, 15, &ucComMF522Buf[15]);	
										
	status = userRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 17, ucComMF522Buf, &unLen);	// ���յ��Ŀ�Ƭ���ͺű���
	
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
 * ����: userRC522_PcdSelectFile
 * ����: ѡ���ļ� 
 * ����: 1. *pDataIn: �ļ���ʶ����DF����
 * ���: ��
 * ����: MI_OK
 * ����: ��
 ***************************************************************
 */
s8 userRC522_PcdSelectFile(u8* pDataIn)
{
	s8 status = MI_ERR;  
	u16 unLen;
	u8 ucComMF522Buf[MAXRLEN]; 

	userRC522_ClearBitMask(Status2Reg, 0x08);	// ���У��ɹ���־�����MFCrypto1Onλ

	memset(ucComMF522Buf, 0x00, MAXRLEN);

 	userRC522_PcdSwitchPCB();
	
	ucComMF522Buf[0] = Pcb;
	ucComMF522Buf[1] = 0x01;
	ucComMF522Buf[2] = 0x00;
	ucComMF522Buf[3] = 0xA4;				
	ucComMF522Buf[4] = 0x00;	// ��������							
	ucComMF522Buf[5] = 0x00;	// ��֤�������ʶ��			
	ucComMF522Buf[6] = 0x02;									
	ucComMF522Buf[7] = pDataIn[0];	// д������
	ucComMF522Buf[8] = pDataIn[1];	
	
    userRC522_CalulateCRC(ucComMF522Buf, 9, &ucComMF522Buf[9]);	

	status = userRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 11, ucComMF522Buf, &unLen);	// ���յ��Ŀ�Ƭ���ͺű���			
	
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
	
	if ((sw[0] == 0x90) && (sw[1] == 0x00))	// �����ʲô��˼ liuchao ?
		return 1;
	else 
		return 0;
}


/*
 ***************************************************************
 * ����: userRC522_PcdReadBinary
 * ����: ��ȡ�������ļ�
 * ����: 1. offset: ƫ��
 *       2. Len
 * ���: 1. *pDataOut
 * ����: MI_OK
 * ����: ��
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
	ucComMF522Buf[4] = 0x00;		// ��ǰ�ļ�
	ucComMF522Buf[5] = offset;		// ƫ����ʼλ��
	ucComMF522Buf[6] = Len;			// ����ȡ��Ϣ����
	
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
 * ����: userRC522_PcdUpdateBinary
 * ����: д��������ļ�
 * ����: 1. offset: ���λ��Ϊ1��P1P2�������ļ���ƫ����
 *          doffset:P1��3λ100��P1��5λ�Ƕ��ļ���ʶ����P2Ϊ��ƫ����
 *       2. Len: д����Ϣ�ֽ��������64-7=57
 *       3. *pDataIn: д���ļ�����
 * ���: ��
 * ����: MI_OK
 * ����: ��
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

