#ifndef PWM_DRV_H
#define PWM_DRV_H

/**
  ******************************************************************************
  * @file    pwm_drv.h
  * @brief   PWM input capture driver (TIM4, PWM-in mode on CH1/CH2).
  *
  * Measures RC-style PWM pulse width (800–2200 µs range).
  * When a stable signal is detected, notifies comm via comm_set_pwm_mode().
  * Decoded commands are forwarded to the state machine via
  * comm_handle_pwm_command().
  *
  * TIM4 is mutually exclusive with USART1 on the same pin.
  * pwm_drv_init() de-inits USART1; uart_reinit_up() reverses this.
  ******************************************************************************
  */

#include <stdint.h>
#include <main.h>

/**
  * @brief  Initialise TIM4 for PWM input capture.
  *         De-inits USART1 before starting — call uart_reinit_up() to undo.
  */
void pwm_drv_init(void);

/**
  * @brief  De-init TIM4 and release the pin back to UART.
  *         Called by input_proto when switching to MAVLINK_MODE.
  */
void pwm_drv_deinit(void);

/**
  * @brief  TIM4 input capture callback — call from HAL_TIM_IC_CaptureCallback.
  */
void pwm_capture_callback(TIM_HandleTypeDef *htim);

#endif /* PWM_DRV_H */
