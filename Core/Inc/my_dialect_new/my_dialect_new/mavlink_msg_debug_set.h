#pragma once
// MESSAGE DEBUG_SET PACKING

#include <stdint.h>

#define MAVLINK_MSG_ID_DEBUG_SET 102


typedef struct __mavlink_debug_set_t {
 float data; /*<   Data to set*/
 uint8_t param_id; /*<   Id of parameter to set*/
 uint8_t confirmation; /*<   Sequential number of target unit*/
} mavlink_debug_set_t;

#define MAVLINK_MSG_ID_DEBUG_SET_LEN 6
#define MAVLINK_MSG_ID_DEBUG_SET_MIN_LEN 6
#define MAVLINK_MSG_ID_102_LEN 6
#define MAVLINK_MSG_ID_102_MIN_LEN 6

#define MAVLINK_MSG_ID_DEBUG_SET_CRC 134
#define MAVLINK_MSG_ID_102_CRC 134



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_DEBUG_SET { \
    102, \
    "DEBUG_SET", \
    3, \
    {  { "param_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_debug_set_t, param_id) }, \
         { "data", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_debug_set_t, data) }, \
         { "confirmation", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_debug_set_t, confirmation) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_DEBUG_SET { \
    "DEBUG_SET", \
    3, \
    {  { "param_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_debug_set_t, param_id) }, \
         { "data", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_debug_set_t, data) }, \
         { "confirmation", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_debug_set_t, confirmation) }, \
         } \
}
#endif

/**
 * @brief Pack a debug_set message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param param_id   Id of parameter to set
 * @param data   Data to set
 * @param confirmation   Sequential number of target unit
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_debug_set_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t param_id, float data, uint8_t confirmation)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEBUG_SET_LEN];
    _mav_put_float(buf, 0, data);
    _mav_put_uint8_t(buf, 4, param_id);
    _mav_put_uint8_t(buf, 5, confirmation);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEBUG_SET_LEN);
#else
    mavlink_debug_set_t packet;
    packet.data = data;
    packet.param_id = param_id;
    packet.confirmation = confirmation;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEBUG_SET_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEBUG_SET;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_DEBUG_SET_MIN_LEN, MAVLINK_MSG_ID_DEBUG_SET_LEN, MAVLINK_MSG_ID_DEBUG_SET_CRC);
}

/**
 * @brief Pack a debug_set message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param param_id   Id of parameter to set
 * @param data   Data to set
 * @param confirmation   Sequential number of target unit
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_debug_set_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t param_id, float data, uint8_t confirmation)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEBUG_SET_LEN];
    _mav_put_float(buf, 0, data);
    _mav_put_uint8_t(buf, 4, param_id);
    _mav_put_uint8_t(buf, 5, confirmation);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEBUG_SET_LEN);
#else
    mavlink_debug_set_t packet;
    packet.data = data;
    packet.param_id = param_id;
    packet.confirmation = confirmation;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEBUG_SET_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEBUG_SET;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_DEBUG_SET_MIN_LEN, MAVLINK_MSG_ID_DEBUG_SET_LEN, MAVLINK_MSG_ID_DEBUG_SET_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_DEBUG_SET_MIN_LEN, MAVLINK_MSG_ID_DEBUG_SET_LEN);
#endif
}

/**
 * @brief Pack a debug_set message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param param_id   Id of parameter to set
 * @param data   Data to set
 * @param confirmation   Sequential number of target unit
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_debug_set_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t param_id,float data,uint8_t confirmation)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEBUG_SET_LEN];
    _mav_put_float(buf, 0, data);
    _mav_put_uint8_t(buf, 4, param_id);
    _mav_put_uint8_t(buf, 5, confirmation);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEBUG_SET_LEN);
#else
    mavlink_debug_set_t packet;
    packet.data = data;
    packet.param_id = param_id;
    packet.confirmation = confirmation;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEBUG_SET_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEBUG_SET;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_DEBUG_SET_MIN_LEN, MAVLINK_MSG_ID_DEBUG_SET_LEN, MAVLINK_MSG_ID_DEBUG_SET_CRC);
}

/**
 * @brief Encode a debug_set struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param debug_set C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_debug_set_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_debug_set_t* debug_set)
{
    return mavlink_msg_debug_set_pack(system_id, component_id, msg, debug_set->param_id, debug_set->data, debug_set->confirmation);
}

/**
 * @brief Encode a debug_set struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param debug_set C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_debug_set_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_debug_set_t* debug_set)
{
    return mavlink_msg_debug_set_pack_chan(system_id, component_id, chan, msg, debug_set->param_id, debug_set->data, debug_set->confirmation);
}

/**
 * @brief Encode a debug_set struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param debug_set C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_debug_set_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_debug_set_t* debug_set)
{
    return mavlink_msg_debug_set_pack_status(system_id, component_id, _status, msg,  debug_set->param_id, debug_set->data, debug_set->confirmation);
}

/**
 * @brief Send a debug_set message
 * @param chan MAVLink channel to send the message
 *
 * @param param_id   Id of parameter to set
 * @param data   Data to set
 * @param confirmation   Sequential number of target unit
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_debug_set_send(mavlink_channel_t chan, uint8_t param_id, float data, uint8_t confirmation)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEBUG_SET_LEN];
    _mav_put_float(buf, 0, data);
    _mav_put_uint8_t(buf, 4, param_id);
    _mav_put_uint8_t(buf, 5, confirmation);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_SET, buf, MAVLINK_MSG_ID_DEBUG_SET_MIN_LEN, MAVLINK_MSG_ID_DEBUG_SET_LEN, MAVLINK_MSG_ID_DEBUG_SET_CRC);
#else
    mavlink_debug_set_t packet;
    packet.data = data;
    packet.param_id = param_id;
    packet.confirmation = confirmation;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_SET, (const char *)&packet, MAVLINK_MSG_ID_DEBUG_SET_MIN_LEN, MAVLINK_MSG_ID_DEBUG_SET_LEN, MAVLINK_MSG_ID_DEBUG_SET_CRC);
#endif
}

/**
 * @brief Send a debug_set message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_debug_set_send_struct(mavlink_channel_t chan, const mavlink_debug_set_t* debug_set)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_debug_set_send(chan, debug_set->param_id, debug_set->data, debug_set->confirmation);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_SET, (const char *)debug_set, MAVLINK_MSG_ID_DEBUG_SET_MIN_LEN, MAVLINK_MSG_ID_DEBUG_SET_LEN, MAVLINK_MSG_ID_DEBUG_SET_CRC);
#endif
}

#if MAVLINK_MSG_ID_DEBUG_SET_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_debug_set_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t param_id, float data, uint8_t confirmation)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, data);
    _mav_put_uint8_t(buf, 4, param_id);
    _mav_put_uint8_t(buf, 5, confirmation);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_SET, buf, MAVLINK_MSG_ID_DEBUG_SET_MIN_LEN, MAVLINK_MSG_ID_DEBUG_SET_LEN, MAVLINK_MSG_ID_DEBUG_SET_CRC);
#else
    mavlink_debug_set_t *packet = (mavlink_debug_set_t *)msgbuf;
    packet->data = data;
    packet->param_id = param_id;
    packet->confirmation = confirmation;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_SET, (const char *)packet, MAVLINK_MSG_ID_DEBUG_SET_MIN_LEN, MAVLINK_MSG_ID_DEBUG_SET_LEN, MAVLINK_MSG_ID_DEBUG_SET_CRC);
#endif
}
#endif

#endif

// MESSAGE DEBUG_SET UNPACKING


/**
 * @brief Get field param_id from debug_set message
 *
 * @return   Id of parameter to set
 */
static inline uint8_t mavlink_msg_debug_set_get_param_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field data from debug_set message
 *
 * @return   Data to set
 */
static inline float mavlink_msg_debug_set_get_data(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field confirmation from debug_set message
 *
 * @return   Sequential number of target unit
 */
static inline uint8_t mavlink_msg_debug_set_get_confirmation(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  5);
}

/**
 * @brief Decode a debug_set message into a struct
 *
 * @param msg The message to decode
 * @param debug_set C-struct to decode the message contents into
 */
static inline void mavlink_msg_debug_set_decode(const mavlink_message_t* msg, mavlink_debug_set_t* debug_set)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    debug_set->data = mavlink_msg_debug_set_get_data(msg);
    debug_set->param_id = mavlink_msg_debug_set_get_param_id(msg);
    debug_set->confirmation = mavlink_msg_debug_set_get_confirmation(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_DEBUG_SET_LEN? msg->len : MAVLINK_MSG_ID_DEBUG_SET_LEN;
        memset(debug_set, 0, MAVLINK_MSG_ID_DEBUG_SET_LEN);
    memcpy(debug_set, _MAV_PAYLOAD(msg), len);
#endif
}
