#include "stm32f4xx.h"                  // Device header

#define A GPIO_ODR_ODR_14

uint8_t data;
uint8_t data2 = 0;

void set_timmer(void);
void set_adc(void);

int main(void){
	#if 1
	
	//LED TEST
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;//habilita o clock da porta D
	GPIOD->MODER |= GPIO_MODER_MODER14_0;
	GPIOD->ODR &= ~A;
	
	set_adc();//executa a func de setar os pinos gpio e o periferico adc
	
	#endif
	while(1)
	{
		#if 1
		
		while(!(ADC2->SR & ADC_SR_STRT))
		{
			ADC2->CR2 |= ADC_CR2_SWSTART;//COMECA A CONV
		}
		ADC2->SR &= ~ADC_SR_STRT;
 		while(!(ADC2->SR & ADC_SR_EOC));//ESPERA A CONVERSAO TERMINAR
		ADC2->SR &= ~ADC_SR_EOC;
		GPIOD->ODR ^= A;
		data2 = ADC2-> DR;//ARMAZENA OS DADOS
		//ADC2->CR2 &= ~ADC_CR2_ADON;
#endif
	}
}

void set_timmer()
{
	//confg pin timmer 
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB -> MODER |= GPIO_MODER_MODER3_1;//MODO AF
	GPIOB-> AFR[0] |= 1 << 12 ;
	GPIOB->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED3_Pos;
	
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	TIM2->PSC = 1600 - 1;//VALOR PELO QUAL O CLOCK INICIAL É DIVIDIDO
	TIM2->ARR = 10 - 1;
	TIM2->CR1 |= TIM_CR1_CEN|
							 TIM_CR1_ARPE;
	
}
void set_adc()
{
	//GPIO CFG(ANALOG MODE)
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODE0_0|GPIO_MODER_MODE0_1;//PA 0
	
	//ADC CFG
	RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
	//ADC->CCR = 0 << ADC_CCR_ADCPRE_Pos;//PRESCALE
	ADC2->CR2 &= ~ADC_CR2_ADON;
		
	ADC2->CR1 |= ADC_CR1_RES_0;//ADC 8 BITS
	ADC2->CR1 &= ~ADC_CR1_AWDEN;//WHATCHDOG DISABLE
	//ADC2->HTR &= ~ADC_HTR_HT;
	ADC2->SQR1 = 0 << ADC_SQR1_L_Pos;//N DE CONV = 1
	ADC2->SQR3 = 1 << ADC_SQR3_SQ1_Pos;//(CANAL 0)
	ADC2->SMPR2 = 8 << ADC_SMPR2_SMP1_Pos;//SAMPLE TIME 84 CYCLES
	ADC2->CR2 &= ~ADC_CR2_CONT;//CONTINUOUS MODE DISABLE
	ADC2->CR2 |= ADC_CR2_ADON;

}
