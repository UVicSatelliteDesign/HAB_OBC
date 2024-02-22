/*
 * HAWB_APRS_lib.h
 *
 *  Created on: Feb 13, 2024
 *      Author: aabuelazm
 */

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_i2c.h"

#ifndef HAWB_APRS_LIB_H_
#define HAWB_APRS_LIB_H_

#define APRS_ID 0x69

struct Position {
	float lat;
	float lng;
	float alt;
};

void getLocation(I2C_HandleTypeDef *hi2c, struct Position* location);

#endif /* HAWB_APRS_LIB_H_ */
