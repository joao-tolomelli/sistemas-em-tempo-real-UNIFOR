#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"


void uart2_tx_init(void);

int main(void)
{
    uart2_tx_init();

    while(1)
    {
        printf("Testando uart modular...\n\r");
    }
}
