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


void __NVIC_SetPriority(enum IRQn_TypeDef IRQn,uint32_t priority)
{

}
uint32_t __NVIC_GetPriority(enum IRQn_TypeDef IRQn)
{

}
uint32_t __NVIC_GetActive(enum IRQn_TypeDef IRQn)
{

}
void __NVIC_EnableIRQn(enum IRQn_TypeDef IRQn)
{

}
void __NVIC_DisableIRQn(enum IRQn_TypeDef IRQn)
{

}

uint32_t __get_pending_IRQn(enum IRQn_TypeDef IRQn)
{

}
void __clear_pending_IRQn(enum IRQn_TypeDef IRQn)
{

}

void __enable_irq()
{

}
void __disable_irq()
{

}

void __unset_BASEPRI(uint32_t value)
{

}
void __set_BASEPRI(uint32_t value)
{

}

uint32_t __get_PRIMASK()
{

}
void __set_PRIMASK(uint32_t priMask)
{

}
uint32_t __get_FAULTMASK()
{

}

void __enable_fault_irq()
{

}
void __disable_fault_irq()
{

}