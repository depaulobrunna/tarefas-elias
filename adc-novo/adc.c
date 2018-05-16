#include "stm32f4xx.h"                  // Device header

#define MILISECONDS(x)	(uint32_t)(x)
#define SECONDS(x)			(uint32_t)(x * MILISECONDS(100))

#define INVERTE_LED			GPIOD->ODR ^= GPIO_ODR_OD12;
#define USE_SYSTICK
#ifdef USE_SYSTICK
	#define SYSTICK_TIME		MILISECONDS(100)
#endif

uint32_t i;
float volts = 0.0f;
volatile uint32_t tick = 0;
uint8_t data[100];
uint8_t *point = &data[0];
uint8_t dado;

void SysTick_Handler(void);
void ADC_IRQHandler(void);

ADC_TypeDef *adc = ADC3;

int main(void)
{
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD -> MODER |= GPIO_MODER_MODER12_0;
#ifdef USE_SYSTICK
	SysTick_Config(SystemCoreClock/1000);
#endif
	//GPIO CFG(ANALOG MODE)

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE0) | (3 << GPIO_MODER_MODE0_Pos);//PA 0 analog mode
	
	RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;
	adc->CR2 = (adc->CR2 & ~ADC_CR2_ADON)|(0 << ADC_CR2_ADON);
    
	adc->CR1 = (adc->CR1 & ~(ADC_CR1_RES|ADC_CR1_AWDEN))|(2 << ADC_CR1_RES_Pos)|(1 << ADC_CR1_EOCIE_Pos);// ADC 8BITS & INTERRUPT ENABLE
	adc->SQR1 = (adc->SQR1 & ~ADC_SQR1_L)|(0 << ADC_SQR1_L_Pos);//1 CONV
	adc->SQR3 = (adc->SQR3 & ~ADC_SQR3_SQ1)|(0 << ADC_SQR3_SQ1_Pos);//CHANEL 0
	adc->SMPR2 = (adc->SMPR2 & ~ADC_SMPR2_SMP1)|(8 << ADC_SMPR2_SMP1_Pos);//SAMMPLING TIME
	adc->CR2 = (adc->CR2 & ~(ADC_CR2_CONT|ADC_CR2_ADON))|((1 << ADC_CR2_CONT_Pos)|(1 << ADC_CR2_ADON_Pos));//continuous mode on & TURN ON ADC
	
	NVIC_EnableIRQ(ADC_IRQn);
	while(1)
	{
		
		
		//volts = (float)((float)((uint8_t)adc->DR)/(float)255 * 3.0f);
		
		adc->CR2 = (adc->CR2 & ~ADC_CR2_SWSTART)|(1 << ADC_CR2_SWSTART_Pos);
		while((adc->SR & ADC_SR_EOC)==0);//ESPERA A CONVERSAO TERMINAR
			
		
			
		tick = 0;
		while(tick < SYSTICK_TIME);
//		INVERTE_LED
		
	}
}

void SysTick_Handler(void)
{
	tick++;
}

void ADC_IRQHandler(void)
{
	INVERTE_LED
	dado = adc->DR;
}
