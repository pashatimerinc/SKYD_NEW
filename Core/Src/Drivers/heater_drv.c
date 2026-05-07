/**
  ******************************************************************************
  * @file    heater_drv.c
  * @brief   Heater PWM control driver (TIM1 CH2).
  *
  * Polarity is handled in hardware via TIM_OCPOLARITY — no software value
  * inversion. heater_tim_reinit() stops the channel, flips polarity, restarts.
  *
  * PWM schedule (raw compare values, polarity-independent):
  *   > 20 °C  →     0  (off)
  *   15–20 °C →  2000
  *   10–15 °C →  4000
  *    5–10 °C → 10000
  *   < 5  °C  → 17000  (max heat)
  ******************************************************************************
  */

#include "heater_drv.h"
#include "../App/config.h"
#include "../App/state_machine.h"
#include "tim.h"
#include "main.h"

/* ── PWM constants ────────────────────────────────────────────────────────── */

#define HEATER_PWM_OFF  0u

/* ── Forward declarations ─────────────────────────────────────────────────── */

static void heater_tim_reinit(uint8_t reverse);
static void set_pwm(uint32_t value);

/* ── Private state ────────────────────────────────────────────────────────── */

static uint8_t  s_reverse   = 0;
static uint16_t s_switching = 0;

/* ── heater_init ──────────────────────────────────────────────────────────── */

void heater_init(void)
{
    Config cfg = config_get();
    s_reverse   = cfg.heater_reverse;
    s_switching = 0;

    heater_tim_reinit(s_reverse);
    set_pwm(HEATER_PWM_OFF);
}

/* ── heater_update ────────────────────────────────────────────────────────── */

void heater_update(float temp_c, StateMachineCtx *ctx)
{
	if (!get_isTicked()) return;
	SystemState state = sm_get_state(ctx);

    uint32_t pwm;

    if      (temp_c > 20.0f || state != LOADED)  { pwm = 0;     }
    else if (temp_c >= 15.0f) 					 { pwm = 2000;  }
    else if (temp_c >= 10.0f) 					 { pwm = 4000;  }
    else if (temp_c >=  5.0f) 					 { pwm = 10000; }
    else                      					 { pwm = 17000; }

    set_pwm(pwm);
    clear_isTicked();
}

/* ── heater_set_reverse ───────────────────────────────────────────────────── */

void heater_set_reverse(uint8_t reverse)
{
    s_reverse = reverse;
    heater_tim_reinit(s_reverse);
    set_pwm(s_switching ? 19999u : HEATER_PWM_OFF);
}

/* ── heater_debug_set ─────────────────────────────────────────────────────── */

void heater_debug_set(uint8_t on)
{
    s_switching = on ? 256 : 0;
    set_pwm(on ? 19999u : HEATER_PWM_OFF);
}

/* ── heater_get_switching ─────────────────────────────────────────────────── */

uint16_t heater_get_switching(void)
{
    return s_switching;
}

/* ── Private helpers ──────────────────────────────────────────────────────── */

static void set_pwm(uint32_t value)
{
//    s_switching = (uint16_t)value;
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, value);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
}

static void heater_tim_reinit(uint8_t reverse)
{
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);

    TIM_OC_InitTypeDef oc = {0};
    oc.OCMode     = TIM_OCMODE_PWM1;
    oc.Pulse      = 0;
    oc.OCPolarity = reverse ? TIM_OCPOLARITY_LOW : TIM_OCPOLARITY_HIGH;
    oc.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim1, &oc, TIM_CHANNEL_2);

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
}
