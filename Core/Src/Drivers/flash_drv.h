#ifndef FLASH_DRV_H
#define FLASH_DRV_H

/**
  ******************************************************************************
  * @file    flash_drv.h
  * @brief   Flash storage driver (one page at FLASH_USER_START_ADDR).
  *
  * Stores one savedData_t struct. All higher-level config logic lives in
  * config.c — this driver only handles the raw erase/program/read operations.
  ******************************************************************************
  */

#include <stdint.h>
#include "main.h"   /* savedData_t, FLASH_USER_START_ADDR, FLASH_MAGIC */

/**
  * @brief  Read saved data from flash.
  * @param  out   Pointer to struct to fill.
  * @return 1 if magic matches (valid data), 0 if flash is blank/corrupt.
  */
uint8_t flash_drv_load(savedData_t *out);

/**
  * @brief  Erase the user page and write the full struct.
  * @param  data  Pointer to struct to save.
  */
void flash_drv_save(const savedData_t *data);

#endif /* FLASH_DRV_H */
