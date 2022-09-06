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
    NVIC->IP[IRQn] = (uint8_t)((priority << 4) & 0xFF);
}
uint32_t __NVIC_GetPriority(enum IRQn_TypeDef IRQn)
{
    return (NVIC->IP[IRQn] >> 4);
}

uint32_t __NVIC_GetActive(enum IRQn_TypeDef IRQn)
{
    return (NVIC->IABR[IRQn >> 5] & 1 << (IRQn & 0x1F)) != 0;
}
void __NVIC_EnableIRQn(enum IRQn_TypeDef IRQn)
{
    NVIC->ISER[IRQn >> 5] = 1 << (IRQn & 0x1F);
}

void __NVIC_DisableIRQn(enum IRQn_TypeDef IRQn)
{
    NVIC->ICER[IRQn >> 5] = 1 << (IRQn & 0x1F);
}

uint32_t __get_pending_IRQn(enum IRQn_TypeDef IRQn)
{
    return (NVIC->ISPR[IRQn >> 5] & 1 << (IRQn & 0x1F)) != 0;
}

void __clear_pending_IRQn(enum IRQn_TypeDef IRQn)
{
    NVIC->ICPR[IRQn >> 5] = 1 << (IRQn & 0x1F);
}

void __enable_irq()
{
    __set_PRIMASK(0);
}

void __disable_irq()
{
    __set_PRIMASK(1);
}

void __unset_BASEPRI(uint32_t value)
{
    __set_BASEPRI(0);   // Not sure about this line weather it is required or not :)
    __set_BASEPRI(value);
}

void __set_BASEPRI(uint32_t value)
{
  __asm volatile ("MSR basepri, %0" : : "r" (value) );
}

uint32_t __get_PRIMASK()
{
  uint32_t result;

  __asm volatile ("MRS %0, primask" : "=r" (result) );
  return result;
}

void __set_PRIMASK(uint32_t priMask)
{
  __asm volatile ("MSR primask, %0" : : "r" (priMask) );
}

uint32_t __get_FAULTMASK()
{
  uint32_t result;

  __asm volatile ("MRS %0, faultmask" : "=r" (result) );
  return result;
}

void __set_FAULTMASK(uint32_t faultMask)
{
  __asm volatile ("MSR primask, %0" : : "r" (faultMask) );
}

void __enable_fault_irq()
{
    __set_FAULTMASK(1);
}

void __disable_fault_irq()
{
    __set_FAULTMASK(0);
}

void __enable_primask()
{
    __set_PRIMASK(1);
}

void __disable_primask()
{
    __set_PRIMASK(0);
}
