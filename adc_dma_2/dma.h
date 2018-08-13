#ifndef __DMA_H__
#define __DMA_H__

#include "stm32f4xx.h"
#include "stdint.h"

void dmaInit(uint32_t num, uint32_t *per_add, uint32_t *mem_addr);
void dmaStart(void);
void dmaStop(void);

extern volatile uint8_t transfState;

#endif
