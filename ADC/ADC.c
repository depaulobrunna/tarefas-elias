#include "stm32f4xx.h"                  // Device header

uint8_t i,u,t;

int main(void)
{
	//GPIO CFG(ANALOG MODE)
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODE0_0|GPIO_MODER_MODE0_1;//PA 0
	
	RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
	ADC2->CR2 = (ADC2->CR2 & ~ADC_CR2_ADON)|(0 << ADC_CR2_ADON);
    
	ADC2->CR1 = (ADC2->CR1 & ~(ADC_CR1_RES|ADC_CR1_AWDEN))|(1 << ADC_CR1_RES_Pos);
	ADC2->SQR1 = (ADC2->SQR1 & ~ADC_SQR1_L)|(0 << ADC_SQR1_L_Pos);
	ADC2->SQR3 = (ADC2->SQR3 & ~ADC_SQR3_SQ1)|(1 << ADC_SQR3_SQ1_Pos);
	ADC2->SMPR2 = (ADC2->SMPR2 & ~ADC_SMPR2_SMP1)|(8 << ADC_SMPR2_SMP1_Pos);
	ADC2->CR2 = (ADC2->CR2 & ~(ADC_CR2_CONT|ADC_CR2_ADON))|((0 << ADC_CR2_CONT_Pos)|(1 << ADC_CR2_ADON_Pos));
	
	while(1)
	{
    #if 1		
        while(!(ADC2->SR & ADC_SR_STRT))
        {
            ADC2->CR2 = (ADC2->CR2 & ~ADC_CR2_SWSTART)|(1 << ADC_CR2_SWSTART_Pos);
        }

        while((ADC2->SR & ADC_SR_EOC) != ADC_SR_EOC );//ESPERA A CONVERSAO TERMINAR
        ADC2->SR &= ~ADC_SR_EOC;
				t = ADC2->DR;
				for (i=0; i < 100 ;i ++);
    #endif
	}
}