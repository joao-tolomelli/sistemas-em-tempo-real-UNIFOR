#include "stm32f4xx.h"
#include "stdint.h"

#define TIM2EN				(1U<<0)
#define CR1_CEN				(1U<<0)

void tim2_1hz_init(void){
	/* Habilita o clock para acesso ao timer2 */
	RCC->APB1ENR |= TIM2EN;
	/* Configura o valor do prescaler */
	TIM2->PSC= 1600 - 1;// 16.000.000 / 1.600 = 10.000
	/* Configura o valor de recarga automática (auto-reload) */
	TIM2->ARR = 10000 - 1; // 10.000 / 10.000 = 1
	/* Limpa o contador */
	TIM2->CNT = 0;
	/* Habilita o timer */
	TIM2->CR1 = CR1_CEN;
}












