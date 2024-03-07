/*
 * HAWB_APRS.h
 *
 *  Created on: Mar 7, 2024
 *      Author: aabuelazm
 */

#ifndef INC_HAWB_APRS_H_
#define INC_HAWB_APRS_H_

#include "stm32h7xx_hal.h"

#pragma pack(1, push)
typedef struct __Position {
	float longitude;
	float latitude;
	float altitude;
} Position;
#pragma pack(pop)

HAL_StatusTypeDef getLocation(SPI_HandleTypeDef* hspi, Position* pos);

#endif /* INC_HAWB_APRS_H_ */
