#include "stm32f4xx.h"
#include "tim.h"



void GPIO_init(void){
	// Habilita clock para GPIOA, GPIOB, GPIOC
	RCC->AHB1ENR |= (1U<<0) | (1U<<1) | (1U<<2);

	GPIOA->MODER |= (1U<<18);	//Pino 9 como saída - RS
	GPIOA->MODER &=~(1U<<19);

	GPIOC->MODER |= (1U<<14);	//Pino 7 como saída - EN
	GPIOC->MODER &=~(1U<<15);

	GPIOB->MODER |= (1U<<10);	//Pino 5 como saída - D4
	GPIOB->MODER &=~(1U<<11);

	GPIOB->MODER |= (1U<<8);	//Pino 4 como saída - D5
	GPIOB->MODER &=~(1U<<9);

	GPIOB->MODER |= (1U<<20);	//Pino 10 como saída - D6
	GPIOB->MODER &=~(1U<<21);

	GPIOA->MODER |= (1U<<16);	//Pino 8 como saída - D7
	GPIOA->MODER &=~(1U<<17);

	GPIOA->MODER |= (1U<<10);	//LED
	GPIOA->MODER &=~(1U<<11);

}

void send_to_lcd (int data, int rs){
	// rs = 1 para dados , rs = 0 para comando
	if (rs == 1){
		GPIOA->ODR |= (1U<<9);
	}else if (rs == 0){
		GPIOA->ODR &=~(1U<<9);
	}

	if (((data>>3)&0x01) == 1){
		GPIOA->ODR |= (1U<<8);		//d7 pin
	}else {
		GPIOA->ODR &=~(1U<<8);
	}

	if (((data>>2)&0x01) == 1){
		GPIOB->ODR |= (1U<<10);		//d6 pin
	}else {
		GPIOB->ODR &=~(1U<<10);
	}

	if (((data>>1)&0x01) == 1){
		GPIOB->ODR |= (1U<<4);		//d5 pin
	}else {
		GPIOB->ODR &=~(1U<<4);
	}

	if (((data>>0)&0x01) == 1){
		GPIOB->ODR |= (1U<<5);		//d4 pin
	}else {
		GPIOB->ODR &=~(1U<<5);
	}

	// Habilita o enable do LCD
	GPIOC->ODR |= (1U<<7);
	delayLCD(100);

	// Desabilita o enable do LCD
	GPIOC->ODR &=~(1U<<7);
	delayLCD(100);
}

void lcd_send_cmd(char cmd){
	char datatosend;
	/* Envia o nibble superior do byte */
	datatosend = ((cmd>>4)&0x0f);
	send_to_lcd(datatosend, 0); // RS deve ser 0 enquanto enviando um comando

	/* Envia o nibble inferior do byte */
	datatosend = ((cmd)&0x0f);
	send_to_lcd(datatosend, 0);
}

void lcd_init(void){
	// Inicialização no modo 4 bits
	delay_ms(50); 			// Espera > 40ms
	lcd_send_cmd(0x30);
	delay_ms(5);			// Espera por > 4.1ms
	lcd_send_cmd(0x30);
	delay_ms(1);			//Espera por > 100us
	lcd_send_cmd(0x30);
	delay_ms(1);			//Espera por > 100us
	lcd_send_cmd(0x20);		// mode 4 bits
	delay_ms(1);			//Espera por > 100us

	// Inicialização do display
	lcd_send_cmd(0x28);		// Função set --> DL=0 (modo 4 bits), N = 1 (2 linhas) F = 0 (Fonte 5x8)
	delay_ms(50);
	lcd_send_cmd(0x08);		// Controle on/off do display --> D=0, C=0, B=0 --> display off
	delay_ms(50);
	lcd_send_cmd(0x01);		// limpa display
	delay_ms(50);
	delay_ms(50);
	lcd_send_cmd(0x06);		// Entry mode set --> I/D = 1 (incrementa cursor) & S = 0 (não shift)
	delay_ms(50);
	lcd_send_cmd(0x0C);		// Display on/off control --> D = 1, C e B = 0. (Cursor e blink, últimos 2 bits)

}

void lcd_send_data (char data){
	char datatosend;

	/* Envia o nibble superior do byte */
	datatosend = ((data>>4)&0x0f);
	send_to_lcd(datatosend,1);		// RS deve ser 1 enquanto enviando um dado

	/* Envia o nibble inferior do byte */
	datatosend = ((data)&0x0f);
	send_to_lcd(datatosend, 1);

}

void lcd_clear(void){
	// comando 01 - Limpa LCD
	lcd_send_cmd(0x01);
	delay_ms(100);
}

void lcd_put_cur(int row, int col){
	switch (row){
	case 0:
		col |= 0x80;	// 0x80 = Endereço da 1a col. na linha 1
		break;
	case 1:
		col |= 0xC0;	// 0xC0 = Endereço da 2a col. na linha 1
		break;
	}

	lcd_send_cmd(col);	// Posiciona o curso no endereço a ser escrito no LCD
}

void lcd_send_string(char *str){
	while (*str) lcd_send_data (*str++);
}














