/*
	The hello world demo with blink led
*/

#include "user_config.h"

extern int ets_uart_printf(const char *fmt, ...);

LOCAL os_timer_t hello_timer;

LOCAL void ICACHE_FLASH_ATTR hello_cb(void *arg)
{
	uint32_t displayId = tft_readId();
	ets_uart_printf("Display ID: %d \r\n", displayId);
}

void user_init(void)
{
	// Configure the UART
	uart_init(BIT_RATE_9600,BIT_RATE_9600);
	// Initialize TFT
	tft_init();
	// Set up a timer to send the message
	os_timer_disarm(&hello_timer);
	os_timer_setfn(&hello_timer, (os_timer_func_t *)hello_cb, (void *)0);
	os_timer_arm(&hello_timer, DELAY, 1);
}
