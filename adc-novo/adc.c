#include "stm32f4xx.h"                  // Device header

#define MILISECONDS(x)	(uint32_t)(x)
#define SECONDS(x)			(uint32_t)(x * MILISECONDS(100))
#define NUM_AMOST				(uint32_t)(10)

#define SALVA_DADO		
#define INVERTE_LED			GPIOD->ODR ^= GPIO_ODR_OD12;
#define USE_SYSTICK
#ifdef USE_SYSTICK
	#define SYSTICK_TIME		MILISECONDS(10)
#endif


float volts = 0.0f;
volatile uint32_t tick = 0;
volatile uint32_t i=0;
volatile uint8_t data[NUM_AMOST];
volatile uint8_t *point = &data[0];

void SysTick_Handler(void);
void ADC_IRQHandler(void);

ADC_TypeDef *adc = ADC3;
GPIO_TypeDef *gpio = GPIOA;

int main(void)
{
	
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD -> MODER |= GPIO_MODER_MODER12_0;
	
#ifdef USE_SYSTICK
	SysTick_Config(SystemCoreClock/1000);
#endif
	
	//GPIO CFG(ANALOG MODE)
#if 1
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE0) | (3 << GPIO_MODER_MODE0_Pos);//PA 0 analog mode
	
	RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;
	adc->CR2 = (adc->CR2 & ~ADC_CR2_ADON)|(0 << ADC_CR2_ADON);
    
	adc->CR1 = (adc->CR1 & ~(ADC_CR1_RES|ADC_CR1_AWDEN))|(2 << ADC_CR1_RES_Pos)|(0 << ADC_CR1_EOCIE_Pos);// ADC 8BITS & INTERRUPT ABLE
	adc->SQR1 = (adc->SQR1 & ~ADC_SQR1_L)|(0 << ADC_SQR1_L_Pos);//1 CONV
	adc->SQR3 = (adc->SQR3 & ~ADC_SQR3_SQ1)|(0 << ADC_SQR3_SQ1_Pos);//CHANEL 0
	adc->SMPR2 = (adc->SMPR2 & ~ADC_SMPR2_SMP1)|(8 << ADC_SMPR2_SMP1_Pos);//SAMMPLING TIME
	adc->CR2 = (adc->CR2 & ~(ADC_CR2_CONT|ADC_CR2_ADON))|((1 << ADC_CR2_CONT_Pos)|(1 << ADC_CR2_ADON_Pos)
						 |(1 << ADC_CR2_EXTEN_Pos)|(3 << ADC_CR2_EXTSEL_Pos));//continuous mode on & TURN ON ADC & EXTEN RISING EDGE &EXSEL TIMER CANAL 2 EVENT
	
	NVIC_EnableIRQ(ADC_IRQn);
#endif

	//confg pin timmer 
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	gpio -> MODER |= GPIO_MODER_MODER1_1;
	gpio->AFR[0] |= 1 << GPIO_AFRL_AFSEL1_Pos;
	gpio->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED1_Pos;
	
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	TIM2->PSC = 1000 - 1;//VALOR PELO QUAL O CLOCK INICIAL É DIVIDIDO
	TIM2->ARR = 16 - 1;
	TIM2->CR1 |= TIM_CR1_CEN;
	
	TIM2->CCMR1 = (TIM2->CCMR1 & ~TIM_CCMR1_OC2M_Pos)| (6 << TIM_CCMR1_OC2M_Pos);
	TIM2->CR1 |= TIM_CR1_ARPE;
	TIM2->CCR2 = 8 - 1;//tempo alto
	TIM2->CCER = (TIM2->CCER & ~TIM_CCER_CC2E)| (1 << TIM_CCER_CC2E_Pos);
	
	while(1)
	{
	#if 0
		//volts = (float)((float)((uint8_t)adc->DR)/(float)255 * 3.0f);
		
		//adc->CR2 = (adc->CR2 & ~ADC_CR2_SWSTART)|(1 << ADC_CR2_SWSTART_Pos);
	
		INVERTE_LED
		//dado = adc->DR;
	#endif
		
		while(i < NUM_AMOST);
		TIM2->CR1 &= ~TIM_CR1_CEN;
		adc->CR2 = (adc->CR2 & ~(ADC_CR2_ADON))|(0 << ADC_CR2_ADON_Pos);
		while(1);
		
	#if 0
		tick = 0;
		while(tick < SYSTICK_TIME);
	#endif
		
	}
}

void SysTick_Handler(void)
{
	tick++;
}

void ADC_IRQHandler(void)
{
	data[i++] = adc->DR;
	
}
