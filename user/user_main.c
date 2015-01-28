/*
	The hello world demo with blink led
*/

#include "user_config.h"

extern int ets_uart_printf(const char *fmt, ...);

LOCAL os_timer_t hello_timer;
LOCAL double degree = -180.0;
LOCAL double scale = 1.0;
LOCAL double scale_inc = 0.025;

LOCAL void ICACHE_FLASH_ATTR hello_cb(void *arg)
{
	cube_draw(0);
	if (degree >= 180.0) degree = -180.0;
	if ((scale < 0.5) && (scale_inc < 0)) scale_inc = -scale_inc;
	if ((scale > 1.5) && (scale_inc > 0)) scale_inc = -scale_inc;
	cube_calculate(degree, degree, degree, scale, 0, 0, 0);
	degree += 2.0;
	scale += scale_inc;
	cube_draw(0xFFFF);
	ets_uart_printf("degree: %d \r\n", (int)degree);
}

void user_init(void)
{
	// Configure the UART
	uart_init(BIT_RATE_9600,BIT_RATE_9600);
	// Set up a timer to send the message
	os_timer_disarm(&hello_timer);
	os_timer_setfn(&hello_timer, (os_timer_func_t *)hello_cb, (void *)0);
	os_timer_arm(&hello_timer, DELAY, 1);
	// Initialize TFT
	tft_init();
}


