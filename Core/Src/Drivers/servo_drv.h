#ifndef SERVO_DRV_H
#define SERVO_DRV_H

/**
  ******************************************************************************
  * @file    servo_drv.h
  * @brief   Servo output driver (TIM1 CH1 = stoper, TIM1 CH3 = pusher).
  *
  * Positions are named, not raw compare values.
  * Stoper trim (OPEN/CLOSE compare values) is loaded from config at init.
  ******************************************************************************
  */

#include <stdint.h>

/* ── Servo command values ─────────────────────────────────────────────────── */

typedef enum {
    SERVO_STOP  = 1500,    /* pusher: hold in place (no PWM pulse change)  */
    SERVO_HOME  = 1400,    /* pusher: retract to home / start position      */
    SERVO_DROP  = 1600,    /* pusher: extend to drop position               */
    SERVO_OPEN  = 1600,    /* stoper: open (release payload path)           */
    SERVO_CLOSE = 1300,    /* stoper: close (block payload path)            */
} ServoCommand;

/**
  * @brief  Initialise TIM1 PWM channels and load trim from config.
  */
void servo_init(void);

/**
  * @brief  Set pusher servo position. No-op if already in that position.
  */
void servo_pusher_set(ServoCommand cmd);

/**
  * @brief  Set stoper servo position. No-op if already in that position.
  */
void servo_stoper_set(ServoCommand cmd);

/**
  * @brief  Get current pusher command.
  */
ServoCommand servo_pusher_get(void);

/**
  * @brief  Get current stoper command.
  */
ServoCommand servo_stoper_get(void);

/**
  * @brief  Update stoper trim values (called after config change).
  */
void servo_stoper_update_trim(uint16_t min_val, uint16_t max_val);

#endif /* SERVO_DRV_H */
