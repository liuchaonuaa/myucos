/*******************************************************************************
** �ļ���: 		download.c
** �汾��  		1.0
** ��������: 	RealView MDK-ARM 4.14
** ����: 		wuguoyana
** ��������: 	2011-04-29
** ����:		�ȴ��û�ѡ�����ļ�����,���߷��������Լ�һЩ��ʾ��Ϣ��
                ������ʵ�ִ��͵���ymodem��cԴ�ļ���
** ����ļ�:	common.h
** �޸���־��	2011-04-29   �����ĵ�
*******************************************************************************/

/* ����ͷ�ļ� *****************************************************************/
#include "common.h"

/* �������� ------------------------------------------------------------------*/
extern uint8_t file_name[FILE_NAME_LENGTH];
extern u8 ID_write;

uint8_t tab_1024[1024] =
{
    0
};

/*******************************************************************************
  * @��������	SerialDownload
  * @����˵��   ͨ�����ڽ���һ���ļ�
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void SerialDownload(void)
{
    uint8_t Number[10] = "          ";
    int32_t Size = 0;

    SerialPutString("Waiting for the file to be sent ... (press 'a' to abort)\n\r");
    Size = Ymodem_Receive(&tab_1024[0]);
  
	if (Size > 0)
    {
        SerialPutString("\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
        SerialPutString(file_name);
        Int2Str(Number, Size);
        SerialPutString("\n\r Size: ");
        SerialPutString(Number);
        SerialPutString(" Bytes\r\n");
        SerialPutString("-------------------\n");
		ID_write = 1;
		
		while (ID_write)
		{
			FlashWrite(FLASH_ADR_1, 0x5a);;
			ID_write = 0;						
		}		
    }
    else if (Size == -1)
    {
        SerialPutString("\n\n\rThe image size is higher than the allowed space memory!\n\r");
    }
    else if (Size == -2)
    {
        SerialPutString("\n\n\rVerification failed!\n\r");
    }
    else if (Size == -3)
    {
        SerialPutString("\r\n\nAborted by user.\n\r");
    }
    else
    {
        SerialPutString("\n\rFailed to receive the file!\n\r");
    }
}

/*******************************�ļ�����***************************************/
