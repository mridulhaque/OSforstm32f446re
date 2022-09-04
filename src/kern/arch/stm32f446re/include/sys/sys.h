#ifndef SYS_H
#define SYS_H 
#include <stdint.h>

typedef struct {
    volatile uint32_t CTRL; 
    volatile uint32_t LOAD; 
    volatile uint32_t VAL; 
    volatile uint32_t CALIB; 
} SysTick_Typedef;

#define SysTick ((SysTick_Typedef *) 0xE000E010 )

void sysTick_init(void);
void sysTick_enable(void);
void sysTick_disable(void);
uint32_t getSysTickCount(void);
void updateSysTick(uint32_t);
uint32_t getTime(void);

#endif
