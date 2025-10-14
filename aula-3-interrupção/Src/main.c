#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"

#define GPIOAEN			(1U<<0)
#define GPIOA_5			(1U<<5)
#define SR_RXNE			(1U<<5)

#define LED_PIN			GPIOA_5

char key;				/* Variável que vai receber o caractere */

static void uart_callback(void);
void uart2_rxtx_init(void);

int main(void)
{
	/* Habilita o clock para acessar GPIOA */
	RCC->AHB1ENR |= GPIOAEN;

	/* Configura o pino PA5 como entrada - RX Serial */
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &=~(1U<<11);

	/* Configura o canal serial - UART2 */
    uart2_rxtx_init();

    while(1)
    {
        /* nada */
    }
}

/* Rotina de serviço da interrupção de recepção serial - callback */
static void uart_callback(void){
	key = USART2->DR;
	GPIOA->ODR ^= LED_PIN;
	printf("tecla pressionada: %c \n\r",key);
}

/* Manipulador de interrupção do canal serial */
void USART2_IRQHandler(void){

	/* Verifica se RXNE is set */
	if(USART2->SR & SR_RXNE){
		uart_callback();
	}
}
