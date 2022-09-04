#include "../include/sys_init.h"
#include "../include/kmain.h"
#include "../include/kstdio.h"
#include "../arch/stm32f446re/include/sys/sys.h"

#include <stdint.h>

void kmain(void)
{
	__sys_init();	
	kprintf((uint8_t*)"%s",(uint8_t*)"\r\n\r\nWelcome to duOS....\r\n\r\n");

	while(1)
	{
		// updateSysTick(1799999/2);
		sysTick_enable();
		kprintf((uint8_t*)"%s",(uint8_t*)"init_time: ");
		uint32_t init_time = getTime();
		kprintf((uint8_t*)"%d",(uint8_t*)&init_time);
		kprintf((uint8_t*)"%s",(uint8_t*)" ms\r\n");

		uint32_t sum = 0;
		for(uint32_t i = 1; i <= 1000000; i++) {
			sum += 1;
		}
		uint32_t final_time = getTime();
		kprintf((uint8_t*)"%s",(uint8_t*)"final_time: ");
		kprintf((uint8_t*)"%d",(uint8_t*)&final_time);
		kprintf((uint8_t*)"%s",(uint8_t*)" ms\r\n");
		kprintf((uint8_t*)"%s",(uint8_t*)"sum = ");
		kprintf((uint8_t*)"%d",(uint8_t*)&sum);
		kprintf((uint8_t*)"%s",(uint8_t*)"\r\n");

		uint32_t timeTaken = final_time - init_time;
		kprintf((uint8_t*)"%s",(uint8_t*)"time_taken: ");
		kprintf((uint8_t*)"%d",(uint8_t*)&timeTaken);
		kprintf((uint8_t*)"%s",(uint8_t*)" ms\r\n");

		while(getTime() - init_time <= 1000);
		kprintf((uint8_t*)"%s",(uint8_t*)"\r\n\r\n");

		sysTick_disable();
	}


}

