#ifndef LCD1602_H_
#define LCD1602_H_

#include "stm32f4xx.h"

void GPIO_init(void);
void send_to_lcd(int data, int rs);
void lcd_send_cmd(char cmd);
void lcd_init(void);
void lcd_send_data(char data);
void lcd_clear(void);
void lcd_put_cur(int row, int col);
void lcd_send_string(char *str);

#endif /* LCD1602_H_ */
