#ifndef FLASH_H
#define FLASH_H
#include "sys.h"
void flashInit(void);
uint32_t readFlashID(void);
void flashEraseSector(uint32_t eraseSector);
void flashWriteBuffer(uint32_t writeAddress, uint8_t *pBuf);
void flashReadBuffer(uint32_t pageAddress,uint8_t* pBuf,uint16_t readDataNum);
/**
* application of flash
*/
enum{
    buttonMusicParameter = 0,
};
void flashRecoverParameter(rt_uint8_t *pBuf);
void flashSaveParmeter(rt_uint8_t saveParameter);
#endif
