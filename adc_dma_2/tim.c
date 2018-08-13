#include "tim.h"
#include "stm32f4xx.h"                

GPIO_TypeDef *gpio = GPIOA;
TIM_TypeDef *tim = TIM2;

void timInit(uint32_t *psc, uint32_t *arr, uint32_t *high_time)
{
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	gpio-> MODER |= GPIO_MODER_MODER1_1; 
	gpio->AFR[0] |= 1 << GPIO_AFRL_AFSEL1_Pos; 
	gpio->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED1_Pos;
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	tim->PSC = (uint32_t)psc - 1;
	tim->ARR = (uint32_t)arr - 1;
	
	tim->CCMR1 = (6 << TIM_CCMR1_OC2M_Pos);
	tim->CR1 |= TIM_CR1_ARPE;
	tim->CCR2 = (uint32_t)high_time - 1;
	tim->CCER = (1 << TIM_CCER_CC2E_Pos);
}

void timStart(void)
{
	TIM2->CR1 |= TIM_CR1_CEN;
}

void timStop(void)
{
	TIM2->CR1 &= ~TIM_CR1_CEN;
}
