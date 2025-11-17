/*
 * lcd.c
 *
 * Driver de LCD 16x2 para STM32F4xx
 * Adaptado para usar a biblioteca HAL.
 *
 * - Troca de acesso a registradores por HAL_GPIO_WritePin()
 * - Troca de delay_ms() por HAL_Delay()
 * - Adiciona uma função interna 'delay_us' para o pulso do pino Enable (EN)
 */

#include "lcd.h"

// --- Protótipos de Funções Privadas ---

/**
 * @brief Função interna de delay curto (microsegundos) para o pulso do 'Enable'.
 * @note  Usa um loop 'for' simples. A precisão não é exata, mas é suficiente
 * para a maioria dos LCDs.
 */
static void delay_us(volatile uint32_t us)
{
    // Esta é uma aproximação. O valor '8' foi
    // ajustado para um clock comum (ex: 84MHz).
    // O objetivo é apenas criar um pulso muito curto.
    for(uint32_t i = 0; i < us * 8; i++)
    {
        __NOP();
    }
}

/**
 * @brief Envia um comando (rs=0) ou dado (rs=1) para o LCD.
 * @note  Esta é a função de baixo nível que gerencia o modo 4-bits.
 */
static void send_to_lcd(char data, int rs)
{
    // Define o pino RS (Register Select)
    // rs=1 para dados, rs=0 para comandos
    HAL_GPIO_WritePin(RS_PORT, RS_PIN, (rs == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    // --- Envia o NIBBLE SUPERIOR (4 bits) ---
    HAL_GPIO_WritePin(D7_PORT, D7_PIN, ((data >> 7) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D6_PORT, D6_PIN, ((data >> 6) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D5_PORT, D5_PIN, ((data >> 5) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D4_PORT, D4_PIN, ((data >> 4) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    // Pulso do 'Enable'
    HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_SET);
    delay_us(5); // Pequeno delay
    HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_RESET);
    delay_us(50); // Delay de hold


    // --- Envia o NIBBLE INFERIOR (4 bits) ---
    HAL_GPIO_WritePin(D7_PORT, D7_PIN, ((data >> 3) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D6_PORT, D6_PIN, ((data >> 2) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D5_PORT, D5_PIN, ((data >> 1) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D4_PORT, D4_PIN, ((data >> 0) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    // Pulso do 'Enable'
    HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_SET);
    delay_us(5); // Pequeno delay
    HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_RESET);
    delay_us(50); // Delay de hold
}

/**
 * @brief Envia um comando para o LCD.
 */
static void lcd_send_cmd(char cmd)
{
    // O driver original dividia o byte em dois nibbles aqui.
    // É mais eficiente fazer isso uma vez dentro da send_to_lcd,
    // mas vamos manter a lógica original para compatibilidade.
    char data_upper_nibble = (cmd >> 4) & 0x0F;
    char data_lower_nibble = cmd & 0x0F;

    // Recria o byte para enviar (a send_to_lcd vai dividir de novo)
    // Isto é para manter a estrutura do seu 'send_to_lcd' original
    send_to_lcd((data_upper_nibble << 4) | data_lower_nibble, 0);
}


// --- Funções Públicas ---

void lcd_init(void)
{
    // A inicialização dos pinos GPIO é feita pelo CubeMX (MX_GPIO_Init)

    // Inicialização no modo 4 bits (conforme seu arquivo original)
    HAL_Delay(50);      // Espera > 40ms
    send_to_lcd(0x30, 0); // Comando de função
    HAL_Delay(5);       // Espera > 4.1ms
    send_to_lcd(0x30, 0); // Comando de função
    HAL_Delay(1);       // Espera > 100us
    send_to_lcd(0x30, 0); // Comando de função
    HAL_Delay(1);
    send_to_lcd(0x20, 0); // Comando para modo 4 bits
    HAL_Delay(1);

    // Inicialização do display
    send_to_lcd(0x28, 0); // Função set: DL=0 (4 bits), N=1 (2 linhas), F=0 (Fonte 5x8)
    HAL_Delay(1);
    send_to_lcd(0x08, 0); // Display off
    HAL_Delay(1);
    send_to_lcd(0x01, 0); // Limpa display
    HAL_Delay(2);       // Este comando demora mais
    send_to_lcd(0x06, 0); // Entry mode set: I/D=1 (incrementa cursor), S=0 (não shift)
    HAL_Delay(1);
    send_to_lcd(0x0C, 0); // Display on/off control: D=1 (Display ON), C=0, B=0
}

void lcd_send_data(char data)
{
    send_to_lcd(data, 1); // rs=1 para dados
}

void lcd_clear(void)
{
    send_to_lcd(0x01, 0); // Comando para limpar o LCD
    HAL_Delay(2); // O comando clear demora mais para executar
}

void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80; // Endereço da Linha 1
            break;
        case 1:
            col |= 0xC0; // Endereço da Linha 2
            break;
    }
    send_to_lcd(col, 0); // Envia o comando de posição do cursor
}

void lcd_send_string(char *str)
{
    while (*str) // Loop enquanto não for o caractere nulo '\0'
    {
        lcd_send_data(*str++); // Envia o caractere e avança o ponteiro
    }
}
