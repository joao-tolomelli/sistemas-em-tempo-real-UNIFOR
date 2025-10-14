#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"

#define GPIOAEN			(1U<<0)
#define GPIOCEN			(1U<<2)
#define SR_EOC			(1U<<1)
#define SR_RXNE			(1U<<5)


#define PIN13           (1U<<13)
#define BTN_PIN         PIN13
#define LED_PIN			(1U<<10)


int sensor_value;
char key;				/* Variável que vai receber o caractere */

// Protótipos de funções
static void adc_callback(void);
static void uart_callback(void);
void ADC_IRQHandler(void);
void USART1_IRQHandler(void);


int main(void)
{
	/* Habilita clock para acessar GPIOs */
	RCC->AHB1ENR |=GPIOCEN;

	//Set PC10 como pino de saída
	GPIOC->MODER |=(1U<<20);
	GPIOC->MODER &=~(1U<<21);

	GPIOC->MODER &=~(1U<<26);
	GPIOC->MODER &=~(1U<<26);


	/* Configura o canal serial - UART2 */
    uart1_rxtx_init();

    /* Configura o ADC */
    pa1_adc_init();

    NVIC_EnableIRQ(USART1_IRQn);
	NVIC_EnableIRQ(ADC_IRQn);

	NVIC_SetPriority(USART1_IRQn,0);
    NVIC_SetPriority(ADC_IRQn,1);



    while(1)
    {
        if(!(GPIOC->IDR & BTN_PIN)){
        	start_convertion();
        }
    }
}

static void adc_callback(void){

	// Leitura do valor bruto do registrador de dados do ADC
	sensor_value = ADC1->DR;

	// Converção do valor para Volts (ADC de 12-bit, Vref = 3.3V)
	float voltage = (sensor_value / 4095.0f) * 3.3f;

	printf("Sensor voltage: %.2f V\n\r", voltage);
	for(int i=0; i<100000; i++){}
}

void ADC_IRQHandler(void){

	/* Verifica o bit EOC - Fim da conversão */
	if ((ADC1->SR & SR_EOC)!=0){
		/* Limpa o EOC */
		ADC1->SR &=~SR_EOC;
		adc_callback();
	}
}

/* Rotina de serviço da interrupção de recepção serial - callback */
static void uart_callback(void){
	key = USART1->DR;
	GPIOC->ODR ^= LED_PIN;
	printf("tecla pressionada: %c \n\r",key);
}

/* Manipulador de interrupção do canal serial */
void USART1_IRQHandler(void){
	/* Verifica se RXNE is set */
	if(USART1->SR & SR_RXNE){
		uart_callback();
	}
}
