/**
  ******************************************************************************
  * @file    voltage_drv.c
  * @brief   Input voltage measurement and power rail selection.
  *
  * ADC raw → voltage: V = (raw / 4095) * 3.3 * 8.3 / 3.3 = raw * 8.3 / 4095
  * (8.3 = resistor divider ratio on the hardware)
  *
  * Exponential moving average: V_new = (3 * V_old + V_sample) / 4
  *
  * Power rail selection:
  *   3.0–7.6 V → DC converter path  (DC_SWITCH low,  SERVO_POWER high)
  *   > 7.6 V   → direct servo power (DC_SWITCH high, SERVO_POWER low)
  ******************************************************************************
  */

#include "voltage_drv.h"
#include "adc.h"
#include "main.h"   /* DC_SWITCH_*, SERVO_POWER_*, Read_ADC() */

/* ── Constants ────────────────────────────────────────────────────────────── */

#define VOLTAGE_THRESHOLD_V     7.6f
#define VOLTAGE_MIN_VALID_V     3.0f
#define VOLTAGE_SAMPLE_MS       10u
#define VOLTAGE_WARMUP_MS       500u
#define VOLTAGE_DIVIDER_RATIO   8.3f
#define VOLTAGE_ADC_REF         3.3f
#define VOLTAGE_ADC_MAX         4095.0f

/* ── Private state ────────────────────────────────────────────────────────── */

static float s_voltage = 0.0f;

/* ── voltage_init ─────────────────────────────────────────────────────────── */

void voltage_init(void)
{
    s_voltage = 0.0f;
}

/* ── voltage_configure_power_rail ────────────────────────────────────────── */

void voltage_configure_power_rail(void)
{
    /* Average over VOLTAGE_WARMUP_MS to get a stable reading */
    uint32_t start = HAL_GetTick();
    while (HAL_GetTick() - start < VOLTAGE_WARMUP_MS)
    {
        float sample = VOLTAGE_DIVIDER_RATIO *
                       ((float)Read_ADC() / VOLTAGE_ADC_MAX) * VOLTAGE_ADC_REF;
        s_voltage = (s_voltage * 3.0f + sample) / 4.0f;
        HAL_Delay(VOLTAGE_SAMPLE_MS);
    }

    if (s_voltage > VOLTAGE_MIN_VALID_V && s_voltage <= VOLTAGE_THRESHOLD_V)
    {
        /* Low voltage: route through DC-DC converter */
        HAL_GPIO_WritePin(DC_SWITCH_GPIO_Port,    DC_SWITCH_Pin,    GPIO_PIN_RESET);
        HAL_Delay(30);
        HAL_GPIO_WritePin(SERVO_POWER_GPIO_Port,  SERVO_POWER_Pin,  GPIO_PIN_SET);
    }
    else if (s_voltage > VOLTAGE_THRESHOLD_V)
    {
        /* High voltage: connect directly to servos */
        HAL_GPIO_WritePin(DC_SWITCH_GPIO_Port,    DC_SWITCH_Pin,    GPIO_PIN_SET);
        HAL_Delay(30);
        HAL_GPIO_WritePin(SERVO_POWER_GPIO_Port,  SERVO_POWER_Pin,  GPIO_PIN_RESET);
    }
    /* else: voltage out of range — leave both pins in reset state */
}

/* ── voltage_drv_get ──────────────────────────────────────────────────────── */

float voltage_drv_get(void)
{
    return s_voltage;
}
