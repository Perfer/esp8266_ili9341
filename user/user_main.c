/*
 *  Example of working ESP8266 and 2.2 Inch SPI TFT LCD Serial Port Module Display ILI9341
 *  Specification on ILI9341 - http://www.newhavendisplay.com/app_notes/ILI9341.pdf
 *  Specification on ESP8266 - http://www.esp8266.com/
 *
 *  Original source https://github.com/Perfer/esp8266_ili9341
 *
 *  Author: Semen Sachkov
 *
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
	ets_uart_printf("Degree: %d \r\n", (int)degree);
}

void user_init(void)
{
	// Configure the UART
	uart_init(BIT_RATE_115200,BIT_RATE_115200);
	ets_uart_printf("\r\nSystem init...\r\n");
	// Initialize TFT
	tft_init();
	// Set up a timer to send the message
	os_timer_disarm(&hello_timer);
	os_timer_setfn(&hello_timer, (os_timer_func_t *)hello_cb, (void *)0);
	os_timer_arm(&hello_timer, DELAY, 1);
	ets_uart_printf("System init done \r\n");
}
