#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"

#define GPIOAEN					(1U<<0)
#define PIN5					(1U<<5)

#define	LED						PIN5
#define SR_UIF					(1U<<0)

int main(void){

	/* Habilita o clock  para acessar o GPIOA */
	RCC->AHB1ENR |= GPIOAEN;

	/* Configura PA5 como pino de saída - LED */
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &=~(1U<<11);

	/* Inicializa o canal serial */
	uart2_rxtx_init();
	tim2_1hz_interrupt_init();

	while(1){
		//nada
	}

}

static void tim2_callback(void){
	printf("A second passed !! \n\r");
			GPIOA->ODR ^= LED;
}

void TIM2_IRQHandler(void){
	/* Limpa o flag de interupção */
	TIM2->SR &=~SR_UIF;
	tim2_callback();
}









