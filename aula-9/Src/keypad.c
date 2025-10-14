#include "keypad.h"

uint8_t keypad_init()
{
    // Habilita clock para GPIOA e ADC1
    RCC->AHB1ENR |= (1U << 0);
    RCC->APB2ENR |= (1U << 8);

    // Configura PA1 como entrada analógica (A1 no shield)
    GPIOA->MODER |= 0x3;

    ADC1->CR2 = 0;
    ADC1->SQR3 = 0; // Conversão no canal 1

    ADC1->SQR1 = 4; // Tamanho da sequência = 1
    ADC1->CR2 |= 1; // Habilita ADC1

    return 0;
}

uint16_t keypad_read_key()
{
    uint16_t adc_readout = 0;

    ADC1->CR2 |= (1U << 30); // Inicia a conversão
    while (!(ADC1->SR & 2)) {} // Espera o final da conversão

    adc_readout = ADC1->DR; // Retorna o resultado

    if (adc_readout >= 0 && adc_readout < 50) // RIGHT
    {
        return KEY_RIGHT;
    }
    else if (adc_readout > 800 && adc_readout < 850)
    {
        return KEY_UP;
    }
    else if (adc_readout > 1950 && adc_readout < 2050)
    {
        return KEY_DOWN;
    }
    else if (adc_readout > 3050 && adc_readout < 3150)
    {
        return KEY_LEFT;
    }
    else if (adc_readout > 4000 && adc_readout < 5050)
    {
        return KEY_SELECT;
    }

    return KEY_NONE;
}




