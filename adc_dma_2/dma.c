#include "dma.h"

static DMA_Stream_TypeDef *dma = DMA2_Stream0;
static IRQn_Type dma_irq = DMA2_Stream0_IRQn;

void DMA2_Stream0_IRQHandler(void);

void dmaInit(DMA_Stream_TypeDef *dma,
						 IRQn_Type *irq,
						 uint32_t num,
						 uint32_t *mem_addr,
						 uint32_t *per_add)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	dma->CR &= ~DMA_SxCR_EN;
	dma->NDTR = num;
	dma->PAR = (uint32_t) mem_addr ;
	dma->M0AR = (uint32_t) per_add;
	dma->CR = (2 << DMA_SxCR_CHSEL_Pos)|//Channel selection = 2 (for adc3)
					  (0 << DMA_SxCR_PL_Pos)|//Priority level high
					  (0 << DMA_SxCR_DBM_Pos)|//double mode disable (nao suporta transferencias memory-memory)
					  (2 << DMA_SxCR_MSIZE_Pos)|//Memory data size = word (32-bit)
					  (2 << DMA_SxCR_PSIZE_Pos)|//Peripheral data size = word (32-bit)
					  (1 << DMA_SxCR_MINC_Pos)|//Memory increment mode = Memory address pointer is incremented after each data transfer(depends on Memory data size)
					  (0 << DMA_SxCR_PINC_Pos)|//Peripheral increment mode = disable
					  (0 << DMA_SxCR_CIRC_Pos)|//Circular mode Off
					  (0 << DMA_SxCR_DIR_Pos)|//Data transfer direction = Peripheral-to-memory
					  (1 << DMA_SxCR_TCIE_Pos);//Transfer complete interrupt able
	NVIC_EnableIRQ(dma_irq);
}

void dmaStart(void)
{
	DMA2_Stream0->CR |= DMA_SxCR_EN;
}

void dmaStop(void)
{
	DMA2_Stream0->CR &= ~DMA_SxCR_EN;
}

void DMA2_Stream0_IRQHandler(void)
{
	if(DMA2->LISR & DMA_LISR_TCIF0)
	{
		DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
	}
}
