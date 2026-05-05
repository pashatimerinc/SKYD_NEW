/**
  ******************************************************************************
  * @file    state_machine.c
  * @brief   Drop-system state machine implementation.
  *
  * Design notes:
  *   - Each state has its own static handler function: state_boot(), etc.
  *   - DROP_ONE and DROP_ALL share drop_sequence_update() — the only logic
  *     difference is whether a successful drop returns to LOADED or loops.
  *   - All timing uses HAL_GetTick() deltas; no busy-waits.
  *   - The context struct (StateMachineCtx) holds all mutable state — no
  *     static locals inside case blocks.
  ******************************************************************************
  */

#include "state_machine.h"
#include "../Drivers/servo_drv.h"
#include "../Drivers/switches_drv.h"
#include "../App/comm.h"
#include "main.h"   /* HAL_GetTick, GPIO for LED */

#include <stddef.h>

/* ── Timeouts (ms) ────────────────────────────────────────────────────────── */

#define TIMEOUT_CLIP_LOADING_MS     6000u
#define TIMEOUT_PUSHER_TO_HOME_MS    200u
#define TIMEOUT_STOPER_SETTLE_MS     300u
#define TIMEOUT_STOPER_OPEN_MS       200u
#define TIMEOUT_PRE_DROP_MS          400u
#define TIMEOUT_DROP_ACTION_MS      8000u
#define TIMEOUT_DROP_RETRY_MS        500u
#define TIMEOUT_CHECK_RETRY_MS       500u
#define TIMEOUT_CHECK_ACTION_MS    16000u
#define TIMEOUT_LOADED_SETTLE_MS     200u
#define TIMEOUT_UNLOADED_HOME_MS     500u
#define TIMEOUT_ERROR_BLINK_MS       300u

/* ── Forward declarations ─────────────────────────────────────────────────── */

static void state_boot(StateMachineCtx *ctx);
static void state_debug(StateMachineCtx *ctx);
static void state_clip_loading(StateMachineCtx *ctx);
static void state_check_load(StateMachineCtx *ctx);
static void state_loaded(StateMachineCtx *ctx);
static void state_drop_one(StateMachineCtx *ctx);
static void state_drop_all(StateMachineCtx *ctx);
static void state_unloaded(StateMachineCtx *ctx);
static void state_error(StateMachineCtx *ctx);

static void drop_sequence_update(StateMachineCtx *ctx, uint8_t loop_on_success);
static void transition_to(StateMachineCtx *ctx, SystemState next);
static void reset_substates(StateMachineCtx *ctx);

/* ── sm_init ──────────────────────────────────────────────────────────────── */

void sm_init(StateMachineCtx *ctx)
{
    ctx->current      = BOOT;
    ctx->requested    = BOOT;
    ctx->has_request  = 0;
    ctx->substate     = SS_START_CHECK;
    ctx->ts           = 0;
    ctx->drop_ts      = 0;
    ctx->is_retry     = 0;
    ctx->reached_start = 0;
    ctx->error        = ERROR_OK;
}

/* ── sm_update ────────────────────────────────────────────────────────────── */

void sm_update(StateMachineCtx *ctx)
{
    /* Apply any externally requested transition first */
    if (ctx->has_request)
    {
        ctx->has_request = 0;
        SystemState req = ctx->requested;

        /* Only honour drop commands when loaded */
        if ((req == DROP_ONE || req == DROP_ALL) && ctx->current == LOADED)
        {
            transition_to(ctx, req);
        }
        /* Debug request is always accepted */
        else if (req == DEBUG_STATE)
        {
            transition_to(ctx, req);
        }
    }

    /* Dispatch to the active state handler */
    switch (ctx->current)
    {
        case BOOT:         state_boot(ctx);         break;
        case DEBUG_STATE:  state_debug(ctx);        break;
        case CLIP_LOADING: state_clip_loading(ctx); break;
        case CHECK_LOAD:   state_check_load(ctx);   break;
        case LOADED:       state_loaded(ctx);       break;
        case DROP_ONE:     state_drop_one(ctx);     break;
        case DROP_ALL:     state_drop_all(ctx);     break;
        case UNLOADED:     state_unloaded(ctx);     break;
        case ERROR_STATE:  state_error(ctx);        break;
        default:           break;
    }
}

/* ── sm_request_transition ────────────────────────────────────────────────── */

void sm_request_transition(StateMachineCtx *ctx, SystemState next)
{
    ctx->requested   = next;
    ctx->has_request = 1;
}

/* ── sm_get_state / sm_get_error / sm_is_loaded ───────────────────────────── */

SystemState sm_get_state(const StateMachineCtx *ctx)  { return ctx->current; }
ErrorDevice sm_get_error(const StateMachineCtx *ctx)  { return ctx->error;   }

uint8_t sm_is_loaded(const StateMachineCtx *ctx)
{
    return (ctx->current == LOADED) ? 1u : 0u;
}

/* ══════════════════════════════════════════════════════════════════════════
   State handlers
   ══════════════════════════════════════════════════════════════════════════ */

/* ── BOOT ─────────────────────────────────────────────────────────────────── */
/*
 * Wait for the input protocol to be resolved (PWM or MAVLink).
 * Once known, branch based on which mechanical switches are active.
 */
static void state_boot(StateMachineCtx *ctx)
{

    if (input_proto_get_mode() == NO_INPUT_MODE) { return; }

    Switches sw = switches_get();

    if      (sw.fixator)  transition_to(ctx, CLIP_LOADING);
    else if (sw.counter)  transition_to(ctx, LOADED);
    else                  transition_to(ctx, CHECK_LOAD);
}

/* ── DEBUG_STATE ──────────────────────────────────────────────────────────── */
/*
 * Manual servo control driven entirely by comm/debug commands.
 * On entry: close stoper, stop pusher.
 * End-stops: stop pusher when it hits start or finish switch.
 */
static void state_debug(StateMachineCtx *ctx)
{
    static uint8_t entered = 0;

    if (!entered)
    {
        servo_stoper_set(SERVO_CLOSE);
        servo_pusher_set(SERVO_STOP);
        entered = 1;
    }

    Switches sw = switches_get();

    if ((sw.start  && servo_pusher_get() == SERVO_HOME) ||
        (sw.finish && servo_pusher_get() == SERVO_DROP))
    {
        servo_pusher_set(SERVO_STOP);
    }
}

/* ── CLIP_LOADING ─────────────────────────────────────────────────────────── */
/*
 * A new clip is being inserted. Open the stoper to receive it.
 * If fixator switch is still active — keep waiting.
 * If no fixator after 6 s — assume done, proceed to CHECK_LOAD.
 */
static void state_clip_loading(StateMachineCtx *ctx)
{
    Switches sw = switches_get();

    /* Keep pusher at home while clip is loading */
    servo_pusher_set(sw.start ? SERVO_STOP : SERVO_HOME);

    if (sw.fixator)
    {
        servo_stoper_set(SERVO_OPEN);
        ctx->ts = HAL_GetTick();   /* reset timeout while fixator active */
    }

    if (HAL_GetTick() - ctx->ts > TIMEOUT_CLIP_LOADING_MS)
    {
        transition_to(ctx, CHECK_LOAD);
    }
}

/* ── CHECK_LOAD ───────────────────────────────────────────────────────────── */
/*
 * Verify whether a payload is in the chamber by advancing the pusher.
 *
 * Substates:
 *   SS_START_CHECK        → close stoper, go to SS_PUSHER_GO
 *   SS_PUSHER_GO          → wait 400 ms, then push
 *   SS_WAITING_TIMEOUT_END → detect fixator / counter / finish / timeout
 *   SS_RETRY_CHECK        → pause 500 ms before second attempt
 */
static void state_check_load(StateMachineCtx *ctx)
{
    Switches sw = switches_get();

    switch (ctx->substate)
    {
        case SS_START_CHECK:
            servo_stoper_set(SERVO_CLOSE);
            ctx->substate = SS_PUSHER_GO;
            ctx->ts       = HAL_GetTick();
            break;

        case SS_PUSHER_GO:
            if (HAL_GetTick() - ctx->ts >= TIMEOUT_PRE_DROP_MS)
            {
                servo_pusher_set(SERVO_DROP);
                ctx->ts       = HAL_GetTick();
                ctx->substate = SS_WAITING_TIMEOUT_END;
            }
            break;

        case SS_WAITING_TIMEOUT_END:
            if (sw.fixator)
            {
                /* Clip detected — go back to loading */
                transition_to(ctx, CLIP_LOADING);
                break;
            }

            if (sw.counter)
            {
                /* Payload counted — chamber is loaded */
                servo_pusher_set(SERVO_STOP);
                transition_to(ctx, LOADED);
                break;
            }

            if (sw.finish)
            {
                /* Pusher reached end without a count — nothing in chamber */
                servo_pusher_set(SERVO_STOP);
                transition_to(ctx, UNLOADED);
                break;
            }

            if (HAL_GetTick() - ctx->ts > TIMEOUT_CHECK_ACTION_MS)
            {
                servo_pusher_set(SERVO_STOP);

                if (ctx->is_retry)
                {
                    ctx->error = ERROR_STUCK;
                    transition_to(ctx, ERROR_STATE);
                }
                else
                {
                    ctx->is_retry = 1;
                    ctx->substate = SS_RETRY_CHECK;
                    ctx->ts       = HAL_GetTick();
                }
            }
            break;

        case SS_RETRY_CHECK:
            if (HAL_GetTick() - ctx->ts >= TIMEOUT_CHECK_RETRY_MS)
            {
                ctx->substate = SS_START_CHECK;
                ctx->ts       = HAL_GetTick();
            }
            break;

        default:
            ctx->substate = SS_START_CHECK;
            break;
    }
}

/* ── LOADED ───────────────────────────────────────────────────────────────── */
/*
 * Chamber has a payload. Wait for a drop command.
 * Stop the pusher, then close the stoper after a short settle delay.
 */
static void state_loaded(StateMachineCtx *ctx)
{
    Switches sw = switches_get();

    if (sw.fixator)
    {
        transition_to(ctx, CLIP_LOADING);
        return;
    }

    if (servo_pusher_get() != SERVO_STOP)
    {
        servo_pusher_set(SERVO_STOP);
        ctx->ts = HAL_GetTick();
    }

    if (HAL_GetTick() - ctx->ts >= TIMEOUT_LOADED_SETTLE_MS)
    {
        servo_stoper_set(SERVO_CLOSE);
    }
}

/* ── DROP_ONE ─────────────────────────────────────────────────────────────── */
/*
 * Drop a single payload, then return to LOADED (or UNLOADED on failure).
 * Delegates the common drop sequence to drop_sequence_update().
 * loop_on_success = 0  →  on successful count, go back to LOADED.
 */
static void state_drop_one(StateMachineCtx *ctx)
{
    drop_sequence_update(ctx, /*loop_on_success=*/0);
}

/* ── DROP_ALL ─────────────────────────────────────────────────────────────── */
/*
 * Drop all payloads continuously until the chamber is empty.
 * Same sequence as DROP_ONE but loop_on_success = 1 keeps cycling.
 */
static void state_drop_all(StateMachineCtx *ctx)
{
    drop_sequence_update(ctx, /*loop_on_success=*/1);
}

/* ── UNLOADED ─────────────────────────────────────────────────────────────── */
/*
 * Chamber is empty. Keep stoper open, let pusher return home.
 * If fixator becomes active → new clip coming → CLIP_LOADING.
 */
static void state_unloaded(StateMachineCtx *ctx)
{
    Switches sw = switches_get();

    if (sw.fixator)
    {
        transition_to(ctx, CLIP_LOADING);
        return;
    }

    servo_stoper_set(SERVO_OPEN);

    /* Step 1: wait for finish switch, then pause before homing */
    if (sw.finish && ctx->substate == 0)
    {
        ctx->ts       = HAL_GetTick();
        ctx->substate = 1;
    }

    if (ctx->substate == 1 && HAL_GetTick() - ctx->ts > TIMEOUT_UNLOADED_HOME_MS)
    {
        servo_pusher_set(SERVO_HOME);
        ctx->substate = 2;
    }

    /* Manual jog: start switch drives pusher forward, release stops it */
    if (sw.start && !sw.finish)
    {
        ctx->reached_start = 1;
        servo_pusher_set(SERVO_DROP);
    }

    if ((!sw.start || sw.finish) && ctx->reached_start)
    {
        servo_pusher_set(SERVO_STOP);
    }
}

/* ── ERROR_STATE ──────────────────────────────────────────────────────────── */
/*
 * Unrecoverable mechanical error (jammed pusher, etc.).
 * Blink the red LED. Comm layer still forwards commands to next unit.
 */
static void state_error(StateMachineCtx *ctx)
{
    static uint32_t blink_ts = 0;
    static uint8_t toggle = 0;
    uint8_t seq = comm_get_sequential_number();


    if (HAL_GetTick() - blink_ts >= TIMEOUT_ERROR_BLINK_MS)
    {
        blink_ts = HAL_GetTick();
        HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);

        if (toggle)
        {
        	update_bitmask_set(seq);
        	toggle = !toggle;
        }
        else
        {
        	update_bitmask_clear(seq);
        	toggle = !toggle;
        }

    }
    if (switches_get().fixator)
    {
    	ctx->error = ERROR_OK;
    	transition_to(ctx, CLIP_LOADING);
    }
}

/* ══════════════════════════════════════════════════════════════════════════
   Shared drop sequence (DROP_ONE and DROP_ALL)
   ══════════════════════════════════════════════════════════════════════════

   Substate flow:
     SS_PUSHER_ROLLBACK   → send pusher home
     SS_STOP_PUSHER       → wait for start switch or 200 ms, then stop
     SS_OPEN_STOPER       → wait 300 ms, open stoper
     SS_PUSHER_THROWS     → wait 200 ms, push payload
     SS_WAITING_ACTION    → wait for counter, finish switch, or timeout
     SS_RETRY             → pause 500 ms before retry
*/

static void drop_sequence_update(StateMachineCtx *ctx, uint8_t loop_on_success)
{
    Switches sw = switches_get();

    /* counter_state tracks the rising/falling edge of the counter switch:
       0 = idle, 1 = switch went high, 2 = switch released → count confirmed */
    static uint8_t counter_edge = 0;

    switch (ctx->substate)
    {
        /* ── Roll the pusher back to home position ── */
        case SS_PUSHER_ROLLBACK:
            servo_pusher_set(SERVO_HOME);
            ctx->ts       = HAL_GetTick();
            ctx->substate = SS_STOP_PUSHER;
            break;

        /* ── Wait until pusher reaches start switch (or 200 ms) ── */
        case SS_STOP_PUSHER:
            if ((HAL_GetTick() - ctx->ts > TIMEOUT_PUSHER_TO_HOME_MS) || sw.start)
            {
                servo_pusher_set(SERVO_STOP);
                ctx->ts       = HAL_GetTick();
                ctx->substate = SS_OPEN_STOPER;
            }
            break;

        /* ── Open stoper and wait for it to settle ── */
        case SS_OPEN_STOPER:
            if (HAL_GetTick() - ctx->ts > TIMEOUT_STOPER_SETTLE_MS)
            {
                servo_stoper_set(SERVO_OPEN);
                ctx->ts       = HAL_GetTick();
                ctx->substate = SS_PUSHER_THROWS;
            }
            break;

        /* ── Delay slightly, then push the payload out ── */
        case SS_PUSHER_THROWS:
            if (HAL_GetTick() - ctx->ts > TIMEOUT_STOPER_OPEN_MS)
            {
                servo_pusher_set(SERVO_DROP);
                ctx->drop_ts  = HAL_GetTick();
                counter_edge  = 0;
                ctx->substate = SS_WAITING_ACTION;
            }
            break;

        /* ── Wait for confirmation or timeout ── */
        case SS_WAITING_ACTION:

            /* Finish switch → pusher hit end without counting → unloaded */
            if (sw.finish)
            {
                servo_pusher_set(SERVO_STOP);
                ctx->reached_start = 0;
                counter_edge = 0;
                transition_to(ctx, UNLOADED);
                break;
            }

            /* Rising edge of counter switch */
            if (sw.counter && counter_edge == 0)
            {
                counter_edge = 1;
                if (loop_on_success)
                {
                    /* DROP_ALL: reset drop timeout on each successful count */
                    ctx->drop_ts = HAL_GetTick();
                }
            }

            /* Falling edge of counter switch → count confirmed */
            if (!sw.counter && counter_edge == 1)
            {
                counter_edge = 2;
                ctx->drop_ts = HAL_GetTick();
            }

            if (sw.counter && counter_edge == 2)
            {


                if (loop_on_success)
                {

                	counter_edge = 0;
                	ctx->drop_ts = HAL_GetTick();
                    /* DROP_ALL: cycle back to push the next one */
//                    ctx->substate = SS_PUSHER_ROLLBACK;
                }
                else
                {
                	counter_edge = 0;
					servo_pusher_set(SERVO_STOP);
                    /* DROP_ONE: done — go back to loaded */
                    transition_to(ctx, LOADED);
                }
                ctx->is_retry = 0;
                break;
            }

            /* Action timeout — retry once, then fault */
            if (HAL_GetTick() - ctx->drop_ts > TIMEOUT_DROP_ACTION_MS)
            {
                counter_edge = 0;
                servo_pusher_set(SERVO_STOP);

                if (ctx->is_retry)
                {
                    ctx->error = ERROR_STUCK;
                    transition_to(ctx, ERROR_STATE);
                }
                else
                {
                    ctx->is_retry = 1;
                    ctx->substate = SS_RETRY;
                    ctx->ts       = HAL_GetTick();
                }
            }
            break;

        /* ── Pause before retry ── */
        case SS_RETRY:
            if (HAL_GetTick() - ctx->ts > TIMEOUT_DROP_RETRY_MS)
            {
                ctx->substate = SS_PUSHER_ROLLBACK;
            }
            break;

        default:
            ctx->substate = SS_PUSHER_ROLLBACK;
            break;
    }
}

/* ── Helpers ──────────────────────────────────────────────────────────────── */

static void reset_substates(StateMachineCtx *ctx)
{
    ctx->substate      = SS_START_CHECK;
    ctx->ts            = HAL_GetTick();
    ctx->drop_ts       = 0;
    ctx->is_retry      = 0;
    ctx->reached_start = 0;
}

static void transition_to(StateMachineCtx *ctx, SystemState next)
{
    ctx->current = next;
    reset_substates(ctx);
}
