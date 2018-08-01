#include "stm32f4xx.h"                  // Device header

#define NUM_AMOST				4096

void ADC_IRQHandler(void);
void DMA2_Stream0_IRQHandler(void);

volatile uint32_t i = 0;
volatile uint32_t data[NUM_AMOST];

ADC_TypeDef *adc = ADC3;
GPIO_TypeDef *gpio = GPIOA;

int main(void)
{
	//led cfg
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= GPIO_MODER_MODER12_0|GPIO_MODER_MODER13_0|GPIO_MODER_MODER14_0; //12gr 13og
	GPIOD->ODR &= ~(GPIO_ODR_OD12|GPIO_ODR_OD13|GPIO_ODR_OD14);
# if 1
	//dma cfg
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	DMA2_Stream0->CR &= ~DMA_SxCR_EN;
	DMA2_Stream0->NDTR = sizeof(data);
	DMA2_Stream0->PAR = (uint32_t)((uint32_t *)&adc->DR);
	DMA2_Stream0->M0AR = (uint32_t)((uint32_t *)&data[0]);
	DMA2_Stream0->CR = (2 << DMA_SxCR_CHSEL_Pos)|//Channel selection = 2 (for adc3)
										 (2 << DMA_SxCR_PL_Pos)|//Priority level High
										 (0 << DMA_SxCR_DBM_Pos)|//double mode disable (nao suporta transferencias memory-memory)
										 (2 << DMA_SxCR_MSIZE_Pos)|//Memory data size = word (32-bit)
										 (2 << DMA_SxCR_PSIZE_Pos)|//Peripheral data size = word (32-bit)
										 (1 << DMA_SxCR_MINC_Pos)|//Memory increment mode = Memory address pointer is incremented after each data transfer
										 (0 << DMA_SxCR_PINC_Pos)|//Memory increment mode = disable
										 (0 << DMA_SxCR_DIR_Pos)|//Data transfer direction = Peripheral-to-memory
										 (1 << DMA_SxCR_TCIE_Pos);//Transfer complete interrupt able
  DMA2_Stream0->CR |= DMA_SxCR_EN;
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
	
#endif
	//adc pin cfg
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= (3 << GPIO_MODER_MODE0_Pos);//PA 0 analog mode
	
	//adc cfg
	RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;
	adc->CR2 &= ~ADC_CR2_ADON; 
	adc->CR1 |= (0 << ADC_CR1_RES_Pos)| //ADC 12BITS 
						  (1 << ADC_CR1_EOCIE_Pos); // INTERRUPT ABLE
	adc->SQR1 |= (0 << ADC_SQR1_L_Pos); //1 CONV
	adc->SQR3 |= (0 << ADC_SQR3_SQ1_Pos); //CHANEL 0
	adc->SMPR2 = (0 << ADC_SMPR2_SMP1_Pos); //SAMMPLING TIME 
	adc->CR2 |= (0 << ADC_CR2_CONT_Pos)| //continuous oFF
							(1 << ADC_CR2_EXTEN_Pos)| //EXTEN RISING EDGE
							(3 << ADC_CR2_EXTSEL_Pos)| //EXSEL TIMER CANAL 2 EVENT
							(1 << ADC_CR2_EOCS_Pos)| // End of conversion selection = the EOC bit is set at the end of each regular conversion
							(1 << ADC_CR2_DMA_Pos)|//Direct memory access mode DISABLE
							(1 << ADC_CR2_DDS_Pos);//DMA requests are issued as long as data are converted and DMA=1  DISABLE
	NVIC_EnableIRQ(ADC_IRQn);

	//confg pin timmer 
	#if 1
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	gpio-> MODER |= GPIO_MODER_MODER1_1; //PA1 ALTERNATE FUNC
	gpio->AFR[0] |= 1 << GPIO_AFRL_AFSEL1_Pos; 
	gpio->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED1_Pos;
	#endif
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	TIM2->PSC = 1000 - 1;//VALOR PELO QUAL O CLOCK INICIAL É DIVIDIDO
	TIM2->ARR = 16 - 1;
	
	TIM2->CCMR1 = (TIM2->CCMR1 & ~TIM_CCMR1_OC2M_Pos)| (6 << TIM_CCMR1_OC2M_Pos);
	TIM2->CR1 |= TIM_CR1_ARPE;
	TIM2->CCR2 = 8 - 1;//tempo alto
	TIM2->CCER = (TIM2->CCER & ~TIM_CCER_CC2E)| (1 << TIM_CCER_CC2E_Pos);
	
	adc->CR2 |= ADC_CR2_ADON;
	TIM2->CR1 |= TIM_CR1_CEN;

	
	while(1)
	{
	#if 0
		i = adc->DR;
		while(!(ADC3->SR & ADC_SR_EOC))
		{ 
			GPIOD->ODR ^= GPIO_ODR_OD14;//red
		}
	#endif
	}
}

void ADC_IRQHandler(void)
{
	//An interrupt can be produced on the end of conversion adc
	GPIOD->ODR ^= GPIO_ODR_OD13;//og
	
}
void DMA2_Stream0_IRQHandler(void)
{
	//Transfer complete dma
	GPIOD->ODR ^= GPIO_ODR_OD12;//gr
}
