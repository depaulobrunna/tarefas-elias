#include "stm32f4xx.h"
#include "dma.h"
#include "tim.h"
#include "adc.h"

#define NUM_AMOST 100

static inline void startProcess(void);
static inline void stopProcess(void);

uint32_t i;
volatile uint32_t data[NUM_AMOST];
volatile uint32_t size = 0;
volatile uint32_t overrun = 0;

int main(void)
{
	
	
	
	dmaInit(NUM_AMOST, (uint32_t *)&ADC3->DR, (uint32_t *)&data[0]);
	timInit((uint32_t) 1000,(uint32_t) 16,(uint32_t) 8);
	adcInit((uint8_t) 0);
	while(1)
	{
		startProcess();
		while(transfState != 1);
		stopProcess();
	}
	
}

static inline void startProcess(void)
{
	dmaStart();
	adcStart();
	timStart();
}

static inline void stopProcess(void)
{
	dmaStop();
	adcStop();
	timStop();
}
