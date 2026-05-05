/**
  ******************************************************************************
  * @file    comm.c
  * @brief   MAVLink communication layer.
  *
  * Chain topology:
  *   [Flight Controller] --USART1--> [Unit 1] --USART2--> [Unit 2] --> ...
  *
  *   - USART1 (huart1): upstream — receives commands from FC, sends status up
  *   - USART2 (huart2): downstream — forwards commands to next unit,
  *                                   receives bitmask updates from it
  *
  * Unit numbering:
  *   sequential_number is assigned on first HEARTBEAT/SKYNET hello.
  *   Unit 1 = first in chain (directly connected to FC).
  *   0xFF   = not yet assigned.
  ******************************************************************************
  */

#include "comm.h"

#include "../Drivers/uart_drv.h"
#include "../Drivers/servo_drv.h"
#include "../Drivers/heater_drv.h"
#include "../Drivers/switches_drv.h"
#include "../Drivers/thermo_drv.h"
#include "../Drivers/flash_drv.h"
#include "../App/config.h"
#include "../App/state_machine.h"

#include "my_dialect_new/my_dialect_new/mavlink.h"
#include "main.h"    /* SYS_ID, COMP_ID, MAJOR, MINOR, PATCH, version macros */

#include <string.h>

/* ── MAVLink channel assignments ──────────────────────────────────────────── */

#define MAVLINK_CH_UP    MAVLINK_COMM_0   /* USART1 — upstream   */
#define MAVLINK_CH_DOWN  MAVLINK_COMM_1   /* USART2 — downstream */

/* ── Skynet command IDs ───────────────────────────────────────────────────── */

#define SKYNET_CMD_HELLO      69u
#define SKYNET_CMD_STATUS      0u

/* ── Slave timeout ────────────────────────────────────────────────────────── */

#define SLAVE_TIMEOUT_MS    2000u

/* ── Module state ─────────────────────────────────────────────────────────── */

static InputCommandMode s_mode            = NO_INPUT_MODE;
static uint8_t          s_seq_num         = 0xFF;
static volatile uint32_t s_bitmask        = 0;
static volatile uint16_t s_slave_timeout  = SLAVE_TIMEOUT_MS;
static StateMachineCtx  *s_sm             = NULL;

/* Pending message flags — set in uart_drv RX callback, cleared here */
static volatile uint8_t  s_msg_ready_up   = 0;
static volatile uint8_t  s_msg_ready_down = 0;
static mavlink_message_t s_msg_up;
static mavlink_message_t s_msg_down;
static mavlink_status_t  s_status_up;
static mavlink_status_t  s_status_down;

/* Periodic tick counters — decremented in SysTick, acted on in main loop */
static volatile uint16_t s_debug_timer     = 50;
static volatile uint16_t s_status_timer    = 1000;
static volatile uint16_t s_heartbeat_timer = 500;

/* Pending send flags — set in SysTick, cleared in main loop */
static volatile uint8_t s_send_heartbeat = 0;
static volatile uint8_t s_send_status    = 0;
static volatile uint8_t s_send_debug     = 0;

/* ── Forward declarations ─────────────────────────────────────────────────── */

static void handle_message_up(mavlink_message_t *msg, StateMachineCtx *sm);
static void handle_message_down(mavlink_message_t *msg, StateMachineCtx *sm);

static void handle_heartbeat(mavlink_message_t *msg);
static void handle_skynet(mavlink_message_t *msg, StateMachineCtx *sm);
static void handle_command_long(mavlink_message_t *msg, StateMachineCtx *sm);
static void handle_debug_request(mavlink_message_t *msg, StateMachineCtx *sm);
static void handle_debug_set(mavlink_message_t *msg);
static void handle_debug_do(mavlink_message_t *msg, StateMachineCtx *sm);

static void apply_drop_command(StateMachineCtx *sm, uint8_t target, SystemState cmd);
static void set_mode_and_seq(InputCommandMode mode, uint8_t seq);

static void send_debug_ack_do(uint8_t cmd_id);
static void send_debug_ack_set(uint8_t param_id, float data);
static void send_data_stream(void);



/* ═══════════════════════════════════════════════════════════════════════════
   Public API
   ═══════════════════════════════════════════════════════════════════════════ */

void comm_init(void)
{
    s_mode           = NO_INPUT_MODE;
    s_seq_num        = 0xFF;
    s_bitmask        = 0;
    s_slave_timeout  = SLAVE_TIMEOUT_MS;
    s_msg_ready_up   = 0;
    s_msg_ready_down = 0;
    s_debug_timer    = 50;
    s_status_timer   = 1000;
}

/* ── comm_process_pending ─────────────────────────────────────────────────── */

void comm_process_pending(StateMachineCtx *sm)
{
    /* Keep local bitmask in sync with state machine */
    comm_update_local_bit(sm);

    /* Process inbound messages */
    if (s_msg_ready_up)
    {
        s_msg_ready_up = 0;
        handle_message_up(&s_msg_up, sm);
    }

    if (s_msg_ready_down)
    {
        s_msg_ready_down = 0;
        handle_message_down(&s_msg_down, sm);
    }

    /* Drain periodic send flags — all actual UART sends happen here,
       on the main loop, never from SysTick IRQ context. */
    if (s_send_heartbeat)
    {
        s_send_heartbeat = 0;
        comm_send_skynet(0.0f, (float)SKYNET_CMD_HELLO);
        if (s_seq_num != 0xFF && s_seq_num > 1)
        {
            comm_send_skynet(0.0f, (float)SKYNET_CMD_STATUS);
        }
    }

    if (s_send_status)
    {
        s_send_status = 0;
        comm_send_status(s_bitmask);
    }

    if (s_send_debug)
    {
        s_send_debug = 0;
        send_data_stream();
    }
}

/* ── comm_systick (1 ms) ──────────────────────────────────────────────────── */

void comm_systick(StateMachineCtx *sm)
{
    if (s_slave_timeout > 0)
    {
        s_slave_timeout--;

        if (s_slave_timeout == 0)
        {
            /* Lost contact with downstream unit — clear its bits */
            if (s_seq_num != 0xFF)
            {
                uint32_t mask = ~(0x0Fu >> s_seq_num);
                __disable_irq();
                s_bitmask &= mask;
                __enable_irq();
            }
        }
    }
}

/* ── comm_heartbeat_tick (500 ms) ─────────────────────────────────────────── */

void comm_heartbeat_tick(StateMachineCtx *sm)
{
    /* Called from SysTick — only set flag, never send directly */
    if (s_mode != NO_INPUT_MODE)
    {
        s_send_heartbeat = 1;
    }
}

/* ── comm_debug_stream_tick (every 1 ms, self-throttled to 50 ms) ─────────── */

void comm_debug_stream_tick(StateMachineCtx *sm)
{
    /* Called from SysTick — only set flag */
    if (sm_get_state(sm) != DEBUG_STATE) { return; }

    if (s_debug_timer > 0) { s_debug_timer--; }
    if (s_debug_timer == 0)
    {
        s_debug_timer    = 50;
        s_send_debug     = 1;
    }
}

/* ── comm_status_tick (every 1 ms, self-throttled to 500 ms) ────────────── */

void comm_status_tick(StateMachineCtx *sm)
{
    /* Called from SysTick — only set flag */
    if (s_mode != MAVLINK_MODE)          { return; }
    if (sm_get_state(sm) == DEBUG_STATE) { return; }
    if (s_seq_num != 1)                  { return; }

    if (s_status_timer > 0) { s_status_timer--; }
    if (s_status_timer == 0)
    {
    	s_status_timer = 500;
        s_send_status  = 1;
    }
}

/* ── comm_set_pwm_mode ────────────────────────────────────────────────────── */

void comm_set_pwm_mode(void)
{
    set_mode_and_seq(PWM_MODE, 1);
}

/* ── comm_register_sm ────────────────────────────────────────────────────── */

void comm_register_sm(StateMachineCtx *sm)
{
    s_sm = sm;
}

/* ── comm_notify_pwm_command ──────────────────────────────────────────────── */

void comm_notify_pwm_command(SystemState cmd)
{
    if (s_sm != NULL)
    {
        apply_drop_command(s_sm, s_seq_num, cmd);
    }
}

/* ── Getters ──────────────────────────────────────────────────────────────── */

InputCommandMode comm_get_mode(void)             { return s_mode;      }
uint8_t          comm_get_sequential_number(void){ return s_seq_num;   }
uint32_t         comm_get_bitmask(void)          { return s_bitmask;   }

/* ── comm_update_local_bit ────────────────────────────────────────────────── */

void comm_update_local_bit(const StateMachineCtx *sm)
{
	if (sm->error)         { return; }
    if (s_seq_num == 0xFF) { return; }

    if (sm_is_loaded(sm))
        update_bitmask_set(s_seq_num);
    else
        update_bitmask_clear(s_seq_num);
}

/* ── comm_send_status ─────────────────────────────────────────────────────── */

void comm_send_status(uint32_t bitmask)
{
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];

    mavlink_msg_command_long_pack(
        SYS_ID, COMP_ID, &msg,
        255, 190,
        MAV_CMD_DO_GRIPPER,
        0,
        0, 0,
        (float)bitmask,
        (float)(MAJOR * 10000 + MINOR * 100 + PATCH),
        voltage_drv_get(),
        (float)s_seq_num,
        0
    );

    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    uart_send_up(buf, len);
}

/* ── comm_send_skynet ─────────────────────────────────────────────────────── */

void comm_send_skynet(float target, float command)
{
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];

    mavlink_msg_skynet_pack(
        SYS_ID, COMP_ID, &msg,
        (float)s_seq_num,
        target,
        command,
        (float)s_bitmask
    );

    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

    /* Status (cmd=0) goes upstream; everything else goes downstream */
    if ((uint8_t)command == SKYNET_CMD_STATUS)
        uart_send_up(buf, len);
    else
        uart_send_down(buf, len);
}

/* ═══════════════════════════════════════════════════════════════════════════
   uart_drv callbacks — called from UART RX IRQ
   ═══════════════════════════════════════════════════════════════════════════ */

/*
 * uart_drv calls these when a complete MAVLink frame has been parsed.
 * We only set a flag here — actual processing happens in comm_process_pending()
 * on the main loop, away from IRQ context.
 */
void comm_on_msg_up(uint8_t byte)
{
    if (mavlink_parse_char(MAVLINK_CH_UP, byte, &s_msg_up, &s_status_up))
    {
        s_msg_ready_up = 1;
    }
}

void comm_on_msg_down(uint8_t byte)
{
    if (mavlink_parse_char(MAVLINK_CH_DOWN, byte, &s_msg_down, &s_status_down))
    {
        /* Only forward Skynet messages from downstream */
        if (s_msg_down.msgid == MAVLINK_MSG_ID_SKYNET)
        {
            s_msg_ready_down = 1;
        }
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
   Message dispatch
   ═══════════════════════════════════════════════════════════════════════════ */

static void handle_message_up(mavlink_message_t *msg, StateMachineCtx *sm)
{
    switch (msg->msgid)
    {
        case MAVLINK_MSG_ID_HEARTBEAT:      handle_heartbeat(msg);          break;
        case MAVLINK_MSG_ID_SKYNET:         handle_skynet(msg, sm);         break;
        case MAVLINK_MSG_ID_COMMAND_LONG:   handle_command_long(msg, sm);   break;
        case MAVLINK_MSG_ID_DEBUG_REQUEST:  handle_debug_request(msg, sm);  break;
        case MAVLINK_MSG_ID_DEBUG_SET:      handle_debug_set(msg);          break;
        case MAVLINK_MSG_ID_DEBUG_DO:       handle_debug_do(msg, sm);       break;
        default:                            break;
    }
}

static void handle_message_down(mavlink_message_t *msg, StateMachineCtx *sm)
{
    /* Only Skynet STATUS messages come up from downstream */
    if (msg->msgid == MAVLINK_MSG_ID_SKYNET)
    {
        handle_skynet(msg, sm);
    }
}

/* ── HEARTBEAT ────────────────────────────────────────────────────────────── */

static void handle_heartbeat(mavlink_message_t *msg)
{
    mavlink_heartbeat_t hb;
    mavlink_msg_heartbeat_decode(msg, &hb);

    if (hb.autopilot && s_mode == NO_INPUT_MODE)
    {
        set_mode_and_seq(MAVLINK_MODE, 1);
        verify_command_mode();
    }
}

/* ── SKYNET ───────────────────────────────────────────────────────────────── */

static void handle_skynet(mavlink_message_t *msg, StateMachineCtx *sm)
{
    mavlink_skynet_t sk;
    mavlink_msg_skynet_decode(msg, &sk);

    /* ── Hello / enumeration ── */
    if (sk.command == SKYNET_CMD_HELLO)
    {
        if (s_mode == NO_INPUT_MODE || s_seq_num == 0xFF)
        {
            set_mode_and_seq(MAVLINK_MODE, (uint8_t)sk.sequential_num + 1);
            verify_command_mode();
        }
        return;
    }

    /* ── Status / bitmask propagation from downstream ── */
    if (sk.command == SKYNET_CMD_STATUS)
    {
        s_slave_timeout = SLAVE_TIMEOUT_MS;

        /* Merge downstream bits into our bitmask */
        if (s_seq_num != 0xFF)
        {
            uint32_t downstream_mask = 0x0Fu >> s_seq_num;
            __disable_irq();
            s_bitmask = (s_bitmask & ~downstream_mask) |
                        ((uint32_t)sk.bitmask & downstream_mask);
            __enable_irq();
        }

//        if (s_seq_num != 0 && s_seq_num < 0xFF) comm_send_status(s_bitmask);

        return;
    }

    /* ── Drop commands ── */
    if (sm_get_state(sm) == ERROR_STATE)
    {
        /* We can't drop — forward to next unit */
        comm_send_skynet((float)(s_seq_num + 1), sk.command);
        return;
    }

    SystemState cmd = (SystemState)(uint8_t)sk.command;

    if (sk.target_unit == s_seq_num || sk.target_unit == 0)
    {
        apply_drop_command(sm, s_seq_num, cmd);
    }
    else if (cmd == DROP_ONE || cmd == DROP_ALL)
    {
        comm_send_skynet(sk.target_unit, sk.command);
    }
}

/* ── COMMAND_LONG (MAV_CMD_DO_GRIPPER from FC) ───────────────────────────── */

static void handle_command_long(mavlink_message_t *msg, StateMachineCtx *sm)
{
    mavlink_command_long_t cmd;
    mavlink_msg_command_long_decode(msg, &cmd);

    if (cmd.command != MAV_CMD_DO_GRIPPER) { return; }

    /* Always send status back upstream */
//    comm_send_status(s_bitmask);

    /* Must be addressed to our component */
    if (cmd.target_component != 169) { return; }

    /* Assign sequential number from param6 if not yet set */
    if (s_seq_num == 0xFF)
    {
        s_seq_num = (uint8_t)cmd.param6 + 1;
    }

    uint8_t target = (uint8_t)cmd.param1;

    if (target == s_seq_num)
    {
        /* Addressed to us */
        apply_drop_command(sm, s_seq_num, DROP_ONE);
    }
    else if (target > s_seq_num)
    {
        /* Addressed to a unit further down the chain */
        comm_send_skynet((float)target, (float)DROP_ONE);
    }
    else if (target == 0)
    {
        /* Broadcast drop-all */
        SystemState state = sm_get_state(sm);
        if (state == LOADED)
        {
            sm_request_transition(sm, DROP_ALL);
        }
        else if (state == UNLOADED || state == ERROR_STATE)
        {
            comm_send_skynet((float)(s_seq_num + 1), (float)DROP_ALL);
        }
    }
}

/* ── DEBUG_REQUEST ────────────────────────────────────────────────────────── */

static void handle_debug_request(mavlink_message_t *msg, StateMachineCtx *sm)
{
    (void)msg;  /* no payload fields used */

    if (s_mode == NO_INPUT_MODE)
    {
        set_mode_and_seq(MAVLINK_MODE, 1);
    }

    sm_request_transition(sm, DEBUG_STATE);
}

/* ── DEBUG_SET ────────────────────────────────────────────────────────────── */

static void handle_debug_set(mavlink_message_t *msg)
{
    mavlink_debug_set_t dbg;
    mavlink_msg_debug_set_decode(msg, &dbg);

    Config cfg = config_get();

    switch (dbg.param_id)
    {
        case SET_TRIM_MIN:
            cfg.min_stopper = (uint16_t)dbg.data;
            config_set(&cfg);
            servo_stoper_update_trim(cfg.min_stopper, cfg.max_stopper);
            break;

        case SET_TRIM_MAX:
            cfg.max_stopper = (uint16_t)dbg.data;
            config_set(&cfg);
            servo_stoper_update_trim(cfg.min_stopper, cfg.max_stopper);
            break;

        case SET_HEATER_REVERS:
            cfg.heater_reverse = (uint8_t)dbg.data;
            config_set(&cfg);
            heater_set_reverse(cfg.heater_reverse);
            break;

        case SAVE_DATA:
            config_save();
            break;

        default:
            break;
    }

    send_debug_ack_set(dbg.param_id, dbg.data);
}

/* ── DEBUG_DO ─────────────────────────────────────────────────────────────── */

static void handle_debug_do(mavlink_message_t *msg, StateMachineCtx *sm)
{
    mavlink_debug_do_t dbg;
    mavlink_msg_debug_do_decode(msg, &dbg);

    switch (dbg.command_id)
    {
        case STOPER_OPEN:       servo_stoper_set(SERVO_OPEN);           break;
        case STOPER_CLOSE:      servo_stoper_set(SERVO_CLOSE);          break;
        case PUSHER_HOME:       servo_pusher_set(SERVO_HOME);           break;
        case PUSHER_STOP:       servo_pusher_set(SERVO_STOP);           break;
        case PUSHER_TO_FINISH:  servo_pusher_set(SERVO_DROP);           break;
        case PUSHER_DROP:       sm_request_transition(sm, DROP_ONE);    break;
        case EMERGENCY_STOP:    /* TODO: define emergency behaviour */   break;
        case RESTART_SERVOS:
            servo_stoper_set(SERVO_CLOSE);
            servo_pusher_set(SERVO_STOP);
            break;
        case START_HEAT:    heater_debug_set(1);    break;
        case END_HEAT:      heater_debug_set(0);    break;
        default:            break;
    }

    send_debug_ack_do(dbg.command_id);
}

/* ═══════════════════════════════════════════════════════════════════════════
   Send helpers
   ═══════════════════════════════════════════════════════════════════════════ */

static void send_debug_ack_do(uint8_t cmd_id)
{
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];

    mavlink_msg_debug_do_pack(SYS_ID, COMP_ID, &msg, cmd_id, 1);

    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    uart_send_up(buf, len);
}

static void send_debug_ack_set(uint8_t param_id, float data)
{
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];

    mavlink_msg_debug_set_pack(SYS_ID, COMP_ID, &msg, param_id, data, 1);

    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    uart_send_up(buf, len);
}

static void send_data_stream(void)
{
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];

    Switches sw  = switches_get();
    Config   cfg = config_get();

    mavlink_msg_debug_transmit_pack(
        SYS_ID, COMP_ID, &msg,
        sw.fixator,
        sw.counter,
        sw.start,
        sw.finish,
        (uint8_t)servo_stoper_get(),
        (uint8_t)servo_pusher_get(),
        thermo_get_celsius(),
        cfg.min_stopper,
        cfg.max_stopper,
        cfg.heater_reverse,
        heater_get_switching(),
        (MAJOR * 10000 + MINOR * 100 + PATCH)
    );

    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    uart_send_up(buf, len);
}

/* ═══════════════════════════════════════════════════════════════════════════
   Internal helpers
   ═══════════════════════════════════════════════════════════════════════════ */

static void apply_drop_command(StateMachineCtx *sm, uint8_t target, SystemState cmd)
{
    SystemState state = sm_get_state(sm);

    if (state == LOADED)
    {
        sm_request_transition(sm, cmd);
    }
    else if (state == UNLOADED || state == ERROR_STATE)
    {
        /* We can't execute — forward to next unit */
        comm_send_skynet((float)(s_seq_num + 1), (float)cmd);
    }
    /* In any other state (DROP_ONE in progress, etc.) — silently ignore */
}

static void set_mode_and_seq(InputCommandMode mode, uint8_t seq)
{
    s_mode    = mode;
    s_seq_num = seq;
}

void update_bitmask_set(uint8_t seq)
{
    __disable_irq();
    s_bitmask |= (0x10u >> seq);
    __enable_irq();
}

void update_bitmask_clear(uint8_t seq)
{
    __disable_irq();
    s_bitmask &= ~(0x10u >> seq);
    __enable_irq();
}
