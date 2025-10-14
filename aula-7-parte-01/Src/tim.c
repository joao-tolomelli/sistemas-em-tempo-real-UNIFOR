#include "stm32f4xx.h"
#include "stdint.h"

#define TIM2EN				(1U<<0)
#define CR1_CEN				(1U<<0)
#define OC_TOGGLE 			((1U<<4) | (1U<<5))
#define CCER_CC1E			(1U<<0)

#define GPIOAEN				(1U<<0)
#define AFR5_TIM			(1U<<20)

void tim2_pa5_output_compare(void){

	/* Habilita o clock  para acessar o GPIOA */
	RCC->AHB1ENR |= GPIOAEN;

	/* Configura PA5 para o modo alternate function */
	GPIOA->MODER &=~(1U<<10);
	GPIOA->MODER |= (1U<<11);

	/* Configura PA5 alternate function tipo TIM2_CH1 (AF01) */
	GPIOA->AFR[0] |= AFR5_TIM;

	/* Habilita o clock para acesso ao timer2 */
	RCC->APB1ENR |= TIM2EN;

	/* Configura o valor do prescaler */
	TIM2->PSC= 1600 - 1;// 16.000.000 / 1.600 = 10.000

	/* Configura o valor de recarga automática (auto-reload) */
	TIM2->ARR = 10000 - 1; // 10.000 / 10.000 = 1

	/* Configura o output compare para o modo toggle */
	TIM2->CCMR1 = OC_TOGGLE;

	/* Habilita o CH1 do tim2 para compare mode */
	TIM2->CCER |= CCER_CC1E;

	/* Limpa o contador */
	TIM2->CNT = 0;

	/* Habilita o timer */
	TIM2->CR1 = CR1_CEN;
}












