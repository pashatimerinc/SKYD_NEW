/**
  ******************************************************************************
  * @file    pwm_drv.c
  * @brief   PWM input capture driver.
  *
  * TIM4 is configured in PWM-input mode:
  *   CH1 (rising edge, slave reset) → captures full period
  *   CH2 (falling edge, indirect)   → captures high time
  *
  * Command decoding (RC PWM convention):
  *   900–1100 µs  → arm toggle (enables next command)
  *   1400–1600 µs → DROP_ONE  (requires prior arm)
  *   1900–2100 µs → DROP_ALL  (requires prior arm)
  *
  * Debounce: 5 consecutive stable readings required before acting.
  ******************************************************************************
  */

#include "pwm_drv.h"
#include "../App/comm.h"
#include "../App/state_machine.h"
#include "../Drivers/uart_drv.h"
#include "tim.h"     /* htim4 — CubeMX generated */
#include "usart.h"   /* huart1 */
#include "main.h"

/* ── Pulse range constants (µs) ──────────────────────────────────────────── */

#define PWM_MIN_VALID_US    798u
#define PWM_MAX_VALID_US   2202u

#define PWM_ARM_LOW_US      900u
#define PWM_ARM_HIGH_US    1100u

#define PWM_DROP_ONE_LOW_US   1400u
#define PWM_DROP_ONE_HIGH_US  1600u

#define PWM_DROP_ALL_LOW_US   1900u
#define PWM_DROP_ALL_HIGH_US  2100u

#define PWM_STABLE_COUNT        5u   /* readings to confirm a command    */
#define PWM_DETECT_COUNT       10u   /* readings to confirm mode detect  */
#define PWM_STABLE_WINDOW_US   10u   /* ±10 µs = "same pulse"            */

/* ── Private state ────────────────────────────────────────────────────────── */

static uint8_t s_armed = 0;

/* ── Forward declarations ─────────────────────────────────────────────────── */

static void     tim4_init(void);
static void     handle_pulse(float pulse_us);
static uint8_t  pulse_in_range(float pulse, uint16_t lo, uint16_t hi);

/* ── pwm_drv_init ─────────────────────────────────────────────────────────── */

void pwm_drv_init(void)
{
    /* Disable USART1 IRQ and de-init — pin shared with TIM4 CH1 */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
    HAL_UART_DeInit(&huart1);

    s_armed = 0;
    tim4_init();
}

/* ── pwm_drv_deinit ───────────────────────────────────────────────────────── */

void pwm_drv_deinit(void)
{
    if (htim4.State != HAL_TIM_STATE_READY) { return; }

    HAL_TIM_Base_Stop_IT(&htim4);
    __HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE);
    HAL_NVIC_DisableIRQ(TIM4_IRQn);
    HAL_TIM_Base_DeInit(&htim4);
    HAL_TIM_Base_MspDeInit(&htim4);
}

/* ── pwm_capture_callback ─────────────────────────────────────────────────── */

void pwm_capture_callback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance != TIM4)                        { return; }
    if (htim->Channel  != HAL_TIM_ACTIVE_CHANNEL_1)   { return; }

    uint32_t period   = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    uint32_t high     = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

    (void)period;  /* not used — PWM-in mode resets timer on rising edge */

    float tick_us = (htim->Init.Prescaler + 1) * 1e6f / SystemCoreClock;
    float pulse   = (float)high * tick_us;

    handle_pulse(pulse);
}

/* ═══════════════════════════════════════════════════════════════════════════
   Private helpers
   ═══════════════════════════════════════════════════════════════════════════ */

static void handle_pulse(float pulse_us)
{
    static float   s_last_pulse = 0.0f;
    static uint8_t s_stable_count = 0;
    static uint8_t s_detect_count = 0;

    InputCommandMode mode = comm_get_mode();

    /* ── Mode detection: need 10 valid, stable pulses before locking in ── */
    if (mode == NO_INPUT_MODE)
    {
        if (pulse_us > PWM_MIN_VALID_US && pulse_us < PWM_MAX_VALID_US)
        {
            s_detect_count++;
            if (s_detect_count >= PWM_DETECT_COUNT)
            {
                comm_set_pwm_mode();
            }
        }
        else
        {
            s_detect_count = 0;
        }
        return;
    }

    /* ── Command decoding: 5 stable readings required ── */
    float delta = pulse_us - s_last_pulse;
    if (delta < 0) { delta = -delta; }

    if (delta < PWM_STABLE_WINDOW_US)
    {
        s_stable_count++;
    }
    else
    {
        s_last_pulse   = pulse_us;
        s_stable_count = 0;
        return;
    }

    if (s_stable_count < PWM_STABLE_COUNT) { return; }
    s_stable_count = 0;

    /* Arm on low pulse */
    if (pulse_in_range(pulse_us, PWM_ARM_LOW_US, PWM_ARM_HIGH_US))
    {
        s_armed = 1;
        return;
    }

    if (!s_armed) { return; }

    /* Decode drop commands — require prior arm */
    if (pulse_in_range(pulse_us, PWM_DROP_ONE_LOW_US, PWM_DROP_ONE_HIGH_US))
    {
        s_armed = 0;
        /* NOTE: comm_handle_pwm_command needs StateMachineCtx — passed via
           a registered callback set during app_init(). See note in pwm_drv.h
           if you need to add that indirection. For now we call comm directly
           and let comm fetch the SM context it holds internally. */
        comm_notify_pwm_command(DROP_ONE);
    }
    else if (pulse_in_range(pulse_us, PWM_DROP_ALL_LOW_US, PWM_DROP_ALL_HIGH_US))
    {
        s_armed = 0;
        comm_notify_pwm_command(DROP_ALL);
    }
}

static uint8_t pulse_in_range(float pulse, uint16_t lo, uint16_t hi)
{
    return (pulse > (float)lo && pulse < (float)hi) ? 1u : 0u;
}

/* ── TIM4 initialisation (PWM-input mode) ─────────────────────────────────── */

static void tim4_init(void)
{
    TIM_ClockConfigTypeDef  sClk    = {0};
    TIM_SlaveConfigTypeDef  sSlave  = {0};
    TIM_IC_InitTypeDef      sIC     = {0};
    TIM_MasterConfigTypeDef sMaster = {0};

    htim4.Instance               = TIM4;
    htim4.Init.Prescaler         = 63;
    htim4.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim4.Init.Period            = 65535;
    htim4.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim4) != HAL_OK) { Error_Handler(); }

    sClk.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim4, &sClk) != HAL_OK) { Error_Handler(); }

    if (HAL_TIM_IC_Init(&htim4) != HAL_OK) { Error_Handler(); }

    /* Slave mode: reset counter on rising edge of TI1 — gives period on CH1 */
    sSlave.SlaveMode       = TIM_SLAVEMODE_RESET;
    sSlave.InputTrigger    = TIM_TS_TI1FP1;
    sSlave.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sSlave.TriggerPrescaler = TIM_ICPSC_DIV1;
    sSlave.TriggerFilter   = 0;
    if (HAL_TIM_SlaveConfigSynchro(&htim4, &sSlave) != HAL_OK) { Error_Handler(); }

    /* CH1: direct, rising edge → period */
    sIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    sIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sIC.ICPrescaler = TIM_ICPSC_DIV1;
    sIC.ICFilter    = 0;
    if (HAL_TIM_IC_ConfigChannel(&htim4, &sIC, TIM_CHANNEL_1) != HAL_OK) { Error_Handler(); }

    /* CH2: indirect, falling edge → high time */
    sIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_FALLING;
    sIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
    if (HAL_TIM_IC_ConfigChannel(&htim4, &sIC, TIM_CHANNEL_2) != HAL_OK) { Error_Handler(); }

    sMaster.MasterOutputTrigger = TIM_TRGO_RESET;
    sMaster.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMaster) != HAL_OK) { Error_Handler(); }

    HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
    HAL_TIM_IC_Start(&htim4, TIM_CHANNEL_2);
}
