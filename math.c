/*
* math.c
*
* Created: 23.02.2013 16:18:55
*  Author: OliverS
*
* $Id$
*/

#include "global.h"
#include <stdlib.h>
#include <avr/pgmspace.h>

static const uint8_t sintab[] PROGMEM = 
{
	  0, 2, 4, 7, 9, 11, 13, 15, 18, 20, 22, 24, 26, 29, 31, 33, 35, 37, 39, 
	  41, 43, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 65, 67, 69, 71, 73, 75, 
	  76, 78, 80, 82, 83, 85, 87, 88, 90, 91, 93, 94, 96, 97, 99, 100, 101, 
	  103, 104, 105, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 
	  118, 119, 119, 120, 121, 121, 122, 123, 123, 124, 124, 125, 125, 125, 
	  126, 126, 126, 127, 127, 127, 127, 127, 
};


/*
int8_t sin(int8_t angle)
{
	uint8_t index;
	if (angle >= 0)	
		index = angle >> 1;
	else
		index = -angle >> 1;
	
	if (index > length(sintab))
		index = length(sintab) - index;
	
	if (angle >= 0)
		return pgm_read_byte(&sintab[index]);
	else
		return -pgm_read_byte(&sintab[index]);
}
*/

/*
int16_t arctan2(int16_t y, int16_t x)
{
	int16_t coeff_1 = 128;
	int16_t coeff_2 = 3 * coeff_1;
	int32_t abs_y = abs(y);
	int32_t r, angle;

	if (x >= 0)
	{
		r = (x - abs_y) / (x + abs_y);
		angle = coeff_1 - coeff_1 * r;
	}
	else
	{
		r = (x + abs_y) / (abs_y - x);
		angle = coeff_2 - coeff_1 * r;
	}
	
	if (y < 0)
	return (int16_t)(-angle);
	else
	return (int16_t)(angle);
}
*/

int16_t arctan2 (int16_t y, int16_t x)
{
	if (y == 0)
	{
		if (x >= 0)
			return 0;
		else
			return 180;
	}
	
	if (x == 0)
	{
		if (y > 0)
			return 90;
		else
			return -90;
	}
	
	int32_t angle;
	int32_t x2 = (int32_t)x * x;
	int32_t y2 = (int32_t)y * y;
	int32_t z = labs(3667L * (int32_t)x * (int32_t)y);
	int32_t n;
	
	if (abs(y) <= abs(x))
	{
		n = 64L * (int32_t)x * (int32_t)x + 17L * (int32_t)y * (int32_t)y;
		angle = z / n;
	}		
	else
	{
		n = 64L * (int32_t)y * (int32_t)y + 17L * (int32_t)x * (int32_t)x;
		angle = 90 - z / n;
	}		
	
	
	if (y < 0)
		angle = -angle;
	if (x < 0)
		angle = 180 - angle;
	/*
	else
	{
		if (y < 0)
		angle = 360 - angle;
	}
	*/
	
	
	return (int16_t)(angle);
}

/*
double aTan2(double y, double x) 
{	
	double coeff_1 = Math.PI / 4d;	
	double coeff_2 = 3d * coeff_1;	
	double abs_y = Math.abs(y);	
	double angle;	
	if (x >= 0d) 
	{		
		double r = (x - abs_y) / (x + abs_y);		
		angle = coeff_1 - coeff_1 * r;	
	} 
	else 
	{		
		double r = (x + abs_y) / (abs_y - x);		
		angle = coeff_2 - coeff_1 * r;	
	}	
	return y < 0d ? -angle : angle;
}
*/
