#ifndef __TIM__
#define __TIM__

#include "stm32f4xx.h"                  
#include "stdint.h"

void timInit(uint32_t *psc, uint32_t *arr, uint32_t *high_time);
void timStart(void);
void timStop(void);

#endif