/**
  ******************************************************************************
  * @file    config.c
  * @brief   Persistent application configuration.
  ******************************************************************************
  */

#include "config.h"
#include "../Drivers/flash_drv.h"
#include "main.h"   /* savedData_t, STOP */

/* ── Defaults (used when flash is blank or corrupt) ──────────────────────── */

#define DEFAULT_MIN_STOPPER     1350
#define DEFAULT_MAX_STOPPER     1600
#define DEFAULT_HEATER_REVERSE  0u

/* ── Private state ────────────────────────────────────────────────────────── */

static Config s_config = {
    .min_stopper    = DEFAULT_MIN_STOPPER,
    .max_stopper    = DEFAULT_MAX_STOPPER,
    .heater_reverse = DEFAULT_HEATER_REVERSE,
};

/* ── config_load ──────────────────────────────────────────────────────────── */

void config_load(void)
{
    savedData_t raw;

    if (flash_drv_load(&raw))
    {
        s_config.min_stopper    = (uint16_t)raw.min_stopper;
        s_config.max_stopper    = (uint16_t)raw.max_stopper;
        s_config.heater_reverse = (uint8_t)raw.reverse_heat;
    }
    /* else: keep defaults */
}

/* ── config_get / config_set ──────────────────────────────────────────────── */

Config config_get(void)          { return s_config;  }
void   config_set(const Config *cfg) { s_config = *cfg; }

/* ── config_save ──────────────────────────────────────────────────────────── */

void config_save(void)
{
    savedData_t raw = {
        .magic        = FLASH_MAGIC,
        .min_stopper  = s_config.min_stopper,
        .max_stopper  = s_config.max_stopper,
        .reverse_heat = s_config.heater_reverse,
    };

    flash_drv_save(&raw);
}
