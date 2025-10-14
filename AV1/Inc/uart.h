#ifndef UART_H_
#define UART_H_
#include <stdint.h>

#include "stm32f4xx.h"

static void uart_set_baudrate (USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd (uint32_t PeriphClk, uint32_t BaudRate);
void uart1_tx_init(void);
void uart1_write(int ch);

#endif
