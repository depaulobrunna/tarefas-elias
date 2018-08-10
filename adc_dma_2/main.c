#include "stm32f4xx.h"
#include "dma.h"
#include "tim.h"
#include "adc.h"

#define NUM_AMOST 4096
void DMA2_Stream0_IRQHandler(void);

static inline void startProcess(void);
static inline void stopProcess(void);


volatile uint32_t data[NUM_AMOST];
volatile uint32_t size = 0;
volatile uint32_t overrun = 0;

int main(void)
{
	
	
	
	
	timInit((uint32_t *) 1000,(uint32_t *) 16,(uint32_t *) 8);
	startProcess();
	while(1);
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
