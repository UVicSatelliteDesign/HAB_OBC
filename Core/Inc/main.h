/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "HAWB_APRS.h"
#include "data_storage.h"
#include "adc.h"
#include "spi.h"
#include "usart.h"
#include "retarget.h"
#include <stdio.h>
#include <stdbool.h>

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
/**
 * @brief Retrieves and formats data to be logged in flash memory.
 * 
 * @param data 
 */
void fetchData(Data* data);

/**
 * @brief Determines whether data needs to be written to flash memory, and does so if needed.
 */
void logData();

/**
 * @brief Sends current to a resistor for a specified amount of time.
 * 
 * The purpose of this action is to create enough heat to cut a string and 
 * detatch the payload from the balloon so it can land and be retrieved.
 */
void cutBalloon();

/**
 * @brief An infinite loop that performs limited functionality to reduce system
 *  load.
 */
void lowPowerMode();

/**
 * @brief Get the current voltage output from the power system.
 * 
 * The voltage returned by this function has an error of ~2%.
 * 
 * @return int The current volatge in mV
 */
int getVoltage();

/**
 * @brief Get the current current output from the power system.
 * 
 *  The voltage returned by this function has an error of ~4.5%.
 * 
 * @return int The current current in mA
 */
int getCurrent();

/**
 * @brief Determines whether entering low power mode is necessary.
 */
void checkBattery();

/**
 * @brief Check that the current altitiude is less than the max altitude.
 * 
 * @param altitude 
 * @return int 0 if the altitude is less than max altitude, else 1.
 */
int checkAltitude(float altitude);

/**
 * @brief Checks the current location is within acceptable longitude, latitude
 *  and alitude ranges and detatches the payload from the balloon if not.
 */
void checkLocation(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define STLINK_RX_Pin GPIO_PIN_8
#define STLINK_RX_GPIO_Port GPIOD
#define STLINK_TX_Pin GPIO_PIN_9
#define STLINK_TX_GPIO_Port GPIOD
#define USB_OTG_FS_PWR_EN_Pin GPIO_PIN_10
#define USB_OTG_FS_PWR_EN_GPIO_Port GPIOD
#define USB_OTG_FS_OVCR_Pin GPIO_PIN_7
#define USB_OTG_FS_OVCR_GPIO_Port GPIOG
#define LD2_Pin GPIO_PIN_1
#define LD2_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */
#define powMin 10  // Replace with the actual power threshold value
#define maxLongitude 0
#define minLongitude 0
#define maxLatitude 0
#define minLatitude 0
#define maxAltitude 0
#define voltageConversion_mV 6 // int(25000 / 4095) = mav mV / 12-bit ADC resolution
#define currentConversion_mA 7 // int(30000 / 4095) = mav mA / 12-bit ADC resolution

#define logPeriod	60000	// Period for logging data, in milliseconds.
#define maxLoggingTime 28800000	// Max total time to be logging data, in milliseconds
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
