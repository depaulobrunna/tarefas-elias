#include "stm32f4xx.h"                  // Device header

#define AUTO		TIM2->ARR
#define PRESC		TIM2->PSC

uint32_t periodo, clock = 16000000;
int main()
{
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA-> MODER |= GPIO_MODER_MODER1_1;
	GPIOA->AFR[0] |= 1 << GPIO_AFRL_AFSEL1_Pos;
	GPIOA->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED1_Pos;
	
	//CONFIG TIMER pwm
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	TIM2->PSC = 1000 - 1;//VALOR PELO QUAL O CLOCK INICIAL É DIVIDIDO
	TIM2->ARR = 16 - 1;
	
	TIM2->CCMR1 = (TIM2->CCMR1 & ~TIM_CCMR1_OC2M_Pos)| (6 << TIM_CCMR1_OC2M_Pos);
	TIM2->CR1 |= TIM_CR1_ARPE;
	TIM2->CCR2 = 8 - 1;//tempo alto
	TIM2->CCER = (TIM2->CCER & ~TIM_CCER_CC2E)| (1 << TIM_CCER_CC2E_Pos);
	
	TIM2->CR1 |= TIM_CR1_CEN;
	
	periodo = clock/PRESC;
	while(1)
	{
		
		while(!(TIM2->SR & TIM_SR_UIF));
		TIM2->SR &= ~(TIM_SR_UIF);
		
	}
}


