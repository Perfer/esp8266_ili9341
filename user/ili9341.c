/*
 * ili9341.c


 *
 *  Created on: 12 џэт. 2015 у.
 *      Author: Sem
 */

#include "ili9341.h"

static void transmitCmd(uint8_t data)
{
	TFT_DC_COMMAND;
    TFT_CS_ACTIVE;
    hspi_TxRx(&data, 1);
    TFT_CS_DEACTIVE;
}

static void transmitDataN(uint8_t *data, uint8_t numByte)
{
	TFT_DC_DATA;
    TFT_CS_ACTIVE;
    hspi_TxRx(data, numByte);
    TFT_CS_DEACTIVE;
}

static void transmitData1(uint8_t data)
{
	transmitDataN(&data, 1);
}

/*
static void transmitData2(uint16_t data)
{
	transmitDataN(&data, 2);
}
*/

static uint8_t readRegister(uint8_t cmd, uint8_t numParameter)
{
    uint8_t data = 0;
    transmitCmd(0xD9); // secret command
    transmitData1(0x10 + numParameter);// 0x11 is the first Parameter

    TFT_DC_COMMAND;
    TFT_CS_ACTIVE;
    hspi_TxRx(&cmd, 1);
    TFT_DC_DATA;
    hspi_TxRx(&data, 1);
    TFT_CS_DEACTIVE;
    return data;
}

uint32_t tft_readId(void)
{
    uint8_t i = 0;
	uint32_t_bytes id = {0};

	TFT_RST_ACTIVE;
	os_delay_us(10000);
	TFT_RST_DEACTIVE;

	os_delay_us(1000);

    id.bytes.b2 = readRegister(0xd3, 1);
    id.bytes.b1 = readRegister(0xd3, 2);
    id.bytes.b0 = readRegister(0xd3, 3);

    return  id.all;
}

void tft_init(void)
{
	hspi_init();
	TFT_CS_INIT;
	TFT_DC_INIT;
	TFT_RST_INIT;
}

