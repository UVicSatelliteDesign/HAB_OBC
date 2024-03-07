/*
 * HAWB_APRS.c
 *
 *  Created on: Mar 7, 2024
 *      Author: aabuelazm
 */

#include "HAWB_APRS.h"

/**
 * @brief Copies the received data from the SPI DMA to the position data type.
 *
 * @param[in] hspi: SPI handle to grab data from.
 * @param[out] pos: The Position variable to store data in.
 *
 * @return The status of the SPI peripheral.
*/
HAL_StatusTypeDef getLocation(SPI_HandleTypeDef* hspi, Position* pos) {
    return HAL_SPI_Receive_DMA(hspi, (uint8_t*)pos, sizeof(Position));
}
