/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "stm32f4xx_hal.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

// Global variables

int powMinThreshold = 10;  // Replace with the actual power threshold value
int alttitudeFlag = 0; // if 0 lower than max alt if 1 above max alt




TIM_HandleTypeDef htim2;


void SystemClock_Config(void);
static void MX_TIM2_Init(void);


// Function prototypes
void checkBatteryEveryMinute(void *pvParameters);
void lowPowerMode(void);
void checkBattery(void);
void checkLocation(void);
void getCameraData(void);
void descension(void);


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
		checkBattery();
    }



/**
  * @brief  The application entry point.
  * @retval int
  */
void main(void){

	  //config system clock
	  SystemClock_Config();

	  //initialization of TIM2
	  MX_TIM2_Init();


	  // Initialize FreeRTOS and create tasks
	  xTaskCreate(pollingLoop, "PollingTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);


	  // Start the FreeRTOS scheduler
	  vTaskStartScheduler();



	  return 0;
}

void pollingLoop(void){
	while(1){


		checkLocation();
		gatherCameraData();
		tansmitData();
		controlDescension();

		// Delay is a maybe
		vTaskDelay(pdMS_TO_TICKS(5000));

	}


}


void lowPowerMode(void){

    deactiveCam();

    if (checkTransmission() == TRUE)
    {
        char lowPowerPacket[] = "Low Power Mode";
        addPacketToQueue(lowPowerPacket);
        isPacketSent();
    }
    else
    {
        logError();
        logTime();
        stopTransmitter();

    }
    While(1){

    }
    	}

}

void checkBattery(void){

    // Assuming measureCurrent(), measureBatteryVoltage(), measureTemp(), and getTime() are placeholders
    float current = measureCurrent();
    float batteryVoltage = measureBatteryVoltage();
    float temp = measureTemp();

    storeValues(current, batteryVoltage, temp, getTime());

    if (power <= powMinThreshold)
    {
        lowPowerMode();
    }
    else
    {
        return;
    }
}

void checkLocation(void){
    int maxLongitude = 0;
    int minLongitude = 0;
    int maxLatitude = 0;
    int minLatitude = 0;
    int maxAltitude = 0;


    // getLongitude(), getLatitude(), getAltitude(), and locationDisplay() are placeholders
    int longitude = getLongitude();
    int latitude = getLatitude();
    int altitude = getAltitude();


    if (longitude < maxLongitude && longitude > minLongitude){
    	if(latitude < maxLatitude && latitude > minLatitude){
    		if(checkAltitude() == 1){
    			descend();
    		}
    		return;
    	}
    }

    descend();

}

void getCameraData(void){

    // pollCamera() is a placeholder
    storeValues(pollCamera());
}



void descend(){

	cutBalloon();

}

void descension(void){

	static int lastAltitude[4] = {-1, -1, -1, -1}; // Initialize to invalid values

    int altitudeData = 0;

    if (altitudeData > altitudeThreshold)
    {
        cutBalloon();
    }
    else if (/* Add condition to check if balloon is falling before reaching threshold */)
    {
        storeError(altitudeData, someTime);
    }
    else
    {
        return;
    }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void){
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 24;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_TIM2_Init(void){


  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};


  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 60000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 59999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

