#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"

#define GPIOAEN     (1U<<0)
#define UART1EN     (1U<<4) // Modificado para UART1 no APB2

#define CR1_TE      (1U<<3)
#define CR1_RE		(1U<<2)
#define CR1_RXNEIE  (1U<<5) // Habilita interrupção de recepção
#define CR1_UE      (1U<<13)
#define SR_TXE      (1U<<7)
#define SR_RXNE		(1U<<5)

#define SYS_FREQ    16000000    /* Constante do clock da CPU */
#define APB2_CLK    SYS_FREQ    // UART1 está no APB2

#define UART_BAUDRATE  9600

/* Declarações das funções protótipos */
static void uart_set_baudrate (USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd (uint32_t PeriphClk, uint32_t BaudRate);
void uart1_rxtx_init(void);
void uart1_write(int ch);
char uart1_read(void);

// O caractere será enviado pela UART1
int __io_putchar(int ch){
	uart1_write(ch);
	return ch;
}

void uart1_rxtx_init(void)
{
    /************ Configura o pino GPIO para UART ************/
    /* Habilita o clock para acessar GPIOA */
    RCC->AHB1ENR |= GPIOAEN;

    /* Configura o modo de PA9 para função alternativa (UART1_TX) */
    GPIOA->MODER &= ~(1U<<18);
    GPIOA->MODER |=  (1U<<19);

    /* Configura PA9 para a função alternativa UART_TX (AF07) */
    GPIOA->AFR[1] |= (1U<<4);
    GPIOA->AFR[1] |= (1U<<5);
    GPIOA->AFR[1] |= (1U<<6);
    GPIOA->AFR[1] &= ~(1U<<7);

    /* Configura PA10 para a função alternativa (UART1_RX) */
    GPIOA->MODER &= ~(1U<<20);
    GPIOA->MODER |=  (1U<<21);

    /* Configura PA10 para a função alternativa UART_RX (AF07) */
    GPIOA->AFR[1] |= (1U<<8);
    GPIOA->AFR[1] |= (1U<<9);
    GPIOA->AFR[1] |= (1U<<10);
    GPIOA->AFR[1] &= ~(1U<<11);


    /************ Configura o canal serial uart1 ************/
    /* Habilita o clock para acessar a uart1 */
    RCC->APB2ENR |= UART1EN; // Clock para a UART1 no APB2

    /* Configura o baudrate */
    uart_set_baudrate(USART1, APB2_CLK, UART_BAUDRATE);

    /* Habilita a transmissão e recepção serial */
    USART1->CR1 = (CR1_TE | CR1_RE);

    /* Habilita a interrupção da recepção serial - RXNE */
    USART1->CR1 |= CR1_RXNEIE;

    /* Habilita a interrupção serial no controlador NVIC */
    //NVIC_EnableIRQ(USART1_IRQn);

    /* Habilita a uart1 */
    USART1->CR1 |= CR1_UE;
}

char uart1_read(void){
	/* Verifica se o buffer serial de recepção não está vazio */
	while(!(USART1->SR & SR_RXNE)){}

	return USART1->DR;
}

void uart1_write(int ch){
    /* Certifique-se que o registrador de transmissão de dados esteja vazio */
    while(!(USART1->SR & SR_TXE)){}

    /* Escreve o valor no registrador de dados */
    USART1->DR = (ch & 0xFF);
}

static void uart_set_baudrate (USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
    USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd (uint32_t PeriphClk, uint32_t BaudRate)
{
    return ((PeriphClk + (BaudRate/2U))/BaudRate);
}
