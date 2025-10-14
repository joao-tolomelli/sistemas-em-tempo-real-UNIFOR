#ifndef TIM_H_
#define TIM_H_

#include "stm32f4xx.h"
#include "stdint.h"

#define TIM2EN      (1U<<0)
#define CR1_CEN     (1U<<0)
#define SR_UIF      (1U<<0)

void tim2_init(void);
void delay(uint16_t ms);
void delay_ms(uint16_t us);
void delayLCD(uint16_t us);

#endif /* TIM_H_ */






