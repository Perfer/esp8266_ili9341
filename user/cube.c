/*
 * cube.c
 *
 *  Created on: 28 янв. 2015 г.
 *      Author: Sem
 */
#include "cube.h"
static const int16_t pix[AMOUNT_NODE][3] ={{-25,-25,-25},{25,-25,-25},{25,25,-25},{-25,25,-25},
											{-25,-25,25}, {25,-25,25}, {25,25,25}, {-25,25,25}};
static int16_t newpix[AMOUNT_NODE][3];


static void calcPerspectiveProjection(const int16_t * coordinate, int16_t * x, int16_t * y)
{
	*x = coordinate[0] + coordinate[2] / 2;
	*y = coordinate[1] - coordinate[2] / 2;
}

static void drawLine( int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t color)
{
	tft_drawLine(x0 + MAX_TFT_X/2, y0 + MAX_TFT_Y/2, x1 + MAX_TFT_X/2, y1 + MAX_TFT_Y/2, color);
}

void cube_calculate(double degreeX, double degreeY, double degreeZ, double scale, int16_t shiftX, int16_t shiftY, int16_t shiftZ)
{

	uint8_t i;
	double sinx, siny, sinz, cosx, cosy, cosz;
	double x,y,z,x1,y1,z1;

	cordic(degreeX, &sinx, &cosx);
	cordic(degreeY, &siny, &cosy);
	cordic(degreeZ, &sinz, &cosz);

	for (i = 0; i < AMOUNT_NODE; i++)
	{

	    x = pix[i][0];   //Исходное направление
	    y = pix[i][1];
	    z = pix[i][2];

	    x1 = x*cosz + y*sinz;       // вокруг Z
	    y1 = -x*sinz + y*cosz;
	    z1 = z;

	    x = x1;                     // вокруг X
	    y = y1*cosx + z1*sinx;
	    z = -y1*sinx + z1*cosx;

	    x1 = x*cosy - z*siny;       // вокруг Y
	    y1 = y;
	    z1 = x*siny + z*cosy;

    	newpix[i][0] = (int16_t)round(x1 * scale);    // Масштабирование
    	newpix[i][1] = (int16_t)round(y1 * scale);
    	newpix[i][2] = (int16_t)round(z1 * scale);

    	newpix[i][0] += shiftX;     // Сдвиг по осям
    	newpix[i][1] += shiftY;
    	newpix[i][2] += shiftZ;
	}
}

void cube_draw(uint16_t color)
{
	uint8_t i, j;
	int16_t x0, y0, x1, y1;

	for (j = 0; j < AMOUNT_NODE; j++)
	{
		i = j;
		calcPerspectiveProjection(newpix[i], &x0, &y0);
		if (i < AMOUNT_NODE/2)   // Рисует соединяющие ребра
		{
			calcPerspectiveProjection(newpix[i+4], &x1, &y1);
			drawLine(x0, y0, x1, y1, color);
		}
		i ++;
		i = (i == AMOUNT_NODE/2) ? 0 : i;   // Рисует переднюю грань
		i = (i == AMOUNT_NODE) ? 4 : i;	// Рисует заднюю грань

		calcPerspectiveProjection(newpix[i], &x1, &y1);
		drawLine(x0, y0, x1, y1, color);
	}
}
