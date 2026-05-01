#ifndef VOLTAGE_DRV_H
#define VOLTAGE_DRV_H

/**
  ******************************************************************************
  * @file    voltage_drv.h
  * @brief   Input voltage measurement (ADC1) and power rail selection.
  *
  * voltage_configure_power_rail() is called once at startup to select between
  * the DC converter path (<7.6 V) and the direct servo power path (>7.6 V).
  ******************************************************************************
  */

#include <stdint.h>

/**
  * @brief  Initialise ADC. Call before voltage_configure_power_rail().
  */
void voltage_init(void);

/**
  * @brief  Measure input voltage and configure DC_SWITCH / SERVO_POWER pins.
  *         Blocks for ~500 ms to average the reading. Call once at startup.
  */
void voltage_configure_power_rail(void);

/**
  * @brief  Returns the last measured input voltage in volts.
  */
float voltage_drv_get(void);

#endif /* VOLTAGE_DRV_H */
