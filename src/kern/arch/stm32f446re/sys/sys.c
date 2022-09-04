#include "../include/sys/sys.h"

extern volatile uint32_t mscount;

void sysTick_init(void){
    SysTick->CTRL &= ~(1 << 0); // reset 0th bit
    SysTick->CTRL |= (1 << 2); // processor clock source
    SysTick->CTRL |= (1 << 1); //throws exception
    SysTick->VAL = 0; // reset value 
    SysTick->LOAD = 1799999;
    mscount = 0;
    SysTick->CTRL |= (1 << 0); // set 0th bit
}
void sysTick_enable(void) {
    if(SysTick->CTRL & (1 << 0)) return;
    mscount = 0;
    SysTick->CTRL |= (1 << 0); // set 0th bit
}

void sysTick_disable(void) {
    SysTick->CTRL &= ~(1 << 0); // reset 0th bit
}

uint32_t getSysTickCount(void) {
    return SysTick->LOAD - SysTick->VAL;
}

void updateSysTick(uint32_t val) {
    sysTick_disable();
    SysTick->LOAD = val;
    mscount = 0;
}

uint32_t getTime(void) {
    uint32_t load_val = (SysTick->LOAD+1)/10;
    return mscount + (getSysTickCount()/load_val); // 1 ms = 180 K clock cycles 
}

void SysTick_Handler(void)
{
    const uint32_t clock = 180000000;
    mscount += 1000/(clock/(SysTick->LOAD+1));
}