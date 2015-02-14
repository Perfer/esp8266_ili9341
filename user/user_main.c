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

LOCAL os_timer_t timerHandler;

static void test(void)
{
//	tft_drawStreamString("Troom = 27.0\n",  1);
//	tft_drawStreamString("T(75km): -24.7\n", 2);
	tft_drawString("12:42:32\n18:12:54", 0, 0, 2, 0xFFFF, 0x0000);

	tft_drawString("12:42:34", 0, 40, 2, 0xFFFF, 0x0000);
	tft_drawString("00:01:18", 0, 60, 2, 0xFFFF, 0x0000);
}

LOCAL void ICACHE_FLASH_ATTR sendMsgToHandler(void *arg)
{
	system_os_post(USER_TASK_PRIO_0, RUN_TEST, 'a');
}

void handler_task (os_event_t *e)
{
	switch (e->sig)
	{
		case RUN_TEST: test(); break;
		default: break;
	}
}

void user_init(void)
{
	os_event_t *handlerQueue;
	// Configure the UART
	uart_init(BIT_RATE_115200,BIT_RATE_115200);
	ets_uart_printf("\r\nSystem init...\r\n");

	// Initialize TFT
	tft_init();

	// Set up a timer to send the message to handler
	os_timer_disarm(&timerHandler);
	os_timer_setfn(&timerHandler, (os_timer_func_t *)sendMsgToHandler, (void *)0);
	os_timer_arm(&timerHandler, DELAY_TIMER, 1);

	// Set up a timerHandler to send the message to handler
	handlerQueue = (os_event_t *)os_malloc(sizeof(os_event_t)*TEST_QUEUE_LEN);
	system_os_task(handler_task, USER_TASK_PRIO_0, handlerQueue, TEST_QUEUE_LEN);

	ets_uart_printf("System init done \r\n");
}
