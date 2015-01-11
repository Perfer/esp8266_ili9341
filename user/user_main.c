/*
	The hello world demo with blink led
*/

#include <ets_sys.h>
#include <osapi.h>
#include <os_type.h>
#include <gpio.h>
#include "driver/uart.h"

// see eagle_soc.h for these definitions
#define LED_GPIO_1 4
#define LED_GPIO_1_SET_PIN_FUNC PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4)
#define LED_GPIO_2 12
#define LED_GPIO_2_SET_PIN_FUNC PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12)
#define LED_GPIO_3 13
#define LED_GPIO_3_SET_PIN_FUNC PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13)


#define DELAY 1000 /* milliseconds */

extern void wdt_feed (void);

LOCAL os_timer_t hello_timer;
LOCAL uint8_t led_state = 1;

extern int ets_uart_printf(const char *fmt, ...);

LOCAL void ICACHE_FLASH_ATTR hello_cb(void *arg)
{
	ets_uart_printf("Led %d \r\n", led_state);
	led_state = (led_state == 3) ? 1 : led_state + 1;
	switch (led_state)
	{
		case 1:
			GPIO_OUTPUT_SET(LED_GPIO_1, 1);
			GPIO_OUTPUT_SET(LED_GPIO_2, 0);
			GPIO_OUTPUT_SET(LED_GPIO_3, 0);
		break;

		case 2:
			GPIO_OUTPUT_SET(LED_GPIO_1, 0);
			GPIO_OUTPUT_SET(LED_GPIO_2, 1);
			GPIO_OUTPUT_SET(LED_GPIO_3, 0);
		break;

		case 3:
			GPIO_OUTPUT_SET(LED_GPIO_1, 0);
			GPIO_OUTPUT_SET(LED_GPIO_2, 0);
			GPIO_OUTPUT_SET(LED_GPIO_3, 1);
		break;
	}
	wdt_feed();
}

void user_init(void)
{
	// Configure the UART
	uart_init(BIT_RATE_9600,BIT_RATE_9600);
	// Configure pin as a GPIO
	LED_GPIO_1_SET_PIN_FUNC;
	LED_GPIO_2_SET_PIN_FUNC;
	LED_GPIO_3_SET_PIN_FUNC;
	// Set up a timer to send the message
	// os_timer_disarm(ETSTimer *ptimer)
	os_timer_disarm(&hello_timer);
	// os_timer_setfn(ETSTimer *ptimer, ETSTimerFunc *pfunction, void *parg)
	os_timer_setfn(&hello_timer, (os_timer_func_t *)hello_cb, (void *)0);
	// void os_timer_arm(ETSTimer *ptimer,uint32_t milliseconds, bool repeat_flag)
	os_timer_arm(&hello_timer, DELAY, 1);
}
