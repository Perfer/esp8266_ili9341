#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#include <ets_sys.h>
#include <osapi.h>
#include <os_type.h>
#include <gpio.h>
#include "driver/uart.h"
#include "hspi.h"

// See eagle_soc.h for these definitions
#define LED_GPIO_1 4
#define LED_GPIO_1_SET_PIN_FUNC PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4)
#define LED_GPIO_2 12
#define LED_GPIO_2_SET_PIN_FUNC PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12)
#define LED_GPIO_3 13
#define LED_GPIO_3_SET_PIN_FUNC PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13)

// Delay timer in milliseconds
#define DELAY 1000


#endif
