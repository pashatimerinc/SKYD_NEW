#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

/**
  ******************************************************************************
  * @file    state_machine.h
  * @brief   Drop-system state machine.
  *
  * All SystemState transitions and substate logic live here.
  * The machine is fully self-contained: it reads switches via switches_drv,
  * drives servos via servo_drv, and notifies comm via callbacks — it never
  * touches HAL or global variables directly.
  *
  * Usage:
  *   StateMachineCtx ctx;
  *   sm_init(&ctx);
  *
  *   // every main-loop iteration:
  *   sm_update(&ctx);
  *
  *   // from comm layer, when a remote command arrives:
  *   sm_request_transition(&ctx, DROP_ONE);
  ******************************************************************************
  */

#include <stdint.h>

/* ── System states ────────────────────────────────────────────────────────── */

typedef enum {
    BOOT         = 0,
    DEBUG_STATE  = 1,
    CLIP_LOADING = 2,
    CHECK_LOAD   = 3,
    LOADED       = 4,
    DROP_ONE     = 5,
    DROP_ALL     = 6,
    UNLOADED     = 7,
    ERROR_STATE  = 8,
} SystemState;

/* ── Error codes ──────────────────────────────────────────────────────────── */

typedef enum {
    ERROR_OK    = 0,
    ERROR_STUCK = 1,
} ErrorDevice;

/* ── Context ──────────────────────────────────────────────────────────────── */
/*
 * All state-machine state is in here — no static locals, no globals.
 * app.c owns one instance (g_sm) and passes a pointer everywhere.
 */
typedef struct {
    SystemState current;
    SystemState requested;       /* pending transition requested externally  */
    uint8_t     has_request;     /* flag: a transition is waiting            */

    uint8_t     substate;
    uint32_t    ts;              /* general-purpose timestamp                */
    uint32_t    drop_ts;         /* timestamp for drop timeout               */
    uint8_t     is_retry;        /* retry flag shared by CHECK_LOAD/DROP_*   */
    uint8_t     reached_start;   /* pusher reached start switch              */

    ErrorDevice error;
} StateMachineCtx;

/* ── Substate values (shared by CHECK_LOAD, DROP_ONE, DROP_ALL) ─────────── */

typedef enum {
    SS_START_CHECK          = 0,
    SS_PUSHER_GO            = 1,
    SS_PUSHER_ROLLBACK      = 2,
    SS_STOP_PUSHER          = 3,
    SS_OPEN_STOPER          = 4,
    SS_PUSHER_THROWS        = 5,
    SS_WAITING_ACTION       = 6,
    SS_WAITING_TIMEOUT_END  = 6,  /* alias — same slot, different states use it */
    SS_RETRY                = 7,
    SS_RETRY_CHECK          = 7,  /* alias */
} Substate;

/* ── Public API ───────────────────────────────────────────────────────────── */

/**
  * @brief  Initialise context. Call once before the main loop.
  */
void sm_init(StateMachineCtx *ctx);

/**
  * @brief  Advance the state machine by one tick. Call every main-loop cycle.
  */
void sm_update(StateMachineCtx *ctx);

/**
  * @brief  Request a state transition from outside (comm layer, PWM handler).
  *         The transition is applied on the next sm_update() call.
  *         Ignored if the current state cannot accept the command.
  */
void sm_request_transition(StateMachineCtx *ctx, SystemState next);

/**
  * @brief  Returns the current state (safe to call from any context).
  */
SystemState sm_get_state(const StateMachineCtx *ctx);

/**
  * @brief  Returns the current error code.
  */
ErrorDevice sm_get_error(const StateMachineCtx *ctx);

/**
  * @brief  Returns the loaded/unloaded bitmask bit for this unit.
  *         Comm layer calls this to build the status bitmask.
  *         1 = loaded, 0 = unloaded/error.
  */
uint8_t sm_is_loaded(const StateMachineCtx *ctx);

#endif /* STATE_MACHINE_H */
