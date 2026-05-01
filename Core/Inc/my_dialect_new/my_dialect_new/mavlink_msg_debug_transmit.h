#pragma once
// MESSAGE DEBUG_TRANSMIT PACKING

#include <stdint.h>

#define MAVLINK_MSG_ID_DEBUG_TRANSMIT 101


typedef struct __mavlink_debug_transmit_t {
 float temparature; /*<   current temparature reading*/
 uint32_t fw_version; /*<  Firmware version*/
 uint16_t stoper_position; /*<   current stoper position*/
 uint16_t pusher_position; /*<   current pusher position*/
 uint16_t min_stopper; /*<   minimum value of the stopper*/
 uint16_t max_stopper; /*<   maximum value of the stopper*/
 uint16_t flag_mask; /*<   different flags*/
 uint8_t fixator_switch_state; /*<   fixator switch current state*/
 uint8_t counter_switch_state; /*<   counter switch current state*/
 uint8_t start_switch_state; /*<   start switch current state*/
 uint8_t end_switch_state; /*<   end switch current state*/
 uint8_t heating_reverse; /*<   flag defining heating reverse*/
} mavlink_debug_transmit_t;

#define MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN 23
#define MAVLINK_MSG_ID_DEBUG_TRANSMIT_MIN_LEN 23
#define MAVLINK_MSG_ID_101_LEN 23
#define MAVLINK_MSG_ID_101_MIN_LEN 23

#define MAVLINK_MSG_ID_DEBUG_TRANSMIT_CRC 194
#define MAVLINK_MSG_ID_101_CRC 194



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_DEBUG_TRANSMIT { \
    101, \
    "DEBUG_TRANSMIT", \
    12, \
    {  { "fixator_switch_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_debug_transmit_t, fixator_switch_state) }, \
         { "counter_switch_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_debug_transmit_t, counter_switch_state) }, \
         { "start_switch_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_debug_transmit_t, start_switch_state) }, \
         { "end_switch_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 21, offsetof(mavlink_debug_transmit_t, end_switch_state) }, \
         { "stoper_position", NULL, MAVLINK_TYPE_UINT16_T, 0, 8, offsetof(mavlink_debug_transmit_t, stoper_position) }, \
         { "pusher_position", NULL, MAVLINK_TYPE_UINT16_T, 0, 10, offsetof(mavlink_debug_transmit_t, pusher_position) }, \
         { "temparature", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_debug_transmit_t, temparature) }, \
         { "min_stopper", NULL, MAVLINK_TYPE_UINT16_T, 0, 12, offsetof(mavlink_debug_transmit_t, min_stopper) }, \
         { "max_stopper", NULL, MAVLINK_TYPE_UINT16_T, 0, 14, offsetof(mavlink_debug_transmit_t, max_stopper) }, \
         { "heating_reverse", NULL, MAVLINK_TYPE_UINT8_T, 0, 22, offsetof(mavlink_debug_transmit_t, heating_reverse) }, \
         { "flag_mask", NULL, MAVLINK_TYPE_UINT16_T, 0, 16, offsetof(mavlink_debug_transmit_t, flag_mask) }, \
         { "fw_version", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_debug_transmit_t, fw_version) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_DEBUG_TRANSMIT { \
    "DEBUG_TRANSMIT", \
    12, \
    {  { "fixator_switch_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_debug_transmit_t, fixator_switch_state) }, \
         { "counter_switch_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_debug_transmit_t, counter_switch_state) }, \
         { "start_switch_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_debug_transmit_t, start_switch_state) }, \
         { "end_switch_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 21, offsetof(mavlink_debug_transmit_t, end_switch_state) }, \
         { "stoper_position", NULL, MAVLINK_TYPE_UINT16_T, 0, 8, offsetof(mavlink_debug_transmit_t, stoper_position) }, \
         { "pusher_position", NULL, MAVLINK_TYPE_UINT16_T, 0, 10, offsetof(mavlink_debug_transmit_t, pusher_position) }, \
         { "temparature", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_debug_transmit_t, temparature) }, \
         { "min_stopper", NULL, MAVLINK_TYPE_UINT16_T, 0, 12, offsetof(mavlink_debug_transmit_t, min_stopper) }, \
         { "max_stopper", NULL, MAVLINK_TYPE_UINT16_T, 0, 14, offsetof(mavlink_debug_transmit_t, max_stopper) }, \
         { "heating_reverse", NULL, MAVLINK_TYPE_UINT8_T, 0, 22, offsetof(mavlink_debug_transmit_t, heating_reverse) }, \
         { "flag_mask", NULL, MAVLINK_TYPE_UINT16_T, 0, 16, offsetof(mavlink_debug_transmit_t, flag_mask) }, \
         { "fw_version", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_debug_transmit_t, fw_version) }, \
         } \
}
#endif

/**
 * @brief Pack a debug_transmit message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param fixator_switch_state   fixator switch current state
 * @param counter_switch_state   counter switch current state
 * @param start_switch_state   start switch current state
 * @param end_switch_state   end switch current state
 * @param stoper_position   current stoper position
 * @param pusher_position   current pusher position
 * @param temparature   current temparature reading
 * @param min_stopper   minimum value of the stopper
 * @param max_stopper   maximum value of the stopper
 * @param heating_reverse   flag defining heating reverse
 * @param flag_mask   different flags
 * @param fw_version  Firmware version
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_debug_transmit_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t fixator_switch_state, uint8_t counter_switch_state, uint8_t start_switch_state, uint8_t end_switch_state, uint16_t stoper_position, uint16_t pusher_position, float temparature, uint16_t min_stopper, uint16_t max_stopper, uint8_t heating_reverse, uint16_t flag_mask, uint32_t fw_version)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN];
    _mav_put_float(buf, 0, temparature);
    _mav_put_uint32_t(buf, 4, fw_version);
    _mav_put_uint16_t(buf, 8, stoper_position);
    _mav_put_uint16_t(buf, 10, pusher_position);
    _mav_put_uint16_t(buf, 12, min_stopper);
    _mav_put_uint16_t(buf, 14, max_stopper);
    _mav_put_uint16_t(buf, 16, flag_mask);
    _mav_put_uint8_t(buf, 18, fixator_switch_state);
    _mav_put_uint8_t(buf, 19, counter_switch_state);
    _mav_put_uint8_t(buf, 20, start_switch_state);
    _mav_put_uint8_t(buf, 21, end_switch_state);
    _mav_put_uint8_t(buf, 22, heating_reverse);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN);
#else
    mavlink_debug_transmit_t packet;
    packet.temparature = temparature;
    packet.fw_version = fw_version;
    packet.stoper_position = stoper_position;
    packet.pusher_position = pusher_position;
    packet.min_stopper = min_stopper;
    packet.max_stopper = max_stopper;
    packet.flag_mask = flag_mask;
    packet.fixator_switch_state = fixator_switch_state;
    packet.counter_switch_state = counter_switch_state;
    packet.start_switch_state = start_switch_state;
    packet.end_switch_state = end_switch_state;
    packet.heating_reverse = heating_reverse;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEBUG_TRANSMIT;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_DEBUG_TRANSMIT_MIN_LEN, MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN, MAVLINK_MSG_ID_DEBUG_TRANSMIT_CRC);
}

/**
 * @brief Pack a debug_transmit message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param fixator_switch_state   fixator switch current state
 * @param counter_switch_state   counter switch current state
 * @param start_switch_state   start switch current state
 * @param end_switch_state   end switch current state
 * @param stoper_position   current stoper position
 * @param pusher_position   current pusher position
 * @param temparature   current temparature reading
 * @param min_stopper   minimum value of the stopper
 * @param max_stopper   maximum value of the stopper
 * @param heating_reverse   flag defining heating reverse
 * @param flag_mask   different flags
 * @param fw_version  Firmware version
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_debug_transmit_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t fixator_switch_state, uint8_t counter_switch_state, uint8_t start_switch_state, uint8_t end_switch_state, uint16_t stoper_position, uint16_t pusher_position, float temparature, uint16_t min_stopper, uint16_t max_stopper, uint8_t heating_reverse, uint16_t flag_mask, uint32_t fw_version)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN];
    _mav_put_float(buf, 0, temparature);
    _mav_put_uint32_t(buf, 4, fw_version);
    _mav_put_uint16_t(buf, 8, stoper_position);
    _mav_put_uint16_t(buf, 10, pusher_position);
    _mav_put_uint16_t(buf, 12, min_stopper);
    _mav_put_uint16_t(buf, 14, max_stopper);
    _mav_put_uint16_t(buf, 16, flag_mask);
    _mav_put_uint8_t(buf, 18, fixator_switch_state);
    _mav_put_uint8_t(buf, 19, counter_switch_state);
    _mav_put_uint8_t(buf, 20, start_switch_state);
    _mav_put_uint8_t(buf, 21, end_switch_state);
    _mav_put_uint8_t(buf, 22, heating_reverse);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN);
#else
    mavlink_debug_transmit_t packet;
    packet.temparature = temparature;
    packet.fw_version = fw_version;
    packet.stoper_position = stoper_position;
    packet.pusher_position = pusher_position;
    packet.min_stopper = min_stopper;
    packet.max_stopper = max_stopper;
    packet.flag_mask = flag_mask;
    packet.fixator_switch_state = fixator_switch_state;
    packet.counter_switch_state = counter_switch_state;
    packet.start_switch_state = start_switch_state;
    packet.end_switch_state = end_switch_state;
    packet.heating_reverse = heating_reverse;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEBUG_TRANSMIT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_DEBUG_TRANSMIT_MIN_LEN, MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN, MAVLINK_MSG_ID_DEBUG_TRANSMIT_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_DEBUG_TRANSMIT_MIN_LEN, MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN);
#endif
}

/**
 * @brief Pack a debug_transmit message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param fixator_switch_state   fixator switch current state
 * @param counter_switch_state   counter switch current state
 * @param start_switch_state   start switch current state
 * @param end_switch_state   end switch current state
 * @param stoper_position   current stoper position
 * @param pusher_position   current pusher position
 * @param temparature   current temparature reading
 * @param min_stopper   minimum value of the stopper
 * @param max_stopper   maximum value of the stopper
 * @param heating_reverse   flag defining heating reverse
 * @param flag_mask   different flags
 * @param fw_version  Firmware version
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_debug_transmit_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t fixator_switch_state,uint8_t counter_switch_state,uint8_t start_switch_state,uint8_t end_switch_state,uint16_t stoper_position,uint16_t pusher_position,float temparature,uint16_t min_stopper,uint16_t max_stopper,uint8_t heating_reverse,uint16_t flag_mask,uint32_t fw_version)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN];
    _mav_put_float(buf, 0, temparature);
    _mav_put_uint32_t(buf, 4, fw_version);
    _mav_put_uint16_t(buf, 8, stoper_position);
    _mav_put_uint16_t(buf, 10, pusher_position);
    _mav_put_uint16_t(buf, 12, min_stopper);
    _mav_put_uint16_t(buf, 14, max_stopper);
    _mav_put_uint16_t(buf, 16, flag_mask);
    _mav_put_uint8_t(buf, 18, fixator_switch_state);
    _mav_put_uint8_t(buf, 19, counter_switch_state);
    _mav_put_uint8_t(buf, 20, start_switch_state);
    _mav_put_uint8_t(buf, 21, end_switch_state);
    _mav_put_uint8_t(buf, 22, heating_reverse);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN);
#else
    mavlink_debug_transmit_t packet;
    packet.temparature = temparature;
    packet.fw_version = fw_version;
    packet.stoper_position = stoper_position;
    packet.pusher_position = pusher_position;
    packet.min_stopper = min_stopper;
    packet.max_stopper = max_stopper;
    packet.flag_mask = flag_mask;
    packet.fixator_switch_state = fixator_switch_state;
    packet.counter_switch_state = counter_switch_state;
    packet.start_switch_state = start_switch_state;
    packet.end_switch_state = end_switch_state;
    packet.heating_reverse = heating_reverse;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEBUG_TRANSMIT;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_DEBUG_TRANSMIT_MIN_LEN, MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN, MAVLINK_MSG_ID_DEBUG_TRANSMIT_CRC);
}

/**
 * @brief Encode a debug_transmit struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param debug_transmit C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_debug_transmit_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_debug_transmit_t* debug_transmit)
{
    return mavlink_msg_debug_transmit_pack(system_id, component_id, msg, debug_transmit->fixator_switch_state, debug_transmit->counter_switch_state, debug_transmit->start_switch_state, debug_transmit->end_switch_state, debug_transmit->stoper_position, debug_transmit->pusher_position, debug_transmit->temparature, debug_transmit->min_stopper, debug_transmit->max_stopper, debug_transmit->heating_reverse, debug_transmit->flag_mask, debug_transmit->fw_version);
}

/**
 * @brief Encode a debug_transmit struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param debug_transmit C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_debug_transmit_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_debug_transmit_t* debug_transmit)
{
    return mavlink_msg_debug_transmit_pack_chan(system_id, component_id, chan, msg, debug_transmit->fixator_switch_state, debug_transmit->counter_switch_state, debug_transmit->start_switch_state, debug_transmit->end_switch_state, debug_transmit->stoper_position, debug_transmit->pusher_position, debug_transmit->temparature, debug_transmit->min_stopper, debug_transmit->max_stopper, debug_transmit->heating_reverse, debug_transmit->flag_mask, debug_transmit->fw_version);
}

/**
 * @brief Encode a debug_transmit struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param debug_transmit C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_debug_transmit_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_debug_transmit_t* debug_transmit)
{
    return mavlink_msg_debug_transmit_pack_status(system_id, component_id, _status, msg,  debug_transmit->fixator_switch_state, debug_transmit->counter_switch_state, debug_transmit->start_switch_state, debug_transmit->end_switch_state, debug_transmit->stoper_position, debug_transmit->pusher_position, debug_transmit->temparature, debug_transmit->min_stopper, debug_transmit->max_stopper, debug_transmit->heating_reverse, debug_transmit->flag_mask, debug_transmit->fw_version);
}

/**
 * @brief Send a debug_transmit message
 * @param chan MAVLink channel to send the message
 *
 * @param fixator_switch_state   fixator switch current state
 * @param counter_switch_state   counter switch current state
 * @param start_switch_state   start switch current state
 * @param end_switch_state   end switch current state
 * @param stoper_position   current stoper position
 * @param pusher_position   current pusher position
 * @param temparature   current temparature reading
 * @param min_stopper   minimum value of the stopper
 * @param max_stopper   maximum value of the stopper
 * @param heating_reverse   flag defining heating reverse
 * @param flag_mask   different flags
 * @param fw_version  Firmware version
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_debug_transmit_send(mavlink_channel_t chan, uint8_t fixator_switch_state, uint8_t counter_switch_state, uint8_t start_switch_state, uint8_t end_switch_state, uint16_t stoper_position, uint16_t pusher_position, float temparature, uint16_t min_stopper, uint16_t max_stopper, uint8_t heating_reverse, uint16_t flag_mask, uint32_t fw_version)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN];
    _mav_put_float(buf, 0, temparature);
    _mav_put_uint32_t(buf, 4, fw_version);
    _mav_put_uint16_t(buf, 8, stoper_position);
    _mav_put_uint16_t(buf, 10, pusher_position);
    _mav_put_uint16_t(buf, 12, min_stopper);
    _mav_put_uint16_t(buf, 14, max_stopper);
    _mav_put_uint16_t(buf, 16, flag_mask);
    _mav_put_uint8_t(buf, 18, fixator_switch_state);
    _mav_put_uint8_t(buf, 19, counter_switch_state);
    _mav_put_uint8_t(buf, 20, start_switch_state);
    _mav_put_uint8_t(buf, 21, end_switch_state);
    _mav_put_uint8_t(buf, 22, heating_reverse);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_TRANSMIT, buf, MAVLINK_MSG_ID_DEBUG_TRANSMIT_MIN_LEN, MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN, MAVLINK_MSG_ID_DEBUG_TRANSMIT_CRC);
#else
    mavlink_debug_transmit_t packet;
    packet.temparature = temparature;
    packet.fw_version = fw_version;
    packet.stoper_position = stoper_position;
    packet.pusher_position = pusher_position;
    packet.min_stopper = min_stopper;
    packet.max_stopper = max_stopper;
    packet.flag_mask = flag_mask;
    packet.fixator_switch_state = fixator_switch_state;
    packet.counter_switch_state = counter_switch_state;
    packet.start_switch_state = start_switch_state;
    packet.end_switch_state = end_switch_state;
    packet.heating_reverse = heating_reverse;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_TRANSMIT, (const char *)&packet, MAVLINK_MSG_ID_DEBUG_TRANSMIT_MIN_LEN, MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN, MAVLINK_MSG_ID_DEBUG_TRANSMIT_CRC);
#endif
}

/**
 * @brief Send a debug_transmit message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_debug_transmit_send_struct(mavlink_channel_t chan, const mavlink_debug_transmit_t* debug_transmit)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_debug_transmit_send(chan, debug_transmit->fixator_switch_state, debug_transmit->counter_switch_state, debug_transmit->start_switch_state, debug_transmit->end_switch_state, debug_transmit->stoper_position, debug_transmit->pusher_position, debug_transmit->temparature, debug_transmit->min_stopper, debug_transmit->max_stopper, debug_transmit->heating_reverse, debug_transmit->flag_mask, debug_transmit->fw_version);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_TRANSMIT, (const char *)debug_transmit, MAVLINK_MSG_ID_DEBUG_TRANSMIT_MIN_LEN, MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN, MAVLINK_MSG_ID_DEBUG_TRANSMIT_CRC);
#endif
}

#if MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_debug_transmit_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t fixator_switch_state, uint8_t counter_switch_state, uint8_t start_switch_state, uint8_t end_switch_state, uint16_t stoper_position, uint16_t pusher_position, float temparature, uint16_t min_stopper, uint16_t max_stopper, uint8_t heating_reverse, uint16_t flag_mask, uint32_t fw_version)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, temparature);
    _mav_put_uint32_t(buf, 4, fw_version);
    _mav_put_uint16_t(buf, 8, stoper_position);
    _mav_put_uint16_t(buf, 10, pusher_position);
    _mav_put_uint16_t(buf, 12, min_stopper);
    _mav_put_uint16_t(buf, 14, max_stopper);
    _mav_put_uint16_t(buf, 16, flag_mask);
    _mav_put_uint8_t(buf, 18, fixator_switch_state);
    _mav_put_uint8_t(buf, 19, counter_switch_state);
    _mav_put_uint8_t(buf, 20, start_switch_state);
    _mav_put_uint8_t(buf, 21, end_switch_state);
    _mav_put_uint8_t(buf, 22, heating_reverse);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_TRANSMIT, buf, MAVLINK_MSG_ID_DEBUG_TRANSMIT_MIN_LEN, MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN, MAVLINK_MSG_ID_DEBUG_TRANSMIT_CRC);
#else
    mavlink_debug_transmit_t *packet = (mavlink_debug_transmit_t *)msgbuf;
    packet->temparature = temparature;
    packet->fw_version = fw_version;
    packet->stoper_position = stoper_position;
    packet->pusher_position = pusher_position;
    packet->min_stopper = min_stopper;
    packet->max_stopper = max_stopper;
    packet->flag_mask = flag_mask;
    packet->fixator_switch_state = fixator_switch_state;
    packet->counter_switch_state = counter_switch_state;
    packet->start_switch_state = start_switch_state;
    packet->end_switch_state = end_switch_state;
    packet->heating_reverse = heating_reverse;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_TRANSMIT, (const char *)packet, MAVLINK_MSG_ID_DEBUG_TRANSMIT_MIN_LEN, MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN, MAVLINK_MSG_ID_DEBUG_TRANSMIT_CRC);
#endif
}
#endif

#endif

// MESSAGE DEBUG_TRANSMIT UNPACKING


/**
 * @brief Get field fixator_switch_state from debug_transmit message
 *
 * @return   fixator switch current state
 */
static inline uint8_t mavlink_msg_debug_transmit_get_fixator_switch_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  18);
}

/**
 * @brief Get field counter_switch_state from debug_transmit message
 *
 * @return   counter switch current state
 */
static inline uint8_t mavlink_msg_debug_transmit_get_counter_switch_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  19);
}

/**
 * @brief Get field start_switch_state from debug_transmit message
 *
 * @return   start switch current state
 */
static inline uint8_t mavlink_msg_debug_transmit_get_start_switch_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  20);
}

/**
 * @brief Get field end_switch_state from debug_transmit message
 *
 * @return   end switch current state
 */
static inline uint8_t mavlink_msg_debug_transmit_get_end_switch_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  21);
}

/**
 * @brief Get field stoper_position from debug_transmit message
 *
 * @return   current stoper position
 */
static inline uint16_t mavlink_msg_debug_transmit_get_stoper_position(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  8);
}

/**
 * @brief Get field pusher_position from debug_transmit message
 *
 * @return   current pusher position
 */
static inline uint16_t mavlink_msg_debug_transmit_get_pusher_position(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  10);
}

/**
 * @brief Get field temparature from debug_transmit message
 *
 * @return   current temparature reading
 */
static inline float mavlink_msg_debug_transmit_get_temparature(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field min_stopper from debug_transmit message
 *
 * @return   minimum value of the stopper
 */
static inline uint16_t mavlink_msg_debug_transmit_get_min_stopper(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  12);
}

/**
 * @brief Get field max_stopper from debug_transmit message
 *
 * @return   maximum value of the stopper
 */
static inline uint16_t mavlink_msg_debug_transmit_get_max_stopper(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  14);
}

/**
 * @brief Get field heating_reverse from debug_transmit message
 *
 * @return   flag defining heating reverse
 */
static inline uint8_t mavlink_msg_debug_transmit_get_heating_reverse(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  22);
}

/**
 * @brief Get field flag_mask from debug_transmit message
 *
 * @return   different flags
 */
static inline uint16_t mavlink_msg_debug_transmit_get_flag_mask(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  16);
}

/**
 * @brief Get field fw_version from debug_transmit message
 *
 * @return  Firmware version
 */
static inline uint32_t mavlink_msg_debug_transmit_get_fw_version(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  4);
}

/**
 * @brief Decode a debug_transmit message into a struct
 *
 * @param msg The message to decode
 * @param debug_transmit C-struct to decode the message contents into
 */
static inline void mavlink_msg_debug_transmit_decode(const mavlink_message_t* msg, mavlink_debug_transmit_t* debug_transmit)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    debug_transmit->temparature = mavlink_msg_debug_transmit_get_temparature(msg);
    debug_transmit->fw_version = mavlink_msg_debug_transmit_get_fw_version(msg);
    debug_transmit->stoper_position = mavlink_msg_debug_transmit_get_stoper_position(msg);
    debug_transmit->pusher_position = mavlink_msg_debug_transmit_get_pusher_position(msg);
    debug_transmit->min_stopper = mavlink_msg_debug_transmit_get_min_stopper(msg);
    debug_transmit->max_stopper = mavlink_msg_debug_transmit_get_max_stopper(msg);
    debug_transmit->flag_mask = mavlink_msg_debug_transmit_get_flag_mask(msg);
    debug_transmit->fixator_switch_state = mavlink_msg_debug_transmit_get_fixator_switch_state(msg);
    debug_transmit->counter_switch_state = mavlink_msg_debug_transmit_get_counter_switch_state(msg);
    debug_transmit->start_switch_state = mavlink_msg_debug_transmit_get_start_switch_state(msg);
    debug_transmit->end_switch_state = mavlink_msg_debug_transmit_get_end_switch_state(msg);
    debug_transmit->heating_reverse = mavlink_msg_debug_transmit_get_heating_reverse(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN? msg->len : MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN;
        memset(debug_transmit, 0, MAVLINK_MSG_ID_DEBUG_TRANSMIT_LEN);
    memcpy(debug_transmit, _MAV_PAYLOAD(msg), len);
#endif
}
