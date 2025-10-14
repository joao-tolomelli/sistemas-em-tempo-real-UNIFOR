#include "stm32f4xx.h"
#define GPIOAEN			(1U<<0)
#define GPIOCEN			(1U<<2)

#define PIN5			(1U<<5)
#define PIN5			(1U<<13)

#define LED_PIN			PIN5
#define BTN_PIN			PIN13

int main(void){

	//Habilita o clock para acessar GPIOA e GPIOC
	RCC->AHB1ENR |=GPIOAEN;
	RCC->AHB1ENR |=GPIOCEN;

	//Set PA5 como pino de saída - Ver RM pag. 157
	GPIOA->MODER |=(1U<<10);
	GPIOA->MODER &=~(1U<<11);

	//Set PA13 como pino de entrada - Ver RM pag. 157
	GPIOA->MODER &=~(1U<<26);
	GPIOA->MODER &=~(1U<<27);

	while(1){
		// Verifica se o push button foi pressionado
		if(GPIOC->IDR & BTN_PIN){ // IDR -> Ver RM pag. 159
			//Liga o LED
			GPIOA->BSRR = LED_PIN;  // BSRR -> Ver RM pag. 160
		}else{
			//Desliga o LED
			GPIOA->BSRR = (1U<<21);
		}

	}

}
