/*
	The hello world demo with blink led
*/

#include "user_config.h"

extern int ets_uart_printf(const char *fmt, ...);

LOCAL os_timer_t hello_timer;
LOCAL uint32_t displayID = 0;

#define CS_ACTIVE	GPIO_OUTPUT_SET(4, 1)
#define CS_DEACTIVE GPIO_OUTPUT_SET(4, 0)
#define CS_INIT		PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4); CS_DEACTIVE

#define DATA_SIZE	2

LOCAL void ICACHE_FLASH_ATTR hello_cb(void *arg)
{
	uint8_t data[DATA_SIZE] = {0x01, 0x23};
	uint16_t temperature = 0;

	CS_ACTIVE;
	hspi_TxRx(data, DATA_SIZE);
	temperature = (((data[0] << 8) + data[1]) >> 5) / 4;
	CS_DEACTIVE;

	ets_uart_printf("AD7814 temperature: %d deg.C\r\n", temperature);
}

void user_init(void)
{
	// Configure the UART
	uart_init(BIT_RATE_9600,BIT_RATE_9600);
	CS_INIT;
	// Initialize hspi
	hspi_init();
	// Set up a timer to send the message
	os_timer_disarm(&hello_timer);
	os_timer_setfn(&hello_timer, (os_timer_func_t *)hello_cb, (void *)0);
	os_timer_arm(&hello_timer, DELAY, 1);
}
