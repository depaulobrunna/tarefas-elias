#ifndef __ADC__
#define __ADC__

#include "stm32f4xx.h"
#include "stdint.h"

void adcInit(uint8_t res);
void adcStart(void);
void adcStop(void);
#endif
