/**
  ******************************************************************************
  * @file    app.c
  * @brief   Application orchestration layer.
  *
  * Responsibilities:
  *   - Startup sequence (config load, power rail, driver init)
  *   - Main-loop dispatch: calls each module's tick in the correct order
  *   - SysTick dispatch: delegates to modules, no logic here
  *   - UART/TIM interrupt callbacks: delegate immediately to drivers
  *
  * Rules:
  *   - No HAL calls except through a driver API
  *   - No state machine logic — that lives in state_machine.c
  *   - No MAVLink encoding/decoding — that lives in comm.c
  ******************************************************************************
  */

#include "app.h"

/* Driver layer */
#include "Drivers/flash_drv.h"
#include "Drivers/voltage_drv.h"
#include "Drivers/servo_drv.h"
#include "Drivers/switches_drv.h"
#include "Drivers/thermo_drv.h"
#include "Drivers/heater_drv.h"
#include "Drivers/uart_drv.h"
#include "Drivers/pwm_drv.h"

/* Application layer */
#include "App/config.h"
#include "App/comm.h"
#include "App/input_proto.h"
#include "App/state_machine.h"

/* HAL (only used here for the SysTick LED blink — move to led_drv if desired) */
#include "main.h"

/* ── Private state ────────────────────────────────────────────────────────── */

static StateMachineCtx g_sm;
static uint16_t        g_tick_counter = 0;

/* ── app_init ─────────────────────────────────────────────────────────────── */

void app_init(void)
{
    /* 1. Load persistent config from flash (must be first) */
    config_load();

    /* 2. Init servo PWM outputs */
    servo_init();

    /* 3. Start UART for MAVLink (USART2 always on; USART1 starts as UART,
          input_proto may switch it to PWM capture later) */
    uart_init();

    /* 4. Measure temperature once (blocking — acceptable at startup only) */
    thermo_init();
    thermo_blocking_measure();     /* warms up ds_temp before main loop */

    /* 5. Measure input voltage and configure power rail */
    voltage_init();
    voltage_configure_power_rail();

    /* 6. Seed the state machine */
    sm_init(&g_sm);
}

/* ── app_update ───────────────────────────────────────────────────────────── */

void app_update(void)
{

    /* 1. Resolve input protocol (PWM vs MAVLink) — no-op once locked */
    input_proto_tick();

    /* 2. Process any pending inbound MAVLink messages */
    comm_process_pending(&g_sm);

    /* 3. Non-blocking temperature conversion state machine */
    thermo_tick();

    /* 4. Temperature-based heater control */
    if (thermo_is_valid() &&
        (sm_get_state(&g_sm) == LOADED || sm_get_state(&g_sm) == DEBUG_STATE))
    {
        heater_update(thermo_get_celsius());
    }

    /* 5. Advance the main state machine */
    sm_update(&g_sm);
}

/* ── SysTick callback ─────────────────────────────────────────────────────── */
/*
 * Runs every 1 ms from SysTick IRQ. Keep it short.
 * Each module owns its own timing internally; we just call their tick.
 */
void HAL_SYSTICK_Callback(void)
{
    g_tick_counter++;

    /* Debounce all mechanical switches */
    switches_tick();

    /* Non-blocking DS18B20 one-wire tick */
    thermo_systick();

    /* Slave-unit timeout countdown + bitmask clear */
    comm_systick(&g_sm);

    /* 500 ms heartbeat: LED blink + Skynet broadcast */
    if (g_tick_counter >= 500)
    {
        g_tick_counter = 0;
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
        comm_heartbeat_tick(&g_sm);
    }

    /* Debug data stream (50 ms cadence, only in DEBUG_STATE) */
    comm_debug_stream_tick(&g_sm);

    /* MAVLink status report (1000 ms, unit #1 only, non-debug) */
    comm_status_tick(&g_sm);
}

/* ── UART callbacks ───────────────────────────────────────────────────────── */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uart_rx_callback(huart);   /* feeds mavlink_parse_char, re-arms IT */
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    uart_tx_callback(huart);   /* clears tx_busy flag */
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    uart_error_callback(huart); /* clears ORE, re-arms IT */
}

/* ── TIM callbacks ────────────────────────────────────────────────────────── */

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    pwm_capture_callback(htim); /* pulse measurement + mode detection */
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    thermo_tim_callback(htim);  /* DS18B20 one-wire timing */
}
