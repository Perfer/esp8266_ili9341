/*
	The hello world demo with blink led
*/

#include "user_config.h"

extern int ets_uart_printf(const char *fmt, ...);
extern void ets_wdt_enable (void);
extern void ets_wdt_disable (void);

LOCAL os_timer_t hello_timer;

LOCAL uint16_t nextX = 0;
LOCAL uint16_t nextY = 0;
LOCAL uint16_t color = 0xFFFF;

LOCAL void ICACHE_FLASH_ATTR hello_cb(void *arg)
{
	tft_drawLine(nextX, 0, 240 - nextX, 320, color);
	nextX += 24;
	nextY += 32;
	if ((nextX > 240) && (nextY > 320))
	{
		nextX = 0;
		nextY = 0;
		color = (color == 0) ? 0xFFFF : color << 4;
	}
	ets_uart_printf("Display ID: %d \r\n", tft_readId());
}

void user_init(void)
{
	ets_wdt_enable();
	ets_wdt_disable();
	// Configure the UART
	uart_init(BIT_RATE_9600,BIT_RATE_9600);
	// Set up a timer to send the message
	os_timer_disarm(&hello_timer);
	os_timer_setfn(&hello_timer, (os_timer_func_t *)hello_cb, (void *)0);
	os_timer_arm(&hello_timer, DELAY, 1);
	// Initialize TFT
	tft_init();

}
