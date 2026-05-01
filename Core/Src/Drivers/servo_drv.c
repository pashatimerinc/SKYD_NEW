/**
  ******************************************************************************
  * @file    servo_drv.c
  * @brief   Servo output driver.
  *
  * TIM1 CH1 → stoper servo  (compare value = position)
  * TIM1 CH3 → pusher servo  (compare value = position, named HOME/DROP/STOP)
  *
  * Stoper OPEN/CLOSE compare values come from config (saved trim).
  * Pusher uses fixed raw compare values defined in main.h (HOME, DROP, STOP).
  ******************************************************************************
  */

#include "servo_drv.h"
#include "../App/config.h"
#include "../App/comm.h"
#include "tim.h"    /* htim1 */
#include "main.h"   /* HOME, DROP, STOP raw compare values */

/* ── Private state ────────────────────────────────────────────────────────── */

static ServoCommand s_pusher_cmd = SERVO_STOP;
static ServoCommand s_stoper_cmd = SERVO_STOP;

static uint16_t s_stoper_open  = 0;   /* loaded from config in servo_init */
static uint16_t s_stoper_close = 0;

/* ── servo_init ───────────────────────────────────────────────────────────── */

void servo_init(void)
{
    Config cfg = config_get();
    s_stoper_open  = cfg.min_stopper;
    s_stoper_close = cfg.max_stopper;

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
}

/* ── servo_pusher_set ─────────────────────────────────────────────────────── */

void servo_pusher_set(ServoCommand cmd)
{
    if (s_pusher_cmd == cmd) { return; }

    uint32_t compare = cmd;

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, compare);
    s_pusher_cmd = cmd;
}

/* ── servo_stoper_set ─────────────────────────────────────────────────────── */

void servo_stoper_set(ServoCommand cmd)
{
    if (s_stoper_cmd == cmd) { return; }

    uint32_t compare = cmd;


    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, compare);
    s_stoper_cmd = cmd;
}

/* ── Getters ──────────────────────────────────────────────────────────────── */

ServoCommand servo_pusher_get(void) { return s_pusher_cmd; }
ServoCommand servo_stoper_get(void) { return s_stoper_cmd; }

/* ── servo_stoper_update_trim ─────────────────────────────────────────────── */

void servo_stoper_update_trim(uint16_t min_val, uint16_t max_val)
{
    s_stoper_open  = min_val;
    s_stoper_close = max_val;

    /* Re-apply immediately if stoper is already in one of these positions */
    if (s_stoper_cmd == SERVO_OPEN)
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, s_stoper_open);
    else if (s_stoper_cmd == SERVO_CLOSE)
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, s_stoper_close);
}
