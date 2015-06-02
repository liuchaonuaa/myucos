/*******************************************************************************
** �ļ���: 		upload.c
** �汾��  		1.0
** ��������: 	RealView MDK-ARM 4.14
** ����: 		wuguoyana
** ��������: 	2011-04-29
** ����:		�ϴ��ļ�����غ���
** ����ļ�:	common.h
** �޸���־��	2011-04-29   �����ĵ�
*******************************************************************************/

/* ����ͷ�ļ� *****************************************************************/
#include "common.h"

/*******************************************************************************
  * @��������	SerialUpload
  * @����˵��   ͨ�������ϴ�һ���ļ�
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void SerialUpload(void)
{
    uint32_t status = 0;

    SerialPutString("\n\n\rSelect Receive File ... (press any key to abort)\n\r");

    if (GetKey() == CRC16)
    {
        //ͨ��ymodemЭ���ϴ�����
        status = Ymodem_Transmit((uint8_t*)ApplicationAddress, (const uint8_t*)"UploadedFlashImage.bin", FLASH_IMAGE_SIZE);

        if (status != 0)
        {
            SerialPutString("\n\rError Occured while Transmitting File\n\r");
        }
        else
        {
            SerialPutString("\n\rFile Trasmitted Successfully \n\r");
        }
    }
    else
    {
        SerialPutString("\r\n\nAborted by user.\n\r");
    }

}

/*******************************�ļ�����***************************************/
