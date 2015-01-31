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
	uint16_t i = 0;
	uint8_t nextASCII(void)
	{
		static int16_t ascii = 32;
		ascii = (ascii > 127) ? 32 : ascii + 1;
		return ascii;
	}
	uint8_t nextX0(void)
	{
		static uint8_t x0 = 0;
		x0 = (x0 > 239) ? 0 : x0 + 16;
		return x0;
	}
	tft_drawChar(nextASCII(), nextX0(), i, 2, 0xFFFF);
	tft_drawChar(nextASCII(), nextX0(), i, 2, 0xFFFF);
	tft_drawChar(nextASCII(), nextX0(), i, 2, 0xFFFF);
	tft_drawChar(nextASCII(), nextX0(), i, 2, 0xFFFF);
	tft_drawChar(nextASCII(), nextX0(), i, 2, 0xFFFF);
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
