#include "stm32f4xx.h"

#define SYSTICK_LOAD_VAL			16000 - 1
#define CTRL_ENABLE					(1U<<0)
#define CTRL_CLKSRC					(1U<<0)
#define CTRL_COUNTFLAG				(1U<<0)
#define CTRL_TICKINT				(1U<<1)

void systick_1hz_interrupt(void){
	/* Recarga com número de clocks por milisegundos */
	SysTick->LOAD = SYSTICK_LOAD_VAL;

	/* Zera o valor atual do registrador Systick */
	SysTick->VAL = 0;

	/* Habilita o systick e seleciona a fonte de clock interno */
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;

	/* Habilita a interrupção do systick */
	SysTick->CTRL |= CTRL_TICKINT;
}
