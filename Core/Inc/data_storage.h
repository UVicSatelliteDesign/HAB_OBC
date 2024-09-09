/**
 * @file data_storage.h
 * @author Kali Erickson, kali.erickson@outlook.com
 * @brief Library for accessing flash memory.
 */

#ifndef DATA_STORAGE_H_
#define DATA_STORAGE_H_

#include "stm32h7xx.h"
#include <stdint.h>
#include <stdio.h>

#define FLASH_USER_START_ADDR FLASH_BANK2_BASE;
static uint32_t cur_flash_addr = FLASH_USER_START_ADDR;

/**
 * @brief Structure for passing data to be stored in flash memory.
 *
 * The size of this structure is 8*32 bits = 256 (the size of a flash word).
 * The last 32 bits are zeros inserted to pad to the size of a flash word.
 */
typedef struct {
  float time;
  float volts;
  int longitude;
  int latitude;
  int altitude;
  float temperature;
  float current;
  uint32_t pad;
} Data;

/**
 * @brief Initialize flash memory user data section by setting all bits to 0.
 *
 * This function should only be called when initializing the device as it is not
 * efficient and erases all previously stored data
 */
void erase_bank();

/**
 * @brief Write 8 32-bit words to the flash memory.
 *
 * Assumes memory has been initialized by calling erase_bank.
 *
 * @param data The address of the struct that holds the data to write. Must
 * point to 32 bytes of contiguous memory.
 */
void write_data(uint32_t *data);

/**
 * @brief Read all data from user flash memory and send over UART.
 *
 * Note: This is a blocking function intended to be called upon some external.
 * trigger
 */
void read_data(UART_HandleTypeDef huart3);

/**
 * @brief Verify that data is correctly written to user flash memory.
 */
void test_log_data();

/**
 * @brief Log 2 flash words of data, then read and send over UART.
 *
 * This function does not actually perform any verification. Manual verification
 * of data transmission over UART is required.
 *
 * @param huart3 UART3 handle
 */
void test_read_data(UART_HandleTypeDef huart3);

#endif /*DATA_STORAGE_H_*/
