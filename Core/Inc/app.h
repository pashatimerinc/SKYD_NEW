#ifndef APP_H
#define APP_H

/**
  ******************************************************************************
  * @file    app.h
  * @brief   Top-level application interface.
  *
  * main.c calls exactly two functions:
  *   app_init()   — run once after peripherals are ready
  *   app_update() — run every iteration of the main loop
  *
  * Everything else is internal to the App/ and Drivers/ modules.
  ******************************************************************************
  */

/**
  * @brief  One-time startup: load config, init drivers, resolve power rail,
  *         seed the state machine.
  */
void app_init(void);

/**
  * @brief  Main-loop tick: process inputs, update state, drive outputs.
  *         Must return quickly — no blocking calls.
  */
void app_update(void);

#endif /* APP_H */
