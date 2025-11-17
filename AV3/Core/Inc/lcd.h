/*
 * lcd.h
 *
 * Driver de LCD 16x2 para STM32F4xx
 * Adaptado para usar a biblioteca HAL e FreeRTOS.
 *
 * Baseado no driver original fornecido.
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

// Inclui a biblioteca HAL principal.
// Isso nos dá acesso a HAL_GPIO_WritePin, HAL_Delay, etc.
#include "stm32f4xx_hal.h"

// --- Definições das Conexões do LCD ---
// (Mude aqui se você usar pinos diferentes no CubeMX)
#define RS_PORT GPIOA
#define RS_PIN  GPIO_PIN_9

#define EN_PORT GPIOC
#define EN_PIN  GPIO_PIN_7

#define D4_PORT GPIOB
#define D4_PIN  GPIO_PIN_5

#define D5_PORT GPIOB
#define D5_PIN  GPIO_PIN_4

#define D6_PORT GPIOB
#define D6_PIN  GPIO_PIN_10

#define D7_PORT GPIOA
#define D7_PIN  GPIO_PIN_8
// ----------------------------------------


/**
 * @brief Inicializa o LCD no modo 4 bits.
 * @note  Deve ser chamada na main() ANTES de iniciar o scheduler do FreeRTOS.
 */
void lcd_init(void);

/**
 * @brief Limpa o display do LCD.
 */
void lcd_clear(void);

/**
 * @brief Posiciona o cursor em uma linha e coluna.
 * @param row 0 (Linha 1) ou 1 (Linha 2)
 * @param col 0 a 15
 */
void lcd_put_cur(int row, int col);

/**
 * @brief Envia um único caractere para o LCD.
 * @param data O caractere a ser escrito.
 */
void lcd_send_data(char data);

/**
 * @brief Envia uma string (terminada em '\0') para o LCD.
 * @param str Ponteiro para a string.
 */
void lcd_send_string(char *str);


#endif /* INC_LCD_H_ */
