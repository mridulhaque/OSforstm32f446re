#include "../include/dev/timer.h"
#include "../include/sys/sys.h"
void DRV_TIMER_INIT()
{
    RCC->APB1ENR |= (1 << 5);
    TIM7->CR1 &= ~(1 << 0);
	TIM7->PSC = 42000-1;
	TIM7->ARR = 10000;

	TIM7->DIER |= (1 << 0);
    TIM7->CR1 |= (1 << 2);
    // TIM7->CR1 |= (1 << 3);

    TIM7->CR1 |= (1 << 0);
	while(!(TIM7->SR & (1<<0)));
    // __NVIC_EnableIRQn(TIM7_IRQn);
}


