#ifndef UART_DRV_H
#define UART_DRV_H

/**
  ******************************************************************************
  * @file    uart_drv.h
  * @brief   UART transport driver (USART1 upstream, USART2 downstream).
  *
  * Owns:
  *   - RX interrupt arming and byte routing to comm layer
  *   - DMA TX with busy-flag protection
  *   - Error recovery (overrun, etc.)
  *
  * Does NOT own MAVLink parsing — raw bytes are forwarded to comm via
  * comm_on_msg_up() / comm_on_msg_down().
  ******************************************************************************
  */

#include <stdint.h>
#include <main.h>

/**
  * @brief  Initialise both UARTs and arm RX interrupts.
  *         USART1 starts in UART mode; input_proto may switch it to PWM later.
  */
void uart_init(void);

/**
  * @brief  Re-initialise USART1 as UART after PWM mode was active.
  *         Called by input_proto when switching back from PWM capture.
  */
void uart_reinit_up(void);

/**
  * @brief  Send bytes upstream (USART1) via DMA.
  *         Silently dropped if TX is already in progress.
  */
void uart_send_up(const uint8_t *buf, uint16_t len);

/**
  * @brief  Send bytes downstream (USART2) via DMA.
  *         Silently dropped if TX is already in progress.
  */
void uart_send_down(const uint8_t *buf, uint16_t len);

/* ── HAL callbacks — call these from app.c ─────────────────────────────────*/

void uart_rx_callback(UART_HandleTypeDef *huart);
void uart_tx_callback(UART_HandleTypeDef *huart);
void uart_error_callback(UART_HandleTypeDef *huart);

#endif /* UART_DRV_H */
