#ifndef HEATER_DRV_H
#define HEATER_DRV_H

/**
  ******************************************************************************
  * @file    heater_drv.h
  * @brief   Heater PWM control driver (TIM1 CH2).
  *
  * heater_update() implements a simple temperature-threshold PWM schedule.
  * heater_set_reverse() swaps the PWM polarity (for reversed-wired MOSFETs).
  * heater_debug_set() forces heater on/off from the debug interface.
  ******************************************************************************
  */

#include <stdint.h>

/**
  * @brief  Initialise heater PWM output. Loads reverse flag from config.
  */
void heater_init(void);

/**
  * @brief  Update heater PWM based on current temperature.
  *         Call from main loop when thermo_is_valid() is true.
  */
void heater_update(float temp_c);

/**
  * @brief  Set reverse flag (1 = inverted PWM polarity).
  *         Takes effect on next heater_update() call.
  */
void heater_set_reverse(uint8_t reverse);

/**
  * @brief  Force heater on (1) or off (0) from debug interface.
  *         Cleared on next heater_update() call.
  */
void heater_debug_set(uint8_t on);

/**
  * @brief  Returns the current heater switching value (for debug stream).
  */
uint16_t heater_get_switching(void);

#endif /* HEATER_DRV_H */
