#include "stm32f4xx.h"
#include "LCD1602.h"
#include "keypad.h"
#include "stdio.h"
#include "tim.h"

int row = 0;
int col = 0;

int main(void)
{
    uint16_t key_val = 0;

    static char key_name[][7] = {"RIGHT", "UP", "DOWN", "LEFT", "SELECT", "NONE"};

    GPIO_init();
    tim2_init();
    keypad_init();
    lcd_init();

    lcd_put_cur(0,0);
    lcd_send_string("SIST. TEMPO REAL");

    lcd_put_cur(1, 0);
    lcd_send_string("* U N I F O R *");
    delay(3000);

    lcd_clear();
    lcd_put_cur(0, 0);
    lcd_send_string ("TECLA ACIONADA:");

    int bounce = 0;

    while (1)
    {
        key_val = keypad_read_key();
        if ((key_val == 4) && (bounce == 0))
        {
            bounce++;
        }
        else
        {
        	lcd_put_cur(1, 0);
        	lcd_send_string (key_name[key_val]);
        	lcd_send_string("    ");
        	delay(100);
            bounce = 0;
        }
    }
}






