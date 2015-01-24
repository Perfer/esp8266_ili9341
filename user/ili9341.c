/*
 * ili9341.c


 *
 *  Created on: 12 янв. 2015 г.
 *      Author: Sem
 */

#include "ili9341.h"

static void transmitCmdData(uint8_t cmd, uint8_t * data, uint8_t numDataByte)
{
    TFT_CS_ACTIVE;
   	TFT_DC_COMMAND;
   	hspi_TxRx(&cmd, 1);
    if (numDataByte > 0)
    {
    	TFT_DC_DATA;
        hspi_TxRx(data, numDataByte);
    }
    TFT_CS_DEACTIVE;
}

static void transmitData(uint8_t *data, uint8_t numByte)
{
	TFT_DC_DATA;
    TFT_CS_ACTIVE;
    hspi_TxRx(data, numByte);
    TFT_CS_DEACTIVE;
}

static uint8_t readRegister(uint8_t cmd, uint8_t numParameter)
{
    uint8_t data = 0;
    data = 0x10 + numParameter;
    transmitCmdData(0xD9, &data, 1); // secret command and 0x11 is the first Parameter
    transmitCmdData(cmd, &data, 1);
    return data;
}

uint32_t tft_readId(void)
{
    uint8_t i = 0;
	uint32_t_bytes id = {0};

    id.bytes.b2 = readRegister(0xd3, 1);
    id.bytes.b1 = readRegister(0xd3, 2);
    id.bytes.b0 = readRegister(0xd3, 3);

    return  id.all;
}

void tft_init(void)
{
	uint8_t data[15] = {0};

	hspi_init();

	TFT_CS_INIT;
	TFT_DC_INIT;
	TFT_RST_INIT;

	TFT_RST_ACTIVE;
	os_delay_us(10000);
	TFT_RST_DEACTIVE;
	os_delay_us(1000);

	tft_readId();

	data[0] = 0x39;
	data[1] = 0x2C;
	data[2] = 0x00;
	data[3] = 0x34;
	data[4] = 0x02;
	transmitCmdData(0xCB, data, 5);

	data[0] = 0x00;
	data[1] = 0XC1;
	data[2] = 0X30;
	transmitCmdData(0xCF, data, 3);

	data[0] = 0x85;
	data[1] = 0x00;
	data[2] = 0x78;
	transmitCmdData(0xE8, data, 3);

	data[0] = 0x00;
	data[1] = 0x00;
	transmitCmdData(0xEA, data, 2);

	data[0] = 0x64;
	data[1] = 0x03;
	data[2] = 0X12;
	data[3] = 0X81;
	transmitCmdData(0xED, data, 4);

	data[0] = 0x20;
	transmitCmdData(0xF7, data, 1);

	data[0] = 0x23;   	//VRH[5:0]
	transmitCmdData(0xC0, data, 1);    	//Power control

	data[0] = 0x10;   	//SAP[2:0];BT[3:0]
	transmitCmdData(0xC1, data, 1);    	//Power control

	data[0] = 0x3e;   	//Contrast
	data[1] = 0x28;
	transmitCmdData(0xC5, data, 2);    	//VCM control

	data[0] = 0x86;  	 //--
	transmitCmdData(0xC7, data, 1);    	//VCM control2

	data[0] = 0x48;  	//C8	   //48 68绔栧睆//28 E8 妯睆
	transmitCmdData(0x36, data, 1);    	// Memory Access Control

	data[0] = 0x55;
	transmitCmdData(0x3A, data, 1);

	data[0] = 0x00;
	data[1] = 0x18;
	transmitCmdData(0xB1, data, 2);

	data[0] = 0x08;
	data[1] = 0x82;
	data[2] = 0x27;
	transmitCmdData(0xB6, data, 3);    	// Display Function Control

	data[0] = 0x00;
	transmitCmdData(0xF2, data, 1);    	// 3Gamma Function Disable

	data[0] = 0x01;
	transmitCmdData(0x26, data, 1);    	//Gamma curve selected

	data[0] = 0x0F;
	data[1] = 0x31;
	data[2] = 0x2B;
	data[3] = 0x0C;
	data[4] = 0x0E;
	data[5] = 0x08;
	data[6] = 0x4E;
	data[7] = 0xF1;
	data[8] = 0x37;
	data[9] = 0x07;
	data[10] = 0x10;
	data[11] = 0x03;
	data[12] = 0x0E;
	data[13] = 0x09;
	data[14] = 0x00;
	transmitCmdData(0xE0, data, 15);    	//Set Gamma

	data[0] = 0x00;
	data[1] = 0x0E;
	data[2] = 0x14;
	data[3] = 0x03;
	data[4] = 0x11;
	data[5] = 0x07;
	data[6] = 0x31;
	data[7] = 0xC1;
	data[8] = 0x48;
	data[9] = 0x08;
	data[10] = 0x0F;
	data[11] = 0x0C;
	data[12] = 0x31;
	data[13] = 0x36;
	data[14] = 0x0F;
	transmitCmdData(0xE1, data, 15);    	//Set Gamma

	transmitCmdData(0x11, 0, 0);    	//Exit Sleep
	os_delay_us(120000);

	transmitCmdData(0x29, 0, 0);    //Display on
	transmitCmdData(0x2c, 0, 0);
	tft_fillScreen();
}

static void setCol(uint16_t start, uint16_t end)
{
	uint8_t data[4] = {start >> 8, start & 0xFF, end >> 8, end & 0xFF};
	transmitCmdData(0x2A, (uint8_t *)data, 4);	//Column Command address
}

static void setPage(uint16_t start, uint16_t end)
{
	uint8_t data[4] = {start >> 8, start & 0xFF, end >> 8, end & 0xFF};
	transmitCmdData(0x2B, (uint8_t *)data, 4);	//Column Command address
}

#define PAKET_SIZE			8
#define AMOUNT_TRANSMIT		320 * 240 * 2 / PAKET_SIZE

void tft_fillScreen(void)
{
	uint32_t i = 0;

    setCol(0, 239);
    setPage(0, 319);
    transmitCmdData(0x2C, 0, 0);//  start to write to display ram

    for(i = 0; i < AMOUNT_TRANSMIT; i++)
    {
    	uint8_t data[PAKET_SIZE] = {0xF8,0x00,0xF8,0x00, 0xF8,0x00, 0xF8,0x00};
    	transmitData((uint8_t *)data, PAKET_SIZE);
    }

}
