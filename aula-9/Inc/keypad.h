#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "stm32f4xx.h"
#include "stdint.h"

#define KEY_RIGHT   0
#define KEY_UP      1
#define KEY_DOWN    2
#define KEY_LEFT    3
#define KEY_SELECT  4
#define KEY_NONE    5

uint8_t keypad_init(void);
uint16_t keypad_read_key(void);

#endif /* KEYPAD_H_ */








