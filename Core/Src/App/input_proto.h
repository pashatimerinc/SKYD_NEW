#ifndef INPUT_PROTO_H
#define INPUT_PROTO_H

#include "../App/comm.h"

/**
  ******************************************************************************
  * @file    input_proto.h
  * @brief   Input protocol auto-detection (PWM vs MAVLink UART).
  *
  * Tries PWM first (210 ms window), then falls back to UART (100 ms window),
  * then cycles. Once comm_get_mode() returns a locked mode, input_proto_tick()
  * becomes a no-op.
  ******************************************************************************
  */

/**
  * @brief  Protocol detection tick. Call every main-loop iteration.
  *         No-op once a mode is locked in comm.
  */
void input_proto_tick(void);

/**
  * @brief  Ensures ports where configured correctly.
  * 		Fixing problem of detection of command_mode and deinitialization of uart.
  */
void verify_command_mode(void);

/**
  * @brief  Convenience wrapper — returns comm_get_mode().
  *         Avoids callers depending on comm.h just for mode checks.
  */
InputCommandMode input_proto_get_mode(void);

#endif /* INPUT_PROTO_H */
