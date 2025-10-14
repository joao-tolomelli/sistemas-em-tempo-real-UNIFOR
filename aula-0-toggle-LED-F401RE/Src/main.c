#define PERIPH_BASE         (0x40000000UL)   // região de periféricos
#define AHB1PERIPH_OFFSET   (0x00020000UL)
#define AHB1PERIPH_BASE     (PERIPH_BASE + AHB1PERIPH_OFFSET)
#define GPIOA_OFFSET        (0x0000UL)

#define GPIOA_BASE          (AHB1PERIPH_BASE + GPIOA_OFFSET)

#define RCC_OFFSET          (0x3800UL)
#define RCC_BASE            (AHB1PERIPH_BASE + RCC_OFFSET)

#define AHB1ENR_OFFSET      (0x30UL)
#define RCC_AHB1ENR_R       (*(volatile unsigned int *)(RCC_BASE + AHB1ENR_OFFSET))
#define GPIOAEN             (1U<<0)

#define MODE_R_OFFSET       (0x00UL)
#define GPIOA_MODE_R        (*(volatile unsigned int *)(GPIOA_BASE + MODE_R_OFFSET))

#define OD_R_OFFSET         (0x14UL)
#define GPIOA_OD_R          (*(volatile unsigned int *)(GPIOA_BASE + OD_R_OFFSET))

/*
 * 1<<x    (1U<<10)   // Set bit 10 para 1
 * 0<<x    ~(1U<<11)   // Set bit 11 para 0
 */


#define PIN5                (1U<<5)
#define LED_PIN             PIN5

int main(void)
{
    /*1: Habilita o clock para acessar o GPIOA*/
    RCC_AHB1ENR_R |= GPIOAEN;   // liga sem mexer nos outros bits

    /*2:Set PA5 como pino de saída*/
    GPIOA_MODE_R |= (1U<<10);      // coloca o par [11:10] de PA5 em 01b (seta bit 10)
    GPIOA_MODE_R &=~(1U<<11);     // garante que o bit 11 está 0

        while(1)
    {
        /*3: faz o toggle em PA5*/
        GPIOA_OD_R ^= LED_PIN;
        for(int i=0;i<500000;i++){}
    }
}
