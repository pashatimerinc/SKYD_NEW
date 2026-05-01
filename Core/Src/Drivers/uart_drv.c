/**
  ******************************************************************************
  * @file    uart_drv.c
  * @brief   UART transport driver.
  *
  * USART1 = upstream  (to Flight Controller / GCS)
  * USART2 = downstream (to next unit in daisy-chain)
  *
  * RX: interrupt-driven byte-at-a-time, forwarded to comm layer.
  * TX: DMA with a busy flag — if a TX is in flight, new sends are dropped.
  *
  * Reset safety:
  *   After a hard reset (NRST) the DMA controller retains state from before
  *   the reset. Calling MX_USARTx_UART_Init() on a live DMA stream causes
  *   HAL to silently fail or fire a spurious error IRQ on the first byte.
  *   uart_safe_init() always aborts + deinits before reinitialising, which
  *   guarantees a clean slate regardless of prior DMA state.
  *
  * Static TX buffers:
  *   HAL DMA is asynchronous — the buffer must stay valid until
  *   TxCpltCallback fires. Callers build frames on the stack, so
  *   uart_send_* copies into a static buffer before handing off to DMA.
  ******************************************************************************
  */

#include "uart_drv.h"
#include "../App/comm.h"
#include "usart.h"
#include <string.h>

/* ── TX buffer size ───────────────────────────────────────────────────────── */

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

static void uart_safe_init(UART_HandleTypeDef *huart,
                            void (*mx_init)(void),
                            uint8_t *rx_byte);

/* ── uart_init ────────────────────────────────────────────────────────────── */

void uart_init(void)
{
    uart_safe_init(&huart2, MX_USART2_UART_Init, &s_rx_byte_down);
    uart_safe_init(&huart1, MX_USART1_UART_Init, &s_rx_byte_up);
    s_tx_busy_up   = 0;
    s_tx_busy_down = 0;
}

/* ── uart_reinit_up ───────────────────────────────────────────────────────── */
/*
 * Called by input_proto when switching back from PWM capture to UART.
 * Must abort DMA and fully deinit before reinitialising — TIM4 may have
 * left the pin in an output state that causes a framing error on the
 * first received byte.
 */
void uart_reinit_up(void)
{
    uart_safe_init(&huart1, MX_USART1_UART_Init, &s_rx_byte_up);
    s_tx_busy_up = 0;
}

/* ── uart_send_up ─────────────────────────────────────────────────────────── */

void uart_send_up(const uint8_t *buf, uint16_t len)
{
    if (s_tx_busy_up)                              { return; }
    if (len == 0 || len > MAVLINK_MAX_PACKET_LEN)  { return; }

    memcpy(s_tx_buf_up, buf, len);

    s_tx_busy_up = 1;
    if (HAL_UART_Transmit_DMA(&huart1, s_tx_buf_up, len) != HAL_OK)
    {
        /* DMA not available — fall back to blocking TX and clear flag */
        HAL_UART_Transmit(&huart1, s_tx_buf_up, len, 10);
        s_tx_busy_up = 0;
    }
}

/* ── uart_send_down ───────────────────────────────────────────────────────── */

void uart_send_down(const uint8_t *buf, uint16_t len)
{
    if (s_tx_busy_down)                            { return; }
    if (len == 0 || len > MAVLINK_MAX_PACKET_LEN)  { return; }

    memcpy(s_tx_buf_down, buf, len);

    s_tx_busy_down = 1;
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
        comm_on_msg_up(s_rx_byte_up);
        HAL_UART_Receive_IT(&huart1, &s_rx_byte_up, 1);
    }
    else if (huart->Instance == USART2)
    {
        comm_on_msg_down(s_rx_byte_down);
        HAL_UART_Receive_IT(&huart2, &s_rx_byte_down, 1);
    }
}

void uart_tx_callback(UART_HandleTypeDef *huart)
{
    if      (huart->Instance == USART1) { s_tx_busy_up   = 0; }
    else if (huart->Instance == USART2) { s_tx_busy_down = 0; }
}

void uart_error_callback(UART_HandleTypeDef *huart)
{
    /* Clear all error flags */
    if (huart->ErrorCode & HAL_UART_ERROR_ORE) { __HAL_UART_CLEAR_OREFLAG(huart); }
    if (huart->ErrorCode & HAL_UART_ERROR_FE)  { __HAL_UART_CLEAR_FEFLAG(huart);  }
    if (huart->ErrorCode & HAL_UART_ERROR_NE)  { __HAL_UART_CLEAR_NEFLAG(huart);  }

    /* Reset TX busy so the channel can recover */
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

/* ═══════════════════════════════════════════════════════════════════════════
   Private helpers
   ═══════════════════════════════════════════════════════════════════════════ */

/**
  * @brief  Safely (re)initialise a UART peripheral.
  *
  *   1. Abort any in-flight DMA TX/RX — stops the DMA stream and clears
  *      the HAL handle state so the next init starts clean.
  *   2. DeInit the peripheral — resets CR1/CR2/CR3 and disables IRQ.
  *   3. Call the CubeMX-generated init function.
  *   4. Clear any stale error/status flags that may have been latched
  *      during reset (framing error, noise, overrun).
  *   5. Re-arm the RX interrupt.
  */
static void uart_safe_init(UART_HandleTypeDef *huart,
                            void (*mx_init)(void),
                            uint8_t *rx_byte)
{
    /* Step 1 — force UART handle to a known state before anything else.
       After a hard reset RAM retains pre-reset values. huart->gState may
       say BUSY which causes HAL_UART_DeInit to bail early, leaving the
       handle broken. Setting gState = READY first lets DeInit run fully. */
    huart->gState    = HAL_UART_STATE_READY;
    huart->RxState   = HAL_UART_STATE_READY;

    /* Step 2 — reset the DMA handle states the same way.
       hdmatx/hdmarx are linked by mx_init so they may be valid pointers
       from before the reset. If their State says BUSY, HAL_DMA_Init (called
       inside mx_init) will skip reinitialisation entirely.
       We reset State directly — safer than calling HAL_DMA_DeInit on a
       potentially stale handle whose Instance pointer we can't trust. */
    if (huart->hdmatx != NULL) { huart->hdmatx->State = HAL_DMA_STATE_RESET; }
    if (huart->hdmarx != NULL) { huart->hdmarx->State = HAL_DMA_STATE_RESET; }

    /* Step 3 — full peripheral deinit — now safe because gState is READY */
    HAL_UART_DeInit(huart);

    /* Step 4 — reinitialise via CubeMX config (links and inits DMA handles) */
    mx_init();

    /* Step 5 — clear any flags latched during or before reset.
       Reading SR then DR is the STM32F1 prescribed sequence to clear
       RXNE, ORE, FE, NE in one go. */
    volatile uint32_t dummy;
    dummy = huart->Instance->SR;
    dummy = huart->Instance->DR;
    (void)dummy;

    /* Step 6 — ensure DMA TX interrupt is enabled in NVIC.
       CubeMX only enables DMA channel IRQs it knows about. If USART1 TX
       DMA was not explicitly configured in the .ioc, DMA1_Channel4_IRQn
       may not be enabled, so TxCpltCallback never fires.
       Enabling it here is safe — it's a no-op if already enabled. */
    if (huart->Instance == USART1)
    {
        HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
    }
    else if (huart->Instance == USART2)
    {
        HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
    }

    /* Step 7 — arm RX interrupt */
    HAL_UART_Receive_IT(huart, rx_byte, 1);
}
