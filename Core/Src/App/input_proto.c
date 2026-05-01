/**
  ******************************************************************************
  * @file    input_proto.c
  * @brief   Input protocol auto-detection.
  *
  * Phase 0: Start PWM capture (TIM4). Wait 210 ms.
  * Phase 1: If PWM locked → done. Else start UART (USART1). Wait 100 ms.
  * Phase 2: If MAVLINK locked → done. Else restart from Phase 0.
  *
  * Phase transitions happen on elapsed time only — no blocking.
  * Once comm reports any mode other than NO_INPUT_MODE, all phases stop.
  ******************************************************************************
  */

#include "input_proto.h"
#include "../App/comm.h"
#include "../Drivers/pwm_drv.h"
#include "../Drivers/uart_drv.h"

/* ── Timeouts (ms) ────────────────────────────────────────────────────────── */

#define PWM_DETECT_WINDOW_MS    210u
#define UART_DETECT_WINDOW_MS   100u

/* ── Phase state ──────────────────────────────────────────────────────────── */

typedef enum {
    PHASE_START_PWM  = 0,
    PHASE_WAIT_PWM   = 1,
    PHASE_WAIT_UART  = 2,
} DetectPhase;

/* ── Private state ────────────────────────────────────────────────────────── */

static DetectPhase s_phase  = PHASE_START_PWM;
static uint32_t    s_ts     = 0;

/* ── input_proto_tick ─────────────────────────────────────────────────────── */

void input_proto_tick(void)
{
    /* Already locked — nothing to do */
    if (comm_get_mode() != NO_INPUT_MODE) { return; }

    uint32_t now = HAL_GetTick();

    switch (s_phase)
    {
        /* ── Try PWM ── */
        case PHASE_START_PWM:
            pwm_drv_init();
            s_ts    = now;
            s_phase = PHASE_WAIT_PWM;
            break;

        /* ── Wait for PWM lock (210 ms) ── */
        case PHASE_WAIT_PWM:
            if (comm_get_mode() == PWM_MODE) { return; }

            if (now - s_ts > PWM_DETECT_WINDOW_MS)
            {
                /* No PWM signal — de-init TIM4, try UART */
                pwm_drv_deinit();
                uart_reinit_up();
                s_ts    = now;
                s_phase = PHASE_WAIT_UART;
            }
            break;

        /* ── Wait for MAVLINK lock (100 ms) ── */
        case PHASE_WAIT_UART:
            if (comm_get_mode() == MAVLINK_MODE) { return; }

            if (now - s_ts > UART_DETECT_WINDOW_MS)
            {
                /* No MAVLink — cycle back to PWM */
                s_phase = PHASE_START_PWM;
            }
            break;
    }
}

InputCommandMode input_proto_get_mode(void)
{
    return comm_get_mode();
}
