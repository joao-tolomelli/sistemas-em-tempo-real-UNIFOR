#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"

int sensor_value;

int main(void)
{
	/* Configura o canal serial - UART2 */
    uart2_rxtx_init();

    /* Configura o ADC */
    pa1_adc_init();

    while(1)
    {
        start_convertion();
        sensor_value = adc_read();
        printf("valor digital lido = %d \n\r",sensor_value);
        for(int i=0; i<100000;i++);
    }
}
