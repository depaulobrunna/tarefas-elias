#include "stm32f4xx.h"                  // Device header

uint8_t i,count,timeout = 100,data;
uint32_t reset;
float volts = 0.0f;
volatile uint32_t tick = 0;

ADC_TypeDef *adc = ADC3;

int main(void)
{
	SysTick_Config(SystemCoreClock/1000);
	//GPIO CFG(ANALOG MODE)
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE0) | (3 << GPIO_MODER_MODE0_Pos);//PA 0 analog mode
	
	
	RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;
	adc->CR2 = (adc->CR2 & ~ADC_CR2_ADON)|(0 << ADC_CR2_ADON);
    
	adc->CR1 = (adc->CR1 & ~(ADC_CR1_RES|ADC_CR1_AWDEN))|(2 << ADC_CR1_RES_Pos);
	adc->SQR1 = (adc->SQR1 & ~ADC_SQR1_L)|(0 << ADC_SQR1_L_Pos);
	adc->SQR3 = (adc->SQR3 & ~ADC_SQR3_SQ1)|(0 << ADC_SQR3_SQ1_Pos);
	adc->SMPR2 = (adc->SMPR2 & ~ADC_SMPR2_SMP1)|(8 << ADC_SMPR2_SMP1_Pos);
	adc->CR2 = (adc->CR2 & ~(ADC_CR2_CONT|ADC_CR2_ADON))|((1 << ADC_CR2_CONT_Pos)|(1 << ADC_CR2_ADON_Pos));
	
#if 0
	adc->CR2 = ( (adc->CR2 & ~(ADC_CR2_EOCS))| (1 << ADC_CR2_EOCS_Pos));
	

#endif	
	while(1)
	{
#if 1	
		while(!(adc->SR & ADC_SR_STRT))
		{
				adc->CR2 = (adc->CR2 & ~ADC_CR2_SWSTART)|(1 << ADC_CR2_SWSTART_Pos);
		}

		//while(((ADC2->SR & ADC_SR_EOC) == reset) && (count < timeout)) count++;
		while(!(adc->SR & ADC_SR_EOC));//ESPERA A CONVERSAO TERMINAR
//		adc->SR &= ~ADC_SR_EOC;

		data = adc->DR;
		#if 1
		tick = 0;
		while(tick < 1000);
		volts = (float)((float)((uint8_t)adc->DR)/(float)255 * 3.3f);
		#endif
			
#endif
	}
}

void SysTick_Handler(void)
{
	tick++;
}
