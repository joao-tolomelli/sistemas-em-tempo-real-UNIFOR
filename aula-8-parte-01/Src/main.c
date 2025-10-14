#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"

#define SR_CC1IF 		(1U<<1)

int timestamp = 0;

int main(void){


	tim2_pa5_output_compare();
	tim3_pa6_input_capture();

	while(1){
		/* Espera até uma borda de subida ser capturada */
		while(!(TIM3->SR & SR_CC1IF)){}

		/* Ler o valor de captura */
		timestamp = TIM3->CCR1;
	}

}







