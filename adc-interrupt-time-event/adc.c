#include "stm32f4xx.h"                  // Device header

#define NUM_AMOST				4096

void ADC_IRQHandler(void);

volatile uint32_t data_ad[NUM_AMOST];
volatile uint32_t *per_adress = &ADC3->DR;
volatile uint32_t *memory_adress = &data_ad[0];
uint16_t data;

ADC_TypeDef *adc = ADC3;
GPIO_TypeDef *gpio = GPIOA;
DMA_Stream_TypeDef *dma_stream = DMA2_Stream0;

int main(void)
{
#if 1
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER = (GPIOD->MODER & ~GPIO_MODER_MODE13)|(1 << GPIO_MODER_MODE13_Pos)|(1 << GPIO_MODER_MODE14_Pos);
	GPIOD->ODR = (GPIOD->ODR & ~GPIO_ODR_OD13);
	
//	per_adress = &ADC3->DR;
//	memory_adress = &data_ad[0];
	//dma cfg
	RCC->AHB1ENR = (RCC->AHB1ENR & ~RCC_AHB1ENR_DMA2EN)|(1 << RCC_AHB1ENR_DMA2EN_Pos);
	dma_stream->CR = (2 << DMA_SxCR_CHSEL_Pos)|//Channel selection=channel 2
										 (2 << DMA_SxCR_PL_Pos)|
										 (2 << DMA_SxCR_MSIZE_Pos)|//Memory data size=32-bit
										 (2 << DMA_SxCR_PSIZE_Pos)|//Peripheral data size=32-bit
										 (1 << DMA_SxCR_MINC_Pos)|//Peripheral-to-memory=Memory address pointer is incremented 
										 (0 << DMA_SxCR_DIR_Pos)|//Data transfer direction=Peripheral-to-memory
										 (1 << DMA_SxCR_TCIE_Pos)|//Transfer complete interrupt enable
										 (0 << DMA_SxCR_EN_Pos);
	dma_stream->NDTR = 4096;//Number of data items to transfer
	dma_stream->M0AR = (uint32_t)memory_adress;
	dma_stream->PAR = (uint32_t)per_adress;
	
	dma_stream->CR = (1 << DMA_SxCR_EN_Pos);
	
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
#endif
	//GPIO CFG(ANALOG MODE)
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE0) | (3 << GPIO_MODER_MODE0_Pos);//PA 0 analog mode
	
	//adc cfg
	RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;
	adc->CR2 = (adc->CR2 & ~ADC_CR2_ADON)|(0 << ADC_CR2_ADON);
    
	adc->CR1 = (adc->CR1 & ~(ADC_CR1_RES|ADC_CR1_AWDEN))|(2 << ADC_CR1_RES_Pos)|(1 << ADC_CR1_EOCIE_Pos);// ADC 8BITS & INTERRUPT ABLE
	adc->SQR1 = (adc->SQR1 & ~ADC_SQR1_L)|(0 << ADC_SQR1_L_Pos);//1 CONV
	adc->SQR3 = (adc->SQR3 & ~ADC_SQR3_SQ1)|(0 << ADC_SQR3_SQ1_Pos);//CHANEL 0
	adc->SMPR2 = (adc->SMPR2 & ~ADC_SMPR2_SMP1)|(8 << ADC_SMPR2_SMP1_Pos);//SAMMPLING TIME
	adc->CR2 = 	(adc->CR2 & ~(ADC_CR2_CONT|ADC_CR2_ADON))|
							((1 << ADC_CR2_CONT_Pos)|
							(1 << ADC_CR2_ADON_Pos)|
							(1 << ADC_CR2_EXTEN_Pos)|
							(3 << ADC_CR2_EXTSEL_Pos)|//continuous mode on & TURN ON ADC & EXTEN RISING EDGE &EXSEL TIMER CANAL 2 EVENT
							(1 << ADC_CR2_DDS_Pos)|
							(1 << ADC_CR2_DMA_Pos));
	NVIC_EnableIRQ(ADC_IRQn);


	//confg pin timmer 
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	gpio-> MODER |= GPIO_MODER_MODER1_1;
	gpio->AFR[0] |= 1 << GPIO_AFRL_AFSEL1_Pos;
	gpio->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED1_Pos;
	
	//confg pin timmer 
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	TIM2->PSC = 1000 - 1;//VALOR PELO QUAL O CLOCK INICIAL É DIVIDIDO
	TIM2->ARR = 16 - 1;
	
	TIM2->CCMR1 = (TIM2->CCMR1 & ~TIM_CCMR1_OC2M_Pos)| (6 << TIM_CCMR1_OC2M_Pos);
	TIM2->CR1 |= TIM_CR1_ARPE;
	TIM2->CCR2 = 8 - 1;//tempo alto
	TIM2->CCER = (TIM2->CCER & ~TIM_CCER_CC2E)| (1 << TIM_CCER_CC2E_Pos);
	
	adc->CR2 = 	(adc->CR2 & ~ADC_CR2_ADON)|(1 << ADC_CR2_ADON_Pos);
	TIM2->CR1 |= TIM_CR1_CEN;

	while(1)
	{
		data = adc->DR;
		
	}
}

void ADC_IRQHandler(void)
{
	GPIOD->ODR ^= GPIO_ODR_OD13;
	
	//adc->SR &= ~ADC_SR_EOC; 
}