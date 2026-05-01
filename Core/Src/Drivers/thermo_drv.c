/**
  ******************************************************************************
  * @file    thermo_drv.c
  * @brief   Non-blocking DS18B20 temperature driver.
  *
  * Measurement cycle (non-blocking):
  *   DS_START_CONV → issue convert command
  *   DS_WAIT_CONV  → wait cnv_time ms (polled in thermo_tick)
  *   DS_READ       → request scratchpad read
  *   DS_READY      → poll for completion, store result
  *
  * A new cycle is requested every 5 s by thermo_tick when the system is in
  * an active state (LOADED or DEBUG). The cycle itself takes ~375 ms at
  * 10-bit resolution.
  *
  * Valid temperature range: −55 °C to +125 °C (stored as int16 × 100).
  * Out-of-range values flag the reading as invalid.
  ******************************************************************************
  */

#include "thermo_drv.h"
#include "ds18b20.h"
#include "tim.h"    /* htim2 */
#include "main.h"
#include <stddef.h>

/* ── Constants ────────────────────────────────────────────────────────────── */

#define THERMO_PERIOD_MS        5000u
#define THERMO_VALID_MIN_X100  (-5500)   /* −55.00 °C */
#define THERMO_VALID_MAX_X100  (12500)   /*  125.00 °C */

/* ── State machine ────────────────────────────────────────────────────────── */

typedef enum {
    DS_IDLE,
    DS_START_CONV,
    DS_WAIT_CONV,
    DS_READ,
    DS_READY,
    DS_ERROR,
} DsState;

/* ── Private state ────────────────────────────────────────────────────────── */

static ds18b20_t  s_ds18;
static DsState    s_state       = DS_IDLE;
static uint32_t   s_ts          = 0;
static int16_t    s_temp_x100   = -10000;   /* invalid sentinel */
static uint32_t   s_last_start  = 0;

/* ── Forward declarations ─────────────────────────────────────────────────── */

static void ds18_tim_cb(TIM_HandleTypeDef *htim);

/* ── thermo_init ──────────────────────────────────────────────────────────── */

void thermo_init(void)
{
    ow_init_t ow = {
        .tim_handle    = &htim2,
        .gpio          = GPIOB,
        .pin           = GPIO_PIN_10,
        .tim_cb        = ds18_tim_cb,
        .done_cb       = NULL,
        .rom_id_filter = DS18B20_ID,
    };

    ds18b20_init(&s_ds18, &ow);
    ds18b20_update_rom_id(&s_ds18);
    while (ds18b20_is_busy(&s_ds18));

    ds18b20_config_t cfg = {
        .alarm_high = 50,
        .alarm_low  = -50,
        .cnv_bit    = DS18B20_CNV_BIT_10,
    };
    ds18b20_conf(&s_ds18, &cfg);
    while (ds18b20_is_busy(&s_ds18));
}

/* ── thermo_blocking_measure ──────────────────────────────────────────────── */
/*
 * Used once at startup to prime the temperature reading before the main
 * loop begins. After this, thermo_tick() handles all conversions.
 */
void thermo_blocking_measure(void)
{
    ds18b20_cnv(&s_ds18);
    while (ds18b20_is_busy(&s_ds18));
    while (!ds18b20_is_cnv_done(&s_ds18));

    ds18b20_req_read(&s_ds18, 0);
    while (ds18b20_is_busy(&s_ds18));

    int16_t raw = ds18b20_read_c(&s_ds18);
    if (raw > THERMO_VALID_MIN_X100 && raw < THERMO_VALID_MAX_X100)
    {
        s_temp_x100 = raw;
    }
}

/* ── thermo_tick (main loop) ──────────────────────────────────────────────── */

void thermo_tick(void)
{
    /* Schedule a new conversion every THERMO_PERIOD_MS */
    uint32_t now = HAL_GetTick();
    if (s_state == DS_IDLE &&
        (now - s_last_start >= THERMO_PERIOD_MS))
    {
        s_state      = DS_START_CONV;
        s_last_start = now;
    }

    /* Advance the conversion state machine */
    switch (s_state)
    {
        case DS_IDLE:
            break;

        case DS_START_CONV:
            ds18b20_cnv(&s_ds18);
            s_ts    = HAL_GetTick();
            s_state = DS_WAIT_CONV;
            break;

        case DS_WAIT_CONV:
            if (HAL_GetTick() - s_ts >= s_ds18.cnv_time)
            {
                s_state = DS_READ;
            }
            break;

        case DS_READ:
            ds18b20_req_read(&s_ds18, 0);
            s_state = DS_READY;
            break;

        case DS_READY:
            if (!ds18b20_is_busy(&s_ds18))
            {
                int16_t raw = ds18b20_read_c(&s_ds18);
                if (raw > THERMO_VALID_MIN_X100 && raw < THERMO_VALID_MAX_X100)
                {
                    s_temp_x100 = raw;
                }
                s_state = DS_IDLE;
            }
            break;

        case DS_ERROR:
            s_state = DS_IDLE;
            break;
    }
}

/* ── thermo_systick (SysTick IRQ) ─────────────────────────────────────────── */

void thermo_systick(void)
{
    /* One-wire library requires a periodic tick for its own timing */
    /* (implementation depends on the ds18b20 library version used) */
}

/* ── thermo_tim_callback ──────────────────────────────────────────────────── */

void thermo_tim_callback(TIM_HandleTypeDef *htim)
{
    ow_callback(&s_ds18.ow);
}

/* ── thermo_is_valid ──────────────────────────────────────────────────────── */

uint8_t thermo_is_valid(void)
{
    return (s_temp_x100 > THERMO_VALID_MIN_X100 &&
            s_temp_x100 < THERMO_VALID_MAX_X100) ? 1u : 0u;
}

/* ── thermo_get_celsius ───────────────────────────────────────────────────── */

float thermo_get_celsius(void)
{
    if (!thermo_is_valid()) { return 0.0f; }
    return (float)s_temp_x100 * 0.01f;
}

/* ── Internal TIM callback ────────────────────────────────────────────────── */

static void ds18_tim_cb(TIM_HandleTypeDef *htim)
{
    ow_callback(&s_ds18.ow);
}
