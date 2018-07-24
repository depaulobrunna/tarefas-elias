#include "stm32f4xx.h"
#include <string.h>

static int8_t strg_abc[] = "dont-care";
static int8_t strg_zyx[] = "i-really";

int main(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
    DMA2_Stream0->CR &= ~DMA_SxCR_EN;
    DMA2_Stream0->NDTR = sizeof(strg_abc);
		DMA2_Stream0->PAR = (uint32_t)((uint32_t *)&strg_abc[0]);
		DMA2_Stream0->M0AR = (uint32_t)((uint32_t *)&strg_zyx[0]);
    DMA2_Stream0->CR = (0 << DMA_SxCR_CHSEL_Pos)|//Channel selection
                       (2 << DMA_SxCR_PL_Pos)|//Priority level High
                       (0 << DMA_SxCR_DBM_Pos)|//double mode disable (nao suporta transferencias memory-memory)
                       (0 << DMA_SxCR_MSIZE_Pos)|//Memory data size = Byte (8-bit)
                       (1 << DMA_SxCR_MINC_Pos)|//Memory increment mode = Memory address pointer is incremented after each data transfer
                       (1 << DMA_SxCR_PINC_Pos)|//Memory increment mode = Memory address pointer is incremented after each data transfer
                       (2 << DMA_SxCR_DIR_Pos)|//Data transfer direction = Memory-to-memory
                       (0 << DMA_SxCR_TCIE_Pos);//Transfer complete interrupt disable
    DMA2_Stream0->CR |= DMA_SxCR_EN;
	
	while(1);
}
