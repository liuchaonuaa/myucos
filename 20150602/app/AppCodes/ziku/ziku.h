#ifndef __ZIKU_H
#define __ZIKU_H

void eraseZikuFlash(void);
void WriteCharToFlash(void);
void ziku_Write(u16 index, u16 datalen, u8 *data);

#endif
