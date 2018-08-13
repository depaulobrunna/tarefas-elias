#ifndef __DMA_H__
#define __DMA_H__

#include "stm32f4xx.h"
#include "stdint.h"

void dmaInit(uint32_t num,
						 uint32_t *mem_addr,
						 uint32_t *per_add);
void dmaStart(void);
void dmaStop(void);

#endif
