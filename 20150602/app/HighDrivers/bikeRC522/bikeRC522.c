#include "stm32spi1.h"
#include "bikeRC522.h"
#include "delay.h"


/*
 ***************************************************************
 * ����: bikeRC522_delay_ns 
 * ����: ��ʱ
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ��
 ***************************************************************
 */
void bikeRC522_delay_ns(u32 ns)
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
 * ����: bikeRC522_ReadRawRC 
 * ����: ��RC522�Ĵ���
 * ����: 1. Address���Ĵ�����ַ
 * ���: NULL
 * ����: NULL
 * ����: ��
 ***************************************************************
 */
u8 bikeRC522_ReadRawRC(u8 Address)
{
	u8   ucAddr;
	u8   ucResult = 0;
	
	BIKERC522_NSS_0;

	ucAddr = ((Address<<1)&0x7E) | 0x80;
	
	SPI1_ReadWriteByte(ucAddr);
	ucResult = SPI1_ReadWriteByte(0x00);

	BIKERC522_NSS_1;

	return ucResult;
}


/*
 ***************************************************************
 * ����: bikeRC522_WriteRawRC 
 * ����: дRC522�Ĵ���
 * ����: 1. Address���Ĵ�����ַ
 *       2. value: д���ֵ
 * ���: NULL
 * ����: NULL
 * ����: ��
 ***************************************************************
 */
void bikeRC522_WriteRawRC(u8 Address, u8 value)
{  
    u8   ucAddr;

	BIKERC522_NSS_0;
	
    ucAddr = ((Address<<1)&0x7E);
	SPI1_ReadWriteByte(ucAddr);
	SPI1_ReadWriteByte(value);
	
	BIKERC522_NSS_1;
}


/*
 ***************************************************************
 * ����: bikeRC522_SetBitMask 
 * ����: ��RC522�Ĵ���λ
 * ����: 1. reg���Ĵ�����ַ
 *       2. mask: ��λֵ
 * ���: NULL
 * ����: NULL
 * ����: ��
 ***************************************************************
 */
void bikeRC522_SetBitMask(u8 reg, u8 mask)  
{
    u8 tmp = 0x0;
	
    tmp = bikeRC522_ReadRawRC(reg);
    bikeRC522_WriteRawRC(reg, tmp | mask);  // set bit mask
}


/*
 ***************************************************************
 * ����: bikeRC522_ClearBitMask 
 * ����: ��RC522�Ĵ���λ
 * ����: 1. reg���Ĵ�����ַ
 *       2. mask: ��λֵ
 * ���: NULL
 * ����: NULL
 * ����: ��
 ***************************************************************
 */
void bikeRC522_ClearBitMask(u8 reg, u8 mask)  
{
    u8 tmp = 0x0;
	
    tmp = bikeRC522_ReadRawRC(reg);
    bikeRC522_WriteRawRC(reg, tmp & (~mask));  // clear bit mask
} 


/*
 ***************************************************************
 * ����: bikeRC522_PcdAntennaOn 
 * ����: ��������  
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
 ***************************************************************
 */
void bikeRC522_PcdAntennaOn(void)
{
    u8 i;
   
    i = bikeRC522_ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        bikeRC522_SetBitMask(TxControlReg, 0x03);
    }
}


/*
 ***************************************************************
 * ����: bikeRC522_PcdAntennaOff 
 * ����: �ر�����  
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
 ***************************************************************
 */
void bikeRC522_PcdAntennaOff(void)
{
	bikeRC522_ClearBitMask(TxControlReg, 0x03);
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
u8 bikeRC522_PcdReset(void)
{
	BIKERC522_RST_1;
    bikeRC522_delay_ns(10);
	BIKERC522_RST_0;
    bikeRC522_delay_ns(10);
	BIKERC522_RST_1;
    bikeRC522_delay_ns(10);
    bikeRC522_WriteRawRC(CommandReg, PCD_RESETPHASE);
	bikeRC522_WriteRawRC(CommandReg, PCD_RESETPHASE);
    bikeRC522_delay_ns(10);
    
    bikeRC522_WriteRawRC(ModeReg, 0x3D);            //��Mifare��ͨѶ��CRC��ʼֵ0x6363
    bikeRC522_WriteRawRC(TReloadRegL, 30);           
    bikeRC522_WriteRawRC(TReloadRegH, 0);
    bikeRC522_WriteRawRC(TModeReg, 0x8D);
    bikeRC522_WriteRawRC(TPrescalerReg, 0x3E);	
	bikeRC522_WriteRawRC(TxAutoReg, 0x40);//����Ҫ
   
    return MI_OK;
}


/*
 ***************************************************************
 * ����: bikeRC522_M500PcdConfigISOType
 * ����: ����RC522�Ĺ�����ʽ 
 * ����: 1. type : ѡ��RC522������ʽ
 * ���: NULL
 * ����: NULL
 * ����: ��
 ***************************************************************
 */
u8 bikeRC522_M500PcdConfigISOType(u8 type)
{
	if (type == 'A')	// ISO14443_A
	{ 
		bikeRC522_ClearBitMask(Status2Reg, 0x08);
		bikeRC522_WriteRawRC(ModeReg, 0x3D);//3F
		bikeRC522_WriteRawRC(RxSelReg, 0x86);//84
		bikeRC522_WriteRawRC(RFCfgReg, 0x7F);   //4F
		bikeRC522_WriteRawRC(TReloadRegL, 30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
		bikeRC522_WriteRawRC(TReloadRegH, 0);
		bikeRC522_WriteRawRC(TModeReg, 0x8D);
		bikeRC522_WriteRawRC(TPrescalerReg, 0x3E);
		bikeRC522_delay_ns(1000);
		bikeRC522_PcdAntennaOn();
	}
	else
	{ 
		return 1; 
	}
	
	return MI_OK;
}


/*
 ***************************************************************
 * ����: bikeRC522_InitRc522
 * ����: ��ʼ�������г���ǩ��RC522
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ��
 ***************************************************************
 */
void bikeRC522_InitRc522(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;     //NRST
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	SPI1_Init();
	
	bikeRC522_PcdReset();
	bikeRC522_PcdAntennaOff();
	delay_ms(2);  
	bikeRC522_PcdAntennaOn();
	bikeRC522_M500PcdConfigISOType('A');
}


/*
 ***************************************************************
 * ����: bikeRC522_Reset_RC522
 * ����: ��λ�����г���ǩ��RC522����������
 * ����: NULL
 * ���: NULL
 * ����: NULL
 * ����: ��
 ***************************************************************
 */
void bikeRC522_Reset_RC522(void)
{
	bikeRC522_PcdReset();
	bikeRC522_PcdAntennaOff();
	delay_ms(2);  
	bikeRC522_PcdAntennaOn();
}


/*
 ***************************************************************
 * ����: bikeRC522_CalulateCRC
 * ����: ��MF522����CRC16����
 * ����: 1. *pIn: ��������
 *       2. len: ����
 * ���: 1. *pOut: �������
 * ����: NULL
 * ����: ��
 ***************************************************************
 */
void bikeRC522_CalulateCRC(u8 *pIn, u8 len, u8 *pOut)
{
    u8   i, n;
   
    bikeRC522_ClearBitMask(DivIrqReg, 0x04);
    bikeRC522_WriteRawRC(CommandReg, PCD_IDLE);
    bikeRC522_SetBitMask(FIFOLevelReg, 0x80);
 
    for (i = 0; i < len; i++)
    {   
		bikeRC522_WriteRawRC(FIFODataReg, *(pIn +i));   
	}
    
	bikeRC522_WriteRawRC(CommandReg, PCD_CALCCRC);
   
    i = 0xFF;
    do 
    {
        n = bikeRC522_ReadRawRC(DivIrqReg);
        i--;
    } while ((i!=0) && !(n&0x04));
  
    pOut[0] = bikeRC522_ReadRawRC(CRCResultRegL);
    pOut[1] = bikeRC522_ReadRawRC(CRCResultRegM);
}


/*
 ***************************************************************
 * ����: bikeRC522_PcdComMF522
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
char bikeRC522_PcdComMF522(u8 Command, 
                 u8 *pIn, 
                 u8 InLenByte,
                 u8 *pOut, 
                 u8 *pOutLenBit)
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
   
    bikeRC522_WriteRawRC(ComIEnReg, irqEn|0x80);
    bikeRC522_ClearBitMask(ComIrqReg, 0x80);	//�������ж�λ
    bikeRC522_WriteRawRC(CommandReg, PCD_IDLE);
    bikeRC522_SetBitMask(FIFOLevelReg, 0x80);	 	//��FIFO����
    
    for (i = 0; i < InLenByte; i++)
    {   
		bikeRC522_WriteRawRC(FIFODataReg, pIn [i]);    
	}
    	
	bikeRC522_WriteRawRC(CommandReg, Command);	  

	n = bikeRC522_ReadRawRC(CommandReg);
    
    if (Command == PCD_TRANSCEIVE)	// ��ʼ����
    {    
		bikeRC522_SetBitMask(BitFramingReg, 0x80);  
	}	 
    										 
    //i = 600;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
	i = 2000;
    do 
    {
        n = bikeRC522_ReadRawRC(ComIrqReg);
        i--;
    } while ((i!=0) && !(n&0x01) && !(n&waitFor));
  
    bikeRC522_ClearBitMask(BitFramingReg, 0x80);

    if (i != 0)
    {    
        if (!(bikeRC522_ReadRawRC(ErrorReg)&0x1B))
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   
				status = MI_NOTAGERR;   
			}
            if (Command == PCD_TRANSCEIVE)
            {
               	n = bikeRC522_ReadRawRC(FIFOLevelReg);
              	lastBits = bikeRC522_ReadRawRC(ControlReg) & 0x07;
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
					pOut [i] = bikeRC522_ReadRawRC(FIFODataReg);    
				}
            }
        }
        else
        {   
			status = MI_ERR;   
		}
        
    }

    bikeRC522_SetBitMask(ControlReg, 0x80);           // stop timer now
    bikeRC522_WriteRawRC(CommandReg, PCD_IDLE); 
    
	return status;
}


/*
 ***************************************************************
 * ����: bikeRC522_PcdRequest
 * ����: Ѱ��
 * ����: 1. req_code: Ѱ����ʽ
 *		    0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�
 * 		    0x26 = Ѱδ��������״̬�Ŀ�
 * ���: 1. *pTagType: ��Ƭ���ʹ���
 *        	0x4400 = Mifare_UltraLight
 *        	0x0400 = Mifare_One(S50)
 *      	0x0200 = Mifare_One(S70)
 *     		0x0800 = Mifare_Pro(X)
 *      	0x4403 = Mifare_DESFire
 * ����: ״̬
 * ����: ��
 ***************************************************************
 */
u8 bikeRC522_PcdRequest(u8 req_code, u8 *pTagType)
{
	u8 status;  
	u8 unLen;
	u8 ucComMF522Buf[MAXRLEN]; 

	bikeRC522_ClearBitMask(Status2Reg, 0x08);
	bikeRC522_WriteRawRC(BitFramingReg, 0x07);
	bikeRC522_SetBitMask(TxControlReg, 0x03);
 
	ucComMF522Buf[0] = req_code;

	status = bikeRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 1, ucComMF522Buf, &unLen);

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
 * ����: bikeRC522_PcdAnticoll
 * ����: ����ͻ
 * ����: 1. *pSnr: ��Ƭ���кţ�4�ֽ�
 * ���: ��
 * ����: MI_OK
 * ����: ��
 ***************************************************************
 */
u8 bikeRC522_PcdAnticoll(u8 *pSnr)
{
    u8 status;
    u8 i, snr_check = 0;
    u8 unLen;
    u8 ucComMF522Buf[MAXRLEN]; 
    
    bikeRC522_ClearBitMask(Status2Reg, 0x08);
    bikeRC522_WriteRawRC(BitFramingReg, 0x00);
    bikeRC522_ClearBitMask(CollReg, 0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = bikeRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, &unLen);

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
    
    bikeRC522_SetBitMask(CollReg, 0x80);
  
    return status;
}


/*
 ***************************************************************
 * ����: bikeRC522_PcdSelect
 * ����: ѡ����Ƭ
 * ����: 1. *pSnr: ��Ƭ���кţ�4�ֽ�
 * ���: ��
 * ����: MI_OK
 * ����: ��
 ***************************************************************
 */
u8 bikeRC522_PcdSelect(u8 *pSnr)
{
    u8 status;
    u8 i;
    u8 unLen;
    u8 ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    
	for (i = 0; i < 4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }

    bikeRC522_CalulateCRC(ucComMF522Buf, 7, &ucComMF522Buf[7]);
  
    bikeRC522_ClearBitMask(Status2Reg, 0x08);

    status = bikeRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 9, ucComMF522Buf, &unLen);
    
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
 * ����: bikeRC522_PcdAuthState
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
u8 bikeRC522_PcdAuthState(u8 auth_mode, u8 addr, u8 *pKey, u8 *pSnr)
{
    u8 status;
    u8 unLen;
    u8 ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;

    memcpy(&ucComMF522Buf[2], pKey, 6); 
    memcpy(&ucComMF522Buf[8], pSnr, 4); 
    
    status = bikeRC522_PcdComMF522(PCD_AUTHENT, ucComMF522Buf, 12, ucComMF522Buf, &unLen);
    if ((status != MI_OK) || (!(bikeRC522_ReadRawRC(Status2Reg) & 0x08)))
    {   
		status = MI_ERR;   
	}
		
    return status;
}


/*
 ***************************************************************
 * ����: bikeRC522_PcdRead
 * ����: ��ȡM1��һ������
 * ����: 1. addr: Ҫ���Ŀ��ַ
 * ���: 1. *p������������
 * ����: MI_OK
 * ����: ��
 ***************************************************************
 */
u8 bikeRC522_PcdRead(u8 addr, u8 *p)
{
    u8 status;
    u8 unLen;
    u8 i, ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    bikeRC522_CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);
   
    status = bikeRC522_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
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
 * ����: bikeRC522_PcdWrite
 * ����: д���ݵ�M1��һ��
 * ����: 1. addr: Ҫ���Ŀ��ַ
 *       2. *p��Ҫд������
 * ���: ��
 * ����: MI_OK
 * ����: ��
 ***************************************************************
 */
u8 bikeRC522_PcdWrite(u8 addr, u8 *p)
{
    u8 status;
    u8 unLen;
    u8 i, ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    bikeRC522_CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);
 
    status = bikeRC522_PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

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
		
        bikeRC522_CalulateCRC(ucComMF522Buf, 16, &ucComMF522Buf[16]);

        status = bikeRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 18, ucComMF522Buf, &unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   
			status = MI_ERR;   
		}
    }
    
    return status;
}


/*
 ***************************************************************
 * ����: bikeRC522_PcdHalt
 * ����: ���Ƭ��������״̬
 * ����: ��
 * ���: ��
 * ����: MI_OK
 * ����: ��
 ***************************************************************
 */
u8 bikeRC522_PcdHalt(void)
{
    u8 status;
    u8 unLen;
    u8 ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    bikeRC522_CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);
 
    status = bikeRC522_PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);

    return status;
}

