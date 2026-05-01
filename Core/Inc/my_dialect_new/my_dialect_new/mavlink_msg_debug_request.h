#pragma once
// MESSAGE DEBUG_REQUEST PACKING

#include <stdint.h>

#define MAVLINK_MSG_ID_DEBUG_REQUEST 100


typedef struct __mavlink_debug_request_t {
 uint8_t request; /*<  Request for debug procedure - 200 for start*/
} mavlink_debug_request_t;

#define MAVLINK_MSG_ID_DEBUG_REQUEST_LEN 1
#define MAVLINK_MSG_ID_DEBUG_REQUEST_MIN_LEN 1
#define MAVLINK_MSG_ID_100_LEN 1
#define MAVLINK_MSG_ID_100_MIN_LEN 1

#define MAVLINK_MSG_ID_DEBUG_REQUEST_CRC 60
#define MAVLINK_MSG_ID_100_CRC 60



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_DEBUG_REQUEST { \
    100, \
    "DEBUG_REQUEST", \
    1, \
    {  { "request", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_debug_request_t, request) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_DEBUG_REQUEST { \
    "DEBUG_REQUEST", \
    1, \
    {  { "request", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_debug_request_t, request) }, \
         } \
}
#endif

/**
 * @brief Pack a debug_request message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param request  Request for debug procedure - 200 for start
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_debug_request_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t request)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEBUG_REQUEST_LEN];
    _mav_put_uint8_t(buf, 0, request);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEBUG_REQUEST_LEN);
#else
    mavlink_debug_request_t packet;
    packet.request = request;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEBUG_REQUEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEBUG_REQUEST;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_DEBUG_REQUEST_MIN_LEN, MAVLINK_MSG_ID_DEBUG_REQUEST_LEN, MAVLINK_MSG_ID_DEBUG_REQUEST_CRC);
}

/**
 * @brief Pack a debug_request message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param request  Request for debug procedure - 200 for start
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_debug_request_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint8_t request)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEBUG_REQUEST_LEN];
    _mav_put_uint8_t(buf, 0, request);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEBUG_REQUEST_LEN);
#else
    mavlink_debug_request_t packet;
    packet.request = request;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEBUG_REQUEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEBUG_REQUEST;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_DEBUG_REQUEST_MIN_LEN, MAVLINK_MSG_ID_DEBUG_REQUEST_LEN, MAVLINK_MSG_ID_DEBUG_REQUEST_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_DEBUG_REQUEST_MIN_LEN, MAVLINK_MSG_ID_DEBUG_REQUEST_LEN);
#endif
}

/**
 * @brief Pack a debug_request message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param request  Request for debug procedure - 200 for start
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_debug_request_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t request)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEBUG_REQUEST_LEN];
    _mav_put_uint8_t(buf, 0, request);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEBUG_REQUEST_LEN);
#else
    mavlink_debug_request_t packet;
    packet.request = request;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEBUG_REQUEST_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEBUG_REQUEST;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_DEBUG_REQUEST_MIN_LEN, MAVLINK_MSG_ID_DEBUG_REQUEST_LEN, MAVLINK_MSG_ID_DEBUG_REQUEST_CRC);
}

/**
 * @brief Encode a debug_request struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param debug_request C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_debug_request_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_debug_request_t* debug_request)
{
    return mavlink_msg_debug_request_pack(system_id, component_id, msg, debug_request->request);
}

/**
 * @brief Encode a debug_request struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param debug_request C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_debug_request_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_debug_request_t* debug_request)
{
    return mavlink_msg_debug_request_pack_chan(system_id, component_id, chan, msg, debug_request->request);
}

/**
 * @brief Encode a debug_request struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param debug_request C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_debug_request_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_debug_request_t* debug_request)
{
    return mavlink_msg_debug_request_pack_status(system_id, component_id, _status, msg,  debug_request->request);
}

/**
 * @brief Send a debug_request message
 * @param chan MAVLink channel to send the message
 *
 * @param request  Request for debug procedure - 200 for start
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_debug_request_send(mavlink_channel_t chan, uint8_t request)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEBUG_REQUEST_LEN];
    _mav_put_uint8_t(buf, 0, request);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_REQUEST, buf, MAVLINK_MSG_ID_DEBUG_REQUEST_MIN_LEN, MAVLINK_MSG_ID_DEBUG_REQUEST_LEN, MAVLINK_MSG_ID_DEBUG_REQUEST_CRC);
#else
    mavlink_debug_request_t packet;
    packet.request = request;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_REQUEST, (const char *)&packet, MAVLINK_MSG_ID_DEBUG_REQUEST_MIN_LEN, MAVLINK_MSG_ID_DEBUG_REQUEST_LEN, MAVLINK_MSG_ID_DEBUG_REQUEST_CRC);
#endif
}

/**
 * @brief Send a debug_request message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_debug_request_send_struct(mavlink_channel_t chan, const mavlink_debug_request_t* debug_request)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_debug_request_send(chan, debug_request->request);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_REQUEST, (const char *)debug_request, MAVLINK_MSG_ID_DEBUG_REQUEST_MIN_LEN, MAVLINK_MSG_ID_DEBUG_REQUEST_LEN, MAVLINK_MSG_ID_DEBUG_REQUEST_CRC);
#endif
}

#if MAVLINK_MSG_ID_DEBUG_REQUEST_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_debug_request_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t request)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, request);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_REQUEST, buf, MAVLINK_MSG_ID_DEBUG_REQUEST_MIN_LEN, MAVLINK_MSG_ID_DEBUG_REQUEST_LEN, MAVLINK_MSG_ID_DEBUG_REQUEST_CRC);
#else
    mavlink_debug_request_t *packet = (mavlink_debug_request_t *)msgbuf;
    packet->request = request;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEBUG_REQUEST, (const char *)packet, MAVLINK_MSG_ID_DEBUG_REQUEST_MIN_LEN, MAVLINK_MSG_ID_DEBUG_REQUEST_LEN, MAVLINK_MSG_ID_DEBUG_REQUEST_CRC);
#endif
}
#endif

#endif

// MESSAGE DEBUG_REQUEST UNPACKING


/**
 * @brief Get field request from debug_request message
 *
 * @return  Request for debug procedure - 200 for start
 */
static inline uint8_t mavlink_msg_debug_request_get_request(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a debug_request message into a struct
 *
 * @param msg The message to decode
 * @param debug_request C-struct to decode the message contents into
 */
static inline void mavlink_msg_debug_request_decode(const mavlink_message_t* msg, mavlink_debug_request_t* debug_request)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    debug_request->request = mavlink_msg_debug_request_get_request(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_DEBUG_REQUEST_LEN? msg->len : MAVLINK_MSG_ID_DEBUG_REQUEST_LEN;
        memset(debug_request, 0, MAVLINK_MSG_ID_DEBUG_REQUEST_LEN);
    memcpy(debug_request, _MAV_PAYLOAD(msg), len);
#endif
}
