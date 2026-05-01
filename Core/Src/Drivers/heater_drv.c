/**
  ******************************************************************************
  * @file    heater_drv.c
  * @brief   Heater PWM control driver (TIM1 CH2).
  *
  * PWM schedule (temp thresholds, normal polarity):
  *   > 20 °C  →    0  (off)
  *   15–20 °C → 2000
  *   10–15 °C → 4000
  *    5–10 °C → 10000
  *   < 5  °C  → 17000 (max heat)
  *
  * Reversed polarity: PWM value = 19999 - normal_value.
  ******************************************************************************
  */

#include "heater_drv.h"
#include "../App/config.h"
#include "tim.h"    /* htim1 */
#include "main.h"

/* ── PWM constants ────────────────────────────────────────────────────────── */

#define HEATER_PWM_MAX      19999u
#define HEATER_PWM_OFF      0u

/* ── Private state ────────────────────────────────────────────────────────── */

static uint8_t  s_reverse   = 0;
static uint16_t s_switching = 0;   /* last set value, exposed for debug */

/* ── Helpers ──────────────────────────────────────────────────────────────── */

static void set_pwm(uint32_t value)
{
    s_switching = (uint16_t)value;
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, value);
}

static uint32_t apply_reverse(uint32_t normal_val)
{
    return s_reverse ? (HEATER_PWM_MAX - normal_val) : normal_val;
}

/* ── heater_init ──────────────────────────────────────────────────────────── */

void heater_init(void)
{
    Config cfg = config_get();
    s_reverse   = cfg.heater_reverse;
    s_switching = 0;

    TIM1_Reinit_Channel2(s_reverse);

    /* Start with heater off */
    set_pwm(apply_reverse(HEATER_PWM_OFF));
}

/* ── heater_update ────────────────────────────────────────────────────────── */

void heater_update(float temp_c)
{
    uint32_t pwm;

    if      (temp_c > 20.0f)  { pwm = 0;     }
    else if (temp_c >= 15.0f) { pwm = 2000;  }
    else if (temp_c >= 10.0f) { pwm = 4000;  }
    else if (temp_c >=  5.0f) { pwm = 10000; }
    else                      { pwm = 17000; }

    set_pwm(apply_reverse(pwm));
}

/* ── heater_set_reverse ───────────────────────────────────────────────────── */

void heater_set_reverse(uint8_t reverse)
{
    s_reverse = reverse;
    TIM1_Reinit_Channel2(s_reverse);
    /* Re-apply current switching value with new polarity */
    set_pwm(apply_reverse(HEATER_PWM_OFF));
}

/* ── heater_debug_set ─────────────────────────────────────────────────────── */

void heater_debug_set(uint8_t on)
{
    s_switching = on ? 256 : 0;
    set_pwm(apply_reverse(on ? HEATER_PWM_MAX : HEATER_PWM_OFF));
}

/* ── heater_get_switching ─────────────────────────────────────────────────── */

uint16_t heater_get_switching(void)
{
    return s_switching;
}
