/**
  ******************************************************************************
  * @file    switches_drv.c
  * @brief   Debounced mechanical switch driver.
  *
  * Each switch uses a 4-bit shift register. On every tick the current GPIO
  * reading is shifted in. The output only changes when all 4 samples agree.
  * This gives ~4 ms of contact-bounce rejection.
  ******************************************************************************
  */

#include "switches_drv.h"
#include "gpio.h"
#include "main.h"

/* ── Config ───────────────────────────────────────────────────────────────── */

#define DEBOUNCE_SAMPLES  4u
#define DEBOUNCE_MASK     ((uint8_t)((1u << DEBOUNCE_SAMPLES) - 1u))  /* 0x0F */

/* ── Private state ────────────────────────────────────────────────────────── */

static uint8_t  s_fixator_hist = 0;
static uint8_t  s_counter_hist = 0;
static uint8_t  s_finish_hist  = 0;
static uint8_t  s_start_hist   = 0;

static Switches s_state = {0};

/* ── Debounce helper macro ────────────────────────────────────────────────── */
/*
 * Shift new sample into history register.
 * Update output only when all samples agree (all 1s or all 0s).
 */
#define DEBOUNCE(hist, output, gpio_port, gpio_pin)                      \
    do {                                                                   \
        (hist) = ((hist) << 1) |                                           \
                 (!HAL_GPIO_ReadPin((gpio_port), (gpio_pin)));             \
        if (((hist) & DEBOUNCE_MASK) == DEBOUNCE_MASK) { (output) = 1; } \
        else if (((hist) & DEBOUNCE_MASK) == 0)        { (output) = 0; } \
    } while (0)

/* ── switches_tick ────────────────────────────────────────────────────────── */

void switches_tick(void)
{
    DEBOUNCE(s_fixator_hist, s_state.fixator,
             FIXATOR_SWITCH_GPIO_Port, FIXATOR_SWITCH_Pin);

    DEBOUNCE(s_counter_hist, s_state.counter,
             COUNTER_SWITCH_GPIO_Port, COUNTER_SWITCH_Pin);

    DEBOUNCE(s_finish_hist,  s_state.finish,
             FINISH_SWITCH_GPIO_Port,  FINISH_SWITCH_Pin);

    DEBOUNCE(s_start_hist,   s_state.start,
             START_SWITCH_GPIO_Port,   START_SWITCH_Pin);
}

/* ── switches_get ─────────────────────────────────────────────────────────── */

Switches switches_get(void)
{
    return s_state;
}
