#ifndef CONFIG_H
#define CONFIG_H

/**
  ******************************************************************************
  * @file    config.h
  * @brief   Persistent application configuration.
  *
  * Wraps flash_drv with typed accessors. All modules read config via
  * config_get() and write via config_set() + config_save().
  *
  * Config is loaded once at startup in app_init(). After that, the in-memory
  * copy is authoritative — flash is only written on explicit config_save().
  ******************************************************************************
  */

#include <stdint.h>

typedef struct {
    uint16_t min_stopper;     /* stoper OPEN compare value  */
    uint16_t max_stopper;     /* stoper CLOSE compare value */
    uint8_t  heater_reverse;  /* 1 = inverted heater PWM    */
} Config;

/**
  * @brief  Load config from flash into RAM. Uses defaults if flash is blank.
  */
void config_load(void);

/**
  * @brief  Returns the current in-memory config.
  */
Config config_get(void);

/**
  * @brief  Updates the in-memory config (does NOT write to flash).
  */
void config_set(const Config *cfg);

/**
  * @brief  Writes the current in-memory config to flash.
  */
void config_save(void);

#endif /* CONFIG_H */
