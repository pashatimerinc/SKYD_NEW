#ifndef THERMO_DRV_H
#define THERMO_DRV_H

/**
  ******************************************************************************
  * @file    thermo_drv.h
  * @brief   Non-blocking DS18B20 temperature driver.
  *
  * Two entry points for two different contexts:
  *
  *   thermo_systick()  — called every 1 ms from SysTick IRQ.
  *                       Runs the one-wire timing state machine.
  *
  *   thermo_tick()     — called every main-loop iteration.
  *                       Drives the conversion/read sequence and decides
  *                       when to start the next measurement (every 5 s when
  *                       the system is active).
  *
  *   thermo_blocking_measure() — startup only. Blocks until first reading.
  ******************************************************************************
  */

#include <stdint.h>
#include <main.h>

/**
  * @brief  Initialise DS18B20 one-wire bus and configure sensor.
  */
void thermo_init(void);

/**
  * @brief  Blocking first measurement. Call once from app_init().
  */
void thermo_blocking_measure(void);

/**
  * @brief  Main-loop tick: advance the non-blocking conversion state machine.
  */
void thermo_tick(void);

/**
  * @brief  SysTick tick (1 ms): advance one-wire timing.
  *         Call from HAL_SYSTICK_Callback.
  */
void thermo_systick(void);

/**
  * @brief  TIM2 period-elapsed callback for one-wire timing.
  *         Call from HAL_TIM_PeriodElapsedCallback.
  */
void thermo_tim_callback(TIM_HandleTypeDef *htim);

/**
  * @brief  Returns 1 if the last reading is valid (sensor present, in range).
  */
uint8_t thermo_is_valid(void);

/**
  * @brief  Returns the last valid temperature in degrees Celsius.
  *         Returns 0.0f if no valid reading is available.
  */
float thermo_get_celsius(void);

#endif /* THERMO_DRV_H */
