#include "../include/sys_init.h"
#include "../include/kmain.h"
#include "../include/kstdio.h"
#include "../arch/stm32f446re/include/sys/sys.h"

#include <stdint.h>

void testTime(void)
{
	kprintf((uint8_t*)"%s",(uint8_t*)"Loop : ");
	// kprintf((uint8_t*)"%s",(uint8_t*)"init_time: ");
	uint32_t init_time = getTime();
	// kprintf((uint8_t*)"%d",(uint8_t*)&init_time);
	// kprintf((uint8_t*)"%s",(uint8_t*)" ms\r\n");

	uint32_t sum = 0;
	for(uint32_t i = 1; i <= 10000000; i++) {
		sum += 1;
	}
	uint32_t final_time = getTime();
	// kprintf((uint8_t*)"%s",(uint8_t*)"final_time: ");
	// kprintf((uint8_t*)"%d",(uint8_t*)&final_time);
	// kprintf((uint8_t*)"%s",(uint8_t*)" ms\r\n");
	// kprintf((uint8_t*)"%s",(uint8_t*)"sum = ");
	// kprintf((uint8_t*)"%d",(uint8_t*)&sum);
	// kprintf((uint8_t*)"%s",(uint8_t*)"\r\n");

	uint32_t timeTaken = final_time - init_time;
	kprintf((uint8_t*)"%s",(uint8_t*)"\r\ntime_taken: ");
	kprintf((uint8_t*)"%d",(uint8_t*)&timeTaken);
	kprintf((uint8_t*)"%s",(uint8_t*)" ms\r\n");
	kprintf((uint8_t*)"%s",(uint8_t*)"\r\n");
}

void TIM7_Handler(void)
{
	uint32_t initTime = getTime();
	if (TIM7->SR & (1 << 0)) {
		TIM7->SR &= ~(1 << 0);		
	}
	kprintf((uint8_t*)"%s",(uint8_t*)"\r\n\r\nWelcome to TIM7 interrupt....\r\n\r\n");
	uint32_t t = getTime();
	kprintf((uint8_t*)"%d",(uint8_t*)&t);
	kprintf((uint8_t*)"%s",(uint8_t*)" ms\r\n");

	while(getTime() - initTime <= 1000);
}

void kmain(void)
{
	__sys_init();	
	kprintf((uint8_t*)"%s",(uint8_t*)"\r\n\r\nWelcome to duOS....\r\n\r\n");
	
	kprintf((uint8_t*)"%s",(uint8_t*)"Tesiting PRIMASK\r\n");

	int parity = 0;
	int cnt = 6;
	while(cnt--)
	{
		// updateSysTick(1799999/2);
		if(parity) __disable_irq();
		else __enable_irq();

		parity = !parity;

		testTime();
		// sysTick_disable();
		// break;
	}

	__enable_irq();

	kprintf((uint8_t*)"%s",(uint8_t*)"Priority set to 4\r\n");

	__NVIC_SetPriority(TIM7_IRQn, 4);
	// __set_BASEPRI(4);

	kprintf((uint8_t*)"%s",(uint8_t*)"TIM7_IRQn Enabled\r\n");

	__NVIC_EnableIRQn(TIM7_IRQn);

	cnt = 6;
	while(cnt--) testTime();

	kprintf((uint8_t*)"%s",(uint8_t*)"Tesiting BASEPRI\r\n");

	__set_BASEPRI(4);

	cnt = 6;

	while(cnt--) testTime();
	uint32_t x = __get_pending_IRQn(TIM7_IRQn);
	
	kprintf((uint8_t*)"%d",(uint8_t*)&x);
	kprintf((uint8_t*)"%s",(uint8_t*)"\r\n\r\n");

	__set_BASEPRI(0);

	kprintf((uint8_t*)"%s",(uint8_t*)"Tesiting FAULTMASK\r\n");

	__set_FAULTMASK(1);

	cnt = 6;
	while(cnt--) testTime();

	__set_FAULTMASK(0);

	kprintf((uint8_t*)"%s",(uint8_t*)"DISABLING TIM7_IRQn\r\n");


	__NVIC_DisableIRQn(TIM7_IRQn);
	
	cnt = 6;
	while(cnt--) testTime();
}

