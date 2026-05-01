#ifndef COMM_H
#define COMM_H

/**
  ******************************************************************************
  * @file    comm.h
  * @brief   MAVLink communication layer.
  *
  * Responsibilities:
  *   - Parse inbound MAVLink messages (USART1 = upstream FC, USART2 = downstream
  *     slave unit)
  *   - Dispatch commands to the state machine or forward them down the chain
  *   - Send status reports, Skynet messages, and debug streams upstream
  *   - Track unit identity (sequential_number) and chain bitmask
  *   - Manage slave-unit timeout
  *
  * What this module does NOT do:
  *   - Touch HAL_UART directly — all transport goes through uart_drv
  *   - Contain any state machine logic
  *   - Block
  *
  * Typical call sequence from app.c:
  *
  *   // From main loop:
  *   comm_process_pending(&g_sm);
  *
  *   // From SysTick (1 ms):
  *   comm_systick(&g_sm);
  *
  *   // From SysTick (every 500 ticks):
  *   comm_heartbeat_tick(&g_sm);
  *
  *   // From SysTick (every 50 ticks, DEBUG_STATE only):
  *   comm_debug_stream_tick(&g_sm);
  *
  *   // From SysTick (every 1000 ticks, unit #1 only):
  *   comm_status_tick(&g_sm);
  ******************************************************************************
  */

#include <stdint.h>
#include "../App/state_machine.h"

/* ── Input/command modes ──────────────────────────────────────────────────── */

typedef enum {
    NO_INPUT_MODE  = 0,
    MAVLINK_MODE   = 1,
    PWM_MODE       = 2,
} InputCommandMode;

/* ── Public API ───────────────────────────────────────────────────────────── */

/**
  * @brief  Initialise module state. Call once at startup.
  */
void comm_init(void);

/**
  * @brief  Process all pending inbound MAVLink messages.
  *         Call every main-loop iteration.
  */
void comm_process_pending(StateMachineCtx *sm);

/**
  * @brief  1 ms tick: slave-unit timeout countdown.
  *         Call from HAL_SYSTICK_Callback.
  */
void comm_systick(StateMachineCtx *sm);

/**
  * @brief  500 ms tick: LED heartbeat + Skynet chain broadcast.
  *         Call from HAL_SYSTICK_Callback when tick_counter wraps.
  */
void comm_heartbeat_tick(StateMachineCtx *sm);

/**
  * @brief  50 ms tick: debug data stream (only active in DEBUG_STATE).
  *         Call from HAL_SYSTICK_Callback every tick.
  */
void comm_debug_stream_tick(StateMachineCtx *sm);

/**
  * @brief  1000 ms tick: send MAVLink status upstream (unit #1 only).
  *         Call from HAL_SYSTICK_Callback every tick.
  */
void comm_status_tick(StateMachineCtx *sm);

/**
  * @brief  Notify comm that PWM mode has been detected.
  *         Called by pwm_drv when a stable PWM signal is confirmed.
  */
void comm_set_pwm_mode(void);

/**
  * @brief  Called by pwm_drv when a decoded PWM command arrives.
  *         Applies drop commands or forwards them to the next unit.
  */
void comm_notify_pwm_command(SystemState cmd);

/**
  * @brief  Register the StateMachineCtx pointer used by comm_notify_pwm_command.
  *         Call once from app_init() after sm_init().
  */
void comm_register_sm(StateMachineCtx *sm);

/**
  * @brief  Returns the current input/command mode.
  */
InputCommandMode comm_get_mode(void);

/**
  * @brief  Returns this unit's sequential number in the daisy-chain.
  *         0xFF = not yet assigned.
  */
uint8_t comm_get_sequential_number(void);

/**
  * @brief  Returns the current chain-wide loaded bitmask.
  */
uint32_t comm_get_bitmask(void);

/**
  * @brief  Update the local bit in the bitmask to reflect the SM state.
  *         Called by state machine transitions (via app_update order).
  */
void comm_update_local_bit(const StateMachineCtx *sm);

/* ── Send helpers (also used by debug command handlers) ──────────────────── */

void comm_send_status(uint32_t bitmask);
void comm_send_skynet(float target, float command);

#endif /* COMM_H */
