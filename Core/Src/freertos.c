/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
Position position;
int descendFlag = 0;
float prevAltitude = 0;
bool isLoggingTimeout = false;

/* USER CODE END Variables */
/* Definitions for PollingLoop */
osThreadId_t PollingLoopHandle;
const osThreadAttr_t PollingLoop_attributes = {
  .name = "PollingLoop",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for batteryTimer */
osTimerId_t batteryTimerHandle;
const osTimerAttr_t batteryTimer_attributes = {
  .name = "batteryTimer"
};
/* Definitions for descensionTimer */
osTimerId_t descensionTimerHandle;
const osTimerAttr_t descensionTimer_attributes = {
  .name = "descensionTimer"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartPollingLoop(void *argument);
void BatteryCallback(void *argument);
void DescensionCallback(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of batteryTimer */
  batteryTimerHandle = osTimerNew(BatteryCallback, osTimerPeriodic, NULL, &batteryTimer_attributes);

  /* creation of descensionTimer */
  descensionTimerHandle = osTimerNew(DescensionCallback, osTimerPeriodic, NULL, &descensionTimer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of PollingLoop */
  PollingLoopHandle = osThreadNew(StartPollingLoop, NULL, &PollingLoop_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartPollingLoop */
/**
  * @brief  Function implementing the PollingLoop thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartPollingLoop */
void StartPollingLoop(void *argument)
{
  /* USER CODE BEGIN StartPollingLoop */
  /* Infinite loop */
  RetargetInit(&huart3);
  getLocation(&hspi1, &position);
	uint32_t lastTime_ms = 0;
	HAL_GPIO_WritePin(GPIOB, LD1_Pin, GPIO_PIN_SET);

  for(;;)
  {
		if((uint32_t)(HAL_GetTick() - lastTime_ms) >= logPeriod){
			lastTime_ms = HAL_GetTick();

			checkBattery();
			checkLocation();

			if(descendFlag >= 10){
				lowPowerMode();
			}

			logData();
		}
  }
  /* USER CODE END StartPollingLoop */
}

/* BatteryCallback function */
void BatteryCallback(void *argument)
{
  /* USER CODE BEGIN BatteryCallback */

  /* USER CODE END BatteryCallback */
}

/* DescensionCallback function */
void DescensionCallback(void *argument)
{
  /* USER CODE BEGIN DescensionCallback */

  /* USER CODE END DescensionCallback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void fetchData(Data* data){
	data->time = HAL_GetTick();
	data->volts = getVoltage();
	data->longitude = position.longitude;
	data->latitude = position.latitude;
	data->altitude = position.altitude;
	//data.temperature = getTemperature();
	data->temperature = 0; //replace with above line when temperature function is implemented
	data->current = getCurrent();
}

void logData(){
		if(!isLoggingTimeout && HAL_GetTick() < maxLoggingTime){
			Data data;
			fetchData(&data);
			write_data((uint32_t*)&data);
		} else {
			isLoggingTimeout = true;
		}
}

void cutBalloon(){
	//set to pin to high HAL FUNCTION WITH PIN

	__disable_irq();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	for(int i=0;i<64000000*15;i++){
		__NOP();
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	__enable_irq();
}

void lowPowerMode(){
	uint32_t lastTickCount_ms = 0;
	HAL_GPIO_WritePin(GPIOB, LD1_Pin, GPIO_PIN_RESET);
    //stopTransmitter();
    while(1){
    	if((uint32_t)(HAL_GetTick() - lastTickCount_ms) >= logPeriod){
    		lastTickCount_ms = HAL_GetTick();

    		logData();
    	}
	}
}

int getVoltage(){
	int adc_val;
	HAL_ADC_Start(&hadc1);

	if(HAL_ADC_PollForConversion(&hadc1, 10)==HAL_OK){

		adc_val=HAL_ADC_GetValue(&hadc1);

	} else {
		return 0;
	}

	HAL_ADC_Stop(&hadc1);
	adc_val *= voltageConversion_mV;

	return adc_val;
}

int getCurrent(){
	int adc_val;
	HAL_ADC_Start(&hadc2);

	if(HAL_ADC_PollForConversion(&hadc2, 10)==HAL_OK){

		adc_val=HAL_ADC_GetValue(&hadc2);

	} else {
		return 0;
	}

	HAL_ADC_Stop(&hadc2);
	adc_val *= currentConversion_mA;

	return adc_val;
}

void checkBattery(){
	float batteryVoltage = getVoltage();

		if (batteryVoltage <= powMin){
			lowPowerMode();
		}
}

int checkAltitude(float altitude){
	if (altitude < prevAltitude){
		descendFlag+= 1;
	}

  if (altitude >= maxAltitude){
    return 1;
  }

  prevAltitude = altitude;
  return 0;
}

void checkLocation(void){
	getLocation(&hspi1, &position);

    if (position.longitude < maxLongitude && position.longitude > minLongitude){
        if(position.latitude < maxLatitude && position.latitude > minLatitude){
            if(checkAltitude(position.altitude) == 1){
                cutBalloon();
            }
            return;
        }
    }

    cutBalloon();
}
/* USER CODE END Application */

