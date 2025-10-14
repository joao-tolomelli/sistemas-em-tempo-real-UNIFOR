#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"

#define SR_EOC			(1U<<1)

int sensor_value;

int main(void)
{
	/* Configura o canal serial - UART2 */
    uart2_rxtx_init();

    /* Configura o ADC */
    pa1_adc_init();
    start_convertion();


    while(1)
    {
        //Nada
    }
}

static void adc_callback(void){
	sensor_value = ADC1->DR;
	printf("Sensor value: %d \n\r",(int)sensor_value);
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
