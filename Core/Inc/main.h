/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

#define	MAJOR	3
#define	MINOR	0
#define	PATCH	2

typedef enum {
	STOPER_OPEN,
	STOPER_IDLE,
	STOPER_CLOSE,
	PUSHER_HOME,
	PUSHER_STOP,
	PUSHER_TO_FINISH,
	PUSHER_DROP,
	EMERGENCY_STOP,
	RESTART_SERVOS,
	START_HEAT,
	END_HEAT
} DebugCommands;

typedef enum {
	SET_TRIM_MIN,
	SET_TRIM_MAX,
	SET_HEATER_REVERS,
	SAVE_DATA
} DebugSet;

//typedef enum {
//	BOOT,
//	CLIP_LOADING,
//	CHECK_LOAD,
//	LOADED,
//	DROP_ONE,
//	DROP_ALL,
//	UNLOADED,
//	DEBUG_STATE,
//	ERROR_STATE
//} SystemState;

typedef enum {
	PUSHER_ROLLBACK,
	STOP_PUSHER,
	OPEN_STOPER,
	PUSHER_THROWS,
	WAITING_ACTION_TIMEOUT,
	RETRY
} Drop_one_state;

typedef enum {
	START_CHECK,
	PUSHER_GO,
	WAITING_TIMEOUT_END,
	RETRY_CHECK
} Check_loaded_state;

//typedef enum {
//	NO_INPUT_MODE,
//	PWM_MODE,
//	MAVLINK_MODE,
//	SKYNET_MODE
//} InputCommandMode;


//typedef struct {
//	ServoCommand currentCommand;
//} StoperServo;
//
//typedef struct {
//	ServoCommand currentCommand;
//} PusherServo;


//typedef enum {
//    DS_IDLE = 0,
//    DS_START_CONV,
//    DS_WAIT_CONV,
//    DS_READ,
//    DS_READY,
//    DS_ERROR
//} ds_state_t;

typedef struct
{
    uint32_t magic;
    uint32_t min_stopper;
    uint32_t max_stopper;
    uint32_t reverse_heat;
} savedData_t;

//typedef enum {
//	ERROR_OK = 0,
//	ERROR_STUCK
//} error_device_t;

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define START_SWITCH_Pin GPIO_PIN_13
#define START_SWITCH_GPIO_Port GPIOC
#define FIXATOR_SWITCH_Pin GPIO_PIN_14
#define FIXATOR_SWITCH_GPIO_Port GPIOC
#define COUNTER_SWITCH_Pin GPIO_PIN_15
#define COUNTER_SWITCH_GPIO_Port GPIOC
#define FINISH_SWITCH_Pin GPIO_PIN_0
#define FINISH_SWITCH_GPIO_Port GPIOD
#define DC_SWITCH_Pin GPIO_PIN_4
#define DC_SWITCH_GPIO_Port GPIOA
#define LED_GREEN_Pin GPIO_PIN_12
#define LED_GREEN_GPIO_Port GPIOB
#define LED_RED_Pin GPIO_PIN_13
#define LED_RED_GPIO_Port GPIOB
#define LED_BLUE_Pin GPIO_PIN_14
#define LED_BLUE_GPIO_Port GPIOB
#define SERVO_POWER_Pin GPIO_PIN_15
#define SERVO_POWER_GPIO_Port GPIOB
#define SERVO1_Pin GPIO_PIN_8
#define SERVO1_GPIO_Port GPIOA
#define PWM_HEATER_Pin GPIO_PIN_9
#define PWM_HEATER_GPIO_Port GPIOA
#define SERVO3_Pin GPIO_PIN_10
#define SERVO3_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
#define SYS_ID 1
#define COMP_ID 169
#define BIT_MASK_REQUEST 69
#define FLASH_USER_START_ADDR  ((uint32_t)0x0801FC00)  // Last 1KB page
#define FLASH_MAGIC 0xA5A5A5A5
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
