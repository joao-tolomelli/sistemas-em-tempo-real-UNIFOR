#include "stm32f4xx.h"

#define SYSTICK_LOAD_VAL			16000 - 1
#define CTRL_ENABLE					(1U<<0)
#define CTRL_CLKSRC					(1U<<0)
#define CTRL_COUNTFLAG				(1U<<0)

void systickDelayMs(int delay){
	/* Recarga com número de clocks por milisegundos */
	SysTick->LOAD = SYSTICK_LOAD_VAL;

	/* Zera o valor atual do registrador Systick */
	SysTick->VAL = 0;

	/* Habilita o systick e seleciona a fonte de clock interno */
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;

	for (int i = 0; i<delay; i++){
		/* Espera até o bit COUNTFLAG estar setado */
		while((SysTick->CTRL & CTRL_COUNTFLAG)==0){}
	}
	SysTick->CTRL = 0;
}
