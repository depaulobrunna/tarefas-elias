#include "stm32f4xx.h"                  // Device header

#define NUM_AMOST				4096

void ADC_IRQHandler(void);

volatile uint32_t i = 0;
volatile uint32_t data[NUM_AMOST];

ADC_TypeDef *adc = ADC3;
GPIO_TypeDef *gpio = GPIOA;

int main(void)
{
	//led cfg
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= GPIO_MODER_MODER12_0;
	GPIOD->ODR &= ~GPIO_ODR_OD12;
	
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
	adc->SMPR2 |= (8 << ADC_SMPR2_SMP1_Pos); //SAMMPLING TIME
	adc->CR2 |= (1 << ADC_CR2_CONT_Pos)| //continuous mode on
							(1 << ADC_CR2_ADON_Pos)| //TURN ON ADC
							(1 << ADC_CR2_EXTEN_Pos)| //EXTEN RISING EDGE
							(3 << ADC_CR2_EXTSEL_Pos); //EXSEL TIMER CANAL 2 EVENT
	
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
	
	adc->CR2 |= ADC_CR2_ADON_Pos;
	TIM2->CR1 |= TIM_CR1_CEN;
	
	while(i < NUM_AMOST);

	RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN;
	RCC->APB2ENR &= ~RCC_APB2ENR_ADC3EN;
	while(1);
}

void ADC_IRQHandler(void)
{
	data[i++] = (uint32_t) adc->DR;
	GPIOD->ODR ^= GPIO_ODR_OD12;
}
