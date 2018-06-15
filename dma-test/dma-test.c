#include "stm32f4xx.h"                  // Device header

#define BL GPIO_ODR_OD15

char strg_abc[3] = "abc";
char strg_zzz[3] = "zzz";

char *address_abc = &strg_abc[0];
char *address_zzz = &strg_zzz[0];

int main(void)
{
	static uint32_t num = 5;
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER = (GPIOD->MODER & ~(GPIO_MODER_MODER14))|
								 (1 << GPIO_MODER_MODER15_Pos);
	
	
	//DMA CFG
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	DMA1_Stream0->CR = (1 << DMA_SxCR_DBM_Pos)|
										 (0 << DMA_SxCR_MSIZE_Pos)|
										 (1 << DMA_SxCR_MINC_Pos)|
										 (2 << DMA_SxCR_DIR_Pos)|
										 (0 << DMA_SxCR_TCIE_Pos);
	DMA1_Stream0->NDTR = 3;
	DMA1_Stream0->M0AR = (uint32_t)address_abc;
	DMA1_Stream0->M1AR = (uint32_t)address_zzz;
	
	while((DMA1_Stream0->CR & DMA_SxCR_EN_Msk) == 0 && num--)
	{
		DMA1_Stream0->CR |= DMA_SxCR_EN;
	}
	
	if(num == 0)
	{
		while(1);
	}
		
	while(DMA1->LIFCR & ~DMA_LIFCR_CTEIF0);
	DMA1_Stream0->CR &= (~DMA_SxCR_EN_Msk);
	while(1);
	
}
