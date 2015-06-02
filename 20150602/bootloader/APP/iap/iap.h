#ifndef __IAP_H
#define __IAP_H


/*----------------- �ֿ�����(0xA0) -----------------*/
__packed typedef struct {
	u16  index;			// �ڼ�֡
	u16  len;			// ��һ֡���ݵĳ���
	u8   data[1024];
	u32  crc;	
} Program_Struct;

void IAP_WriteSTMFLASH(u16 index, u16 datalen, u8 *data);
void IAP_WRITE(void);

#endif
