#include "adc.h"
#include "stm32f4xx.h"                  



ADC_TypeDef *adc = ADC3;

void adcInit(uint8_t res)
{
	//adc pin cfg
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= (3 << GPIO_MODER_MODE0_Pos);//PA 0 analog mode
	
	//adc cfg
	RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;
	adc->CR2 |= ADC_CR2_ADON; 
	adc->CR1 |= ((uint8_t)res << ADC_CR1_RES_Pos)| //ADC resolution
							(1 << ADC_CR1_DISCEN_Pos);//| //Discontinuous mode on regular channels
	adc->SQR1 |= (0 << ADC_SQR1_L_Pos); //1 CONV
	adc->SQR3 |= (0 << ADC_SQR3_SQ1_Pos); //CHANEL 0
	adc->SMPR2 = (7 << ADC_SMPR2_SMP1_Pos); //SAMMPLING TIME 
	adc->CR2 |= (1 << ADC_CR2_EXTEN_Pos)| //External trigger detection on the rising edge
							(3 << ADC_CR2_EXTSEL_Pos)| //EXSEL TIMER CANAL 2 EVENT
							(1 << ADC_CR2_DMA_Pos);//Direct memory access mode ABLE
}

void adcStart(void)
{
	adc->CR2 |= ADC_CR2_SWSTART;
}
void adcStop (void)
{
	adc->CR2 &= ~ADC_CR2_SWSTART;
}
