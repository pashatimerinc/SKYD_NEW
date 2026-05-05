/**
  ******************************************************************************
  * @file    uart_drv.c
  * @brief   UART transport driver.
  *
  * USART1 = upstream  (to Flight Controller / GCS)
  * USART2 = downstream (to next unit in daisy-chain)
  *
  * Keep it simple — match exactly what the original working code did:
  *   - Call MX_USARTx_UART_Init() directly, no DeInit dance
  *   - Clear SR/DR to flush any latched error flags after init
  *   - Arm RX interrupt
  *   - Atomic busy flag check+set to prevent SysTick race
  ******************************************************************************
  */

#include "uart_drv.h"
#include "../App/comm.h"
#include "usart.h"
#include <string.h>

/* ── Config ───────────────────────────────────────────────────────────────── */

#ifndef MAVLINK_MAX_PACKET_LEN
#define MAVLINK_MAX_PACKET_LEN  280u
#endif

/* ── Private state ────────────────────────────────────────────────────────── */

static uint8_t s_rx_byte_up   = 0;
static uint8_t s_rx_byte_down = 0;

static volatile uint8_t s_tx_busy_up   = 0;
static volatile uint8_t s_tx_busy_down = 0;

static uint8_t s_tx_buf_up  [MAVLINK_MAX_PACKET_LEN];
static uint8_t s_tx_buf_down[MAVLINK_MAX_PACKET_LEN];

/* ── Forward declarations ─────────────────────────────────────────────────── */

static void clear_error_flags(UART_HandleTypeDef *huart);

/* ── uart_init ────────────────────────────────────────────────────────────── */

void uart_init(void)
{
    /* USART2 downstream — always on */
    MX_USART2_UART_Init();
    clear_error_flags(&huart2);
    s_tx_busy_down = 0;
    HAL_UART_Receive_IT(&huart2, &s_rx_byte_down, 1);

    /* USART1 upstream */
    uart_reinit_up();
}

/* ── uart_reinit_up ───────────────────────────────────────────────────────── */
/*
 * Called at startup and by input_proto when switching back from PWM capture.
 * Matches exactly what the original START_UP() did — just call the CubeMX
 * init function, clear flags, arm RX. No DeInit needed on a bare-metal system
 * where we control the full boot sequence.
 */
void uart_reinit_up(void)
{
    MX_USART1_UART_Init();
    clear_error_flags(&huart1);
    s_tx_busy_up = 0;
    HAL_UART_Receive_IT(&huart1, &s_rx_byte_up, 1);
}

/* ── uart_send_up ─────────────────────────────────────────────────────────── */

void uart_send_up(const uint8_t *buf, uint16_t len)
{
    if (len == 0 || len > MAVLINK_MAX_PACKET_LEN) { return; }

    /* Atomic check+set — prevents race between main loop and SysTick */
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    uint8_t busy = s_tx_busy_up;
    if (!busy) { s_tx_busy_up = 1; }
    __set_PRIMASK(primask);

    if (busy) { return; }

    memcpy(s_tx_buf_up, buf, len);

    if (HAL_UART_Transmit_IT(&huart1, s_tx_buf_up, len) != HAL_OK)
    {
        s_tx_busy_up = 0;
    }
}

/* ── uart_send_down ───────────────────────────────────────────────────────── */

void uart_send_down(const uint8_t *buf, uint16_t len)
{
    if (len == 0 || len > MAVLINK_MAX_PACKET_LEN) { return; }

    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    uint8_t busy = s_tx_busy_down;
    if (!busy) { s_tx_busy_down = 1; }
    __set_PRIMASK(primask);

    if (busy) { return; }

    memcpy(s_tx_buf_down, buf, len);

    if (HAL_UART_Transmit_DMA(&huart2, s_tx_buf_down, len) != HAL_OK)
    {
        HAL_UART_Transmit(&huart2, s_tx_buf_down, len, 10);
        s_tx_busy_down = 0;
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
   HAL callbacks
   ═══════════════════════════════════════════════════════════════════════════ */

void uart_rx_callback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        uint8_t byte = s_rx_byte_up;
        HAL_UART_Receive_IT(&huart1, &s_rx_byte_up, 1);   /* re-arm first */
        comm_on_msg_up(byte);
    }
    else if (huart->Instance == USART2)
    {
        uint8_t byte = s_rx_byte_down;
        HAL_UART_Receive_IT(&huart2, &s_rx_byte_down, 1);
        comm_on_msg_down(byte);
    }
}

void uart_tx_callback(UART_HandleTypeDef *huart)
{
    if      (huart->Instance == USART1) { s_tx_busy_up   = 0; }
    else if (huart->Instance == USART2) { s_tx_busy_down = 0; }
}

void uart_error_callback(UART_HandleTypeDef *huart)
{
    clear_error_flags(huart);

    if (huart->Instance == USART1)
    {
        s_tx_busy_up = 0;
        HAL_UART_Receive_IT(&huart1, &s_rx_byte_up, 1);
    }
    else if (huart->Instance == USART2)
    {
        s_tx_busy_down = 0;
        HAL_UART_Receive_IT(&huart2, &s_rx_byte_down, 1);
    }
}

/* ── Private helpers ──────────────────────────────────────────────────────── */

/*
 * On STM32F1, reading SR then DR clears RXNE, ORE, FE, NE in one sequence.
 * Call after every init to flush any flags latched during reset or pin float.
 */
static void clear_error_flags(UART_HandleTypeDef *huart)
{
    volatile uint32_t dummy;
    dummy = huart->Instance->SR;
    dummy = huart->Instance->DR;
    (void)dummy;
}
