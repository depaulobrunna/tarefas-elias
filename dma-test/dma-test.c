#include "stm32f4xx.h"                  // Device header

#define BL GPIO_ODR_OD15
#define strg_size 26


char strg_abc[strg_size] = "abcdefghijklmnopqrstuvwxyz";
char strg_zyx[strg_size] = "zyxwvutsrqponmlkjihgfedcba";


char *address_abc = &strg_abc[0];
char *address_zyx = &strg_zyx[0];

int main(void)
{
	static uint32_t num = 5;
	
 	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER = (GPIOD->MODER & ~(GPIO_MODER_MODER15))|
								 (1 << GPIO_MODER_MODER15_Pos);
	
	
	//DMA CFG
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	DMA2_Stream0->CR = (0 << DMA_SxCR_DBM_Pos)|//double mode disable (nao suporta transferencias memory-memory)
										 (0 << DMA_SxCR_MSIZE_Pos)|//Memory data size = Byte (8-bit)
										 (1 << DMA_SxCR_MINC_Pos)|//Memory increment mode = Memory address pointer is incremented after each data transfer
										 (2 << DMA_SxCR_DIR_Pos)|//Data transfer direction = Memory-to-memory
										 (0 << DMA_SxCR_TCIE_Pos);//Transfer complete interrupt disable
	DMA2_Stream0->NDTR = strg_size;
	DMA2_Stream0->M0AR = (uint32_t)address_abc;
	DMA2_Stream0->M1AR = (uint32_t)address_zyx;
	DMA2_Stream0->CR &= ~DMA_SxCR_EN;
 #if 1	
	
	while(DMA2_Stream0->NDTR != 0)
	{
		GPIOD->ODR ^= GPIO_ODR_OD15;
		
	}
 
 #else
	while((DMA2_Stream0->CR & DMA_SxCR_EN_Msk) == 0 && num--)
	{
		DMA2_Stream0->CR |= DMA_SxCR_EN;
	}
	
	if(num == 0)
	{
		while(1);
	}
		
	while(DMA2->LIFCR & ~DMA_LIFCR_CTEIF0);
	DMA2_Stream0->CR &= (~DMA_SxCR_EN_Msk);
	while(1);
#endif
}
