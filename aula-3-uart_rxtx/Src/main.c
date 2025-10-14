#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"

#define GPIOAEN			(1U<<0)
#define GPIOA_5			(1U<<5)

#define LED_PIN			GPIOA_5

char key;				/* Variável que vai receber o caractere */

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
        key = uart2_read();

        if(key == '1'){
        	GPIOA->ODR ^= LED_PIN;
        }
        else{
        	GPIOA->ODR &=~LED_PIN;
        }
    }
}
