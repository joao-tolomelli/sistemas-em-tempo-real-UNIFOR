#include "stm32f4xx.h"
#include "LCD1602.h"
#include "keypad.h"
#include "stdio.h"
#include "tim.h"
#include "uart.h"

int row = 0;
int col = 0;

// 2. Variável global para armazenar o caractere recebido via serial
volatile char received_char = ' '; // Inicia com um espaço em branco

// 3. Rotina de Tratamento de Interrupção (ISR) para a USART2
void USART2_IRQHandler(void)
{
    // Verifica se a interrupção foi causada por recepção de dados (RXNE)
    if (USART2->SR & (1U<<5)) // SR_RXNE
    {
        // Lê o dado do registrador de dados (isso também limpa a flag de interrupção)
        received_char = USART2->DR;
    }
}


int main(void)
{
    uint16_t key_val = 0;

    static char key_name[][7] = {"RIGHT", "UP", "DOWN", "LEFT", "SELECT", "NONE"};

    GPIO_init();
    tim2_init();
    keypad_init();
    lcd_init();
    uart2_rxtx_init(); // 4. Inicializa a UART (TX e RX com interrupção)

    // Requisito 1: Mensagem inicial
    lcd_put_cur(0,0);
    lcd_send_string("SIST. TEMPO REAL");

    lcd_put_cur(1, 0);
    lcd_send_string("* U N I F O R *");

    // Requisito 2: Delay de 3s
    delay(3000);

    // Requisito 3: Tela de espera
    lcd_clear();
    lcd_put_cur(0, 0);
    lcd_send_string ("TECLA ACIONADA:");

    int bounce = 0;

    while (1)
    {
        key_val = keypad_read_key();

        if ((key_val == 4) & (bounce == 0))
        {
            bounce++;
        }
        else
        {
            // Exibe "NONE" ou a tecla pressionada
        	lcd_put_cur(1, 0);
        	lcd_send_string (key_name[key_val]);

            // Limpa a área de nome da tecla
        	lcd_send_string("       "); // 7 espaços são suficientes para limpar "SELECT" e "RIGHT" etc.

            // Adiciona o caractere serial na coluna 7
            lcd_put_cur(1, 7); // Posiciona na coluna 7 da linha 2
            lcd_send_data(received_char); // Escreve o último caractere recebido

        	delay(100);
            bounce = 0;
        }
    }
}
