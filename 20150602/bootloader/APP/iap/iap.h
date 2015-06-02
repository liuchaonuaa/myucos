#ifndef __IAP_H
#define __IAP_H


/*----------------- 字库申请(0xA0) -----------------*/
__packed typedef struct {
	u16  index;			// 第几帧
	u16  len;			// 这一帧数据的长度
	u8   data[1024];
	u32  crc;	
} Program_Struct;

void IAP_WriteSTMFLASH(u16 index, u16 datalen, u8 *data);
void IAP_WRITE(void);

#endif
