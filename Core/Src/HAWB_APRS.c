/**
 * @file HAWB_APRS.c
 * Created on: Mar 7, 2024
 * @author aabuelazm
 */

#include "HAWB_APRS.h"
#include <string.h>

static void readBE4SPI(uint8_t* data);

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

/**
 * @brief A callback function that reads the big endian data from the APRS
 * 
 * @param[in,out] hspi: SPI handle to grab data from.
 */
void getLocationClbk(SPI_HandleTypeDef* hspi) {
    for (int i = 0; i < 3; ++i) {
        readBE4SPI(&hspi->pRxBuffPtr[i*4]);
    }
}

/**
 * @brief Helper function to read big endian 4 byte data types.
 * 
 * @param[in,out] data: Pointer to 4 byte buffer.
 */
static void readBE4SPI(uint8_t* data) {
    uint32_t temp;

    temp = (((uint32_t)((uint32_t)data[0]) << 24) | ((uint32_t)((uint32_t)data[1]) << 16) | ((uint32_t)((uint32_t)data[2]) << 8) | ((uint32_t)((uint32_t)data[3]) << 0));

    memcpy(data, &temp, 4);
}
