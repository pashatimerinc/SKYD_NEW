#ifndef SWITCHES_DRV_H
#define SWITCHES_DRV_H

/**
  ******************************************************************************
  * @file    switches_drv.h
  * @brief   Debounced mechanical switch driver.
  *
  * Reads 4 GPIO inputs and applies a 4-sample shift-register debounce.
  * switches_tick() must be called every 1 ms from SysTick.
  * switches_get() returns the last stable reading — safe to call anywhere.
  ******************************************************************************
  */

#include <stdint.h>

typedef struct {
    uint8_t fixator;  /* clip/fixator switch  */
    uint8_t counter;  /* payload counter      */
    uint8_t start;    /* pusher start/home    */
    uint8_t finish;   /* pusher end/finish    */
} Switches;

/**
  * @brief  1 ms debounce tick. Call from HAL_SYSTICK_Callback.
  */
void switches_tick(void);

/**
  * @brief  Returns the last debounced switch state.
  */
Switches switches_get(void);

#endif /* SWITCHES_DRV_H */
