#pragma once
// MESSAGE DEBUG_DO PACKING

#include <stdint.h>

#define MAVLINK_MSG_ID_DEBUG_DO 103


typedef struct __mavlink_debug_do_t {
 uint8_t command_id; /*<   Id of parameter to set*/
 uint8_t confirmation; /*<   Sequential number of target unit*/
} mavlink_debug_do_t;

#define MAVLINK_MSG_ID_DEBUG_DO_LEN 2
#define MAVLINK_MSG_ID_DEBUG_DO_MIN_LEN 2
#define MAVLINK_MSG_ID_103_LEN 2
#define MAVLINK_MSG_ID_103_MIN_LEN 2

#define MAVLINK_MSG_ID_DEBUG_DO_CRC 81
#define MAVLINK_MSG_ID_103_CRC 81



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_DEBUG_DO { \
    103, \
    "DEBUG_DO", \
    2, \
    {  { "command_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_debug_do_t, command_id) }, \
         { "confirmation", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_debug_do_t, confirmation) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_DEBUG_DO { \
    "DEBUG_DO", \
    2, \
    {  { "command_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_debug_do_t, command_id) }, \
         { "confirmation", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_debug_do_t, confirmation) }, \
         } \
}
#endif

/**
 * @brief Pack a debug_do message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param command_id   Id of parameter to set
 * @param confirmation   Sequential number of target unit
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_debug_do_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t command_id, uint8_t confirmation)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEBUG_DO_LEN];
    _mav_put_uint8_t(buf, 0, command_id);
    _mav_put_uint8_t(buf, 1, confirmation);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEBUG_DO_LEN);
#else
    mavlink_debug_do_t packet;
    packet.command_id = command_id;
    packet.confirmation = confirmation;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEBUG_DO_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEBUG_DO;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_DEBUG_DO_MIN_LEN, MAVLINK_MSG_ID_DEBUG_DO_LEN, MAVLINK_MSG_ID_DEBUG_DO_CRC);
}

/**
 * @brief Pack a debug_do message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param command_id   Id of parameter to set
 * @param confirmation   Sequential number of target unit
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_debug_do_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t command_id, uint8_t confirmation)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEBUG_DO_LEN];
    _mav_put_uint8_t(buf, 0, command_id);
    _mav_put_uint8_t(buf, 1, confirmation);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEBUG_DO_LEN);
#else
    mavlink_debug_do_t packet;
    packet.command_id = command_id;
    packet.confirmation = confirmation;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEBUG_DO_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEBUG_DO;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_DEBUG_DO_MIN_LEN, MAVLINK_MSG_ID_DEBUG_DO_LEN, MAVLINK_MSG_ID_DEBUG_DO_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_DEBUG_DO_MIN_LEN, MAVLINK_MSG_ID_DEBUG_DO_LEN);
#endif
}

/**
 * @brief Pack a debug_do message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param command_id   Id of parameter to set
 * @param confirmation   Sequential number of target unit
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_debug_do_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t command_id,uint8_t confirmation)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEBUG_DO_LEN];
    _mav_put_uint8_t(buf, 0, command_id);
    _mav_put_uint8_t(buf, 1, confirmation);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEBUG_DO_LEN);
#else
    mavlink_debug_do_t packet;
    packet.command_id = command_id;
    packet.confirmation = confirmation;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEBUG_DO_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEBUG_DO;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_DEBUG_DO_MIN_LEN, MAVLINK_MSG_ID_DEBUG_DO_LEN, MAVLINK_MSG_ID_DEBUG_DO_CRC);
}

/**
 * @brief Encode a debug_do struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param debug_do C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_debug_do_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_debug_do_t* debug_do)
{
    return mavlink_msg_debug_do_pack(system_id, component_id, msg, debug_do->command_id, debug_do->confirmation);
}

/**
 * @brief Encode a debug_do struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param debug_do C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_debug_do_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_debug_do_t* debug_do)
{
    return mavlink_msg_debug_do_pack_chan(system_id, component_id, chan, msg, debug_do->command_id, debug_do->confirmation);
}

/**
 * @brief Encode a debug_do struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param debug_do C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_debug_do_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_debug_do_t* debug_do)
{
    return mavlink_msg_debug_do_pack_status(system_id, component_id, _status, msg,  debug_do->command_id, debug_do->confirmation);
}

/**
 * @brief Send a debug_do message
 * @param chan MAVLink channel to send the message
 *
 * @param command_id   Id of parameter to set
 * @param confirmation   Sequential number of target unit
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_debug_do_send(mavlink_channel_t chan, uint8_t command_id, uint8_t confirmation)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEBUG_DO_LEN];
    _mav_put_uint8_t(buf, 0, command_id);
    _mav_put_uint8_t(buf, 1, confirmation);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_DO, buf, MAVLINK_MSG_ID_DEBUG_DO_MIN_LEN, MAVLINK_MSG_ID_DEBUG_DO_LEN, MAVLINK_MSG_ID_DEBUG_DO_CRC);
#else
    mavlink_debug_do_t packet;
    packet.command_id = command_id;
    packet.confirmation = confirmation;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_DO, (const char *)&packet, MAVLINK_MSG_ID_DEBUG_DO_MIN_LEN, MAVLINK_MSG_ID_DEBUG_DO_LEN, MAVLINK_MSG_ID_DEBUG_DO_CRC);
#endif
}

/**
 * @brief Send a debug_do message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_debug_do_send_struct(mavlink_channel_t chan, const mavlink_debug_do_t* debug_do)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_debug_do_send(chan, debug_do->command_id, debug_do->confirmation);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_DO, (const char *)debug_do, MAVLINK_MSG_ID_DEBUG_DO_MIN_LEN, MAVLINK_MSG_ID_DEBUG_DO_LEN, MAVLINK_MSG_ID_DEBUG_DO_CRC);
#endif
}

#if MAVLINK_MSG_ID_DEBUG_DO_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_debug_do_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t command_id, uint8_t confirmation)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, command_id);
    _mav_put_uint8_t(buf, 1, confirmation);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_DO, buf, MAVLINK_MSG_ID_DEBUG_DO_MIN_LEN, MAVLINK_MSG_ID_DEBUG_DO_LEN, MAVLINK_MSG_ID_DEBUG_DO_CRC);
#else
    mavlink_debug_do_t *packet = (mavlink_debug_do_t *)msgbuf;
    packet->command_id = command_id;
    packet->confirmation = confirmation;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_DO, (const char *)packet, MAVLINK_MSG_ID_DEBUG_DO_MIN_LEN, MAVLINK_MSG_ID_DEBUG_DO_LEN, MAVLINK_MSG_ID_DEBUG_DO_CRC);
#endif
}
#endif

#endif

// MESSAGE DEBUG_DO UNPACKING


/**
 * @brief Get field command_id from debug_do message
 *
 * @return   Id of parameter to set
 */
static inline uint8_t mavlink_msg_debug_do_get_command_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field confirmation from debug_do message
 *
 * @return   Sequential number of target unit
 */
static inline uint8_t mavlink_msg_debug_do_get_confirmation(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Decode a debug_do message into a struct
 *
 * @param msg The message to decode
 * @param debug_do C-struct to decode the message contents into
 */
static inline void mavlink_msg_debug_do_decode(const mavlink_message_t* msg, mavlink_debug_do_t* debug_do)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    debug_do->command_id = mavlink_msg_debug_do_get_command_id(msg);
    debug_do->confirmation = mavlink_msg_debug_do_get_confirmation(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_DEBUG_DO_LEN? msg->len : MAVLINK_MSG_ID_DEBUG_DO_LEN;
        memset(debug_do, 0, MAVLINK_MSG_ID_DEBUG_DO_LEN);
    memcpy(debug_do, _MAV_PAYLOAD(msg), len);
#endif
}
