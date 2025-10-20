#include "stm32f4xx.h"
#include "tim.h"

void tim2_init(void)
{
    // Habilita o clock para acesso ao timer2
    RCC->APB1ENR |= TIM2EN;

    // Configura o valor do prescaler
    TIM2->PSC = 16;
    // Configura o valor de recarga automática (auto-reload)
    TIM2->ARR = 1000;
    // Limpa o contador
    TIM2->CNT = 0;

    // Desabilita o timer
    TIM2->CR1 &= ~CR1_CEN;
}

void delay(uint16_t ms)
{
    // Habilita o timer
    TIM2->CR1 |= CR1_CEN;
    TIM2->CNT = 0;

    for (int i = 0; i < ms; i++) {
        // Espera pelo flag UIF (Estouro do timer)
        while (!(TIM2->SR & SR_UIF)) {}
        // Limpa o flag UIF
        TIM2->SR &=~SR_UIF;
    }

    TIM2->CR1 &=~CR1_CEN;
}

// Delay de aprox. 1mS
void delay_ms (uint16_t us)
{
    for (int i=0; i < 50*us; i++) {}
}

// Delay de aprox. 20uS
void delayLCD (uint16_t us)
{
    for (int i=0; i < us; i++) {}
}
















