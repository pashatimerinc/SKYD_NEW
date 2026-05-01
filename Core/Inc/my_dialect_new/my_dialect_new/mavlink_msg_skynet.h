#pragma once
// MESSAGE SKYNET PACKING

#include <stdint.h>

#define MAVLINK_MSG_ID_SKYNET 1488


typedef struct __mavlink_skynet_t {
 float sequential_num; /*<   */
 float target_unit; /*<   */
 float command; /*<   */
 float bitmask; /*<   */
} mavlink_skynet_t;

#define MAVLINK_MSG_ID_SKYNET_LEN 16
#define MAVLINK_MSG_ID_SKYNET_MIN_LEN 16
#define MAVLINK_MSG_ID_1488_LEN 16
#define MAVLINK_MSG_ID_1488_MIN_LEN 16

#define MAVLINK_MSG_ID_SKYNET_CRC 172
#define MAVLINK_MSG_ID_1488_CRC 172



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_SKYNET { \
    1488, \
    "SKYNET", \
    4, \
    {  { "sequential_num", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_skynet_t, sequential_num) }, \
         { "target_unit", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_skynet_t, target_unit) }, \
         { "command", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_skynet_t, command) }, \
         { "bitmask", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_skynet_t, bitmask) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_SKYNET { \
    "SKYNET", \
    4, \
    {  { "sequential_num", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_skynet_t, sequential_num) }, \
         { "target_unit", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_skynet_t, target_unit) }, \
         { "command", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_skynet_t, command) }, \
         { "bitmask", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_skynet_t, bitmask) }, \
         } \
}
#endif

/**
 * @brief Pack a skynet message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param sequential_num   
 * @param target_unit   
 * @param command   
 * @param bitmask   
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_skynet_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float sequential_num, float target_unit, float command, float bitmask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SKYNET_LEN];
    _mav_put_float(buf, 0, sequential_num);
    _mav_put_float(buf, 4, target_unit);
    _mav_put_float(buf, 8, command);
    _mav_put_float(buf, 12, bitmask);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SKYNET_LEN);
#else
    mavlink_skynet_t packet;
    packet.sequential_num = sequential_num;
    packet.target_unit = target_unit;
    packet.command = command;
    packet.bitmask = bitmask;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SKYNET_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SKYNET;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_SKYNET_MIN_LEN, MAVLINK_MSG_ID_SKYNET_LEN, MAVLINK_MSG_ID_SKYNET_CRC);
}

/**
 * @brief Pack a skynet message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param sequential_num   
 * @param target_unit   
 * @param command   
 * @param bitmask   
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_skynet_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               float sequential_num, float target_unit, float command, float bitmask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SKYNET_LEN];
    _mav_put_float(buf, 0, sequential_num);
    _mav_put_float(buf, 4, target_unit);
    _mav_put_float(buf, 8, command);
    _mav_put_float(buf, 12, bitmask);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SKYNET_LEN);
#else
    mavlink_skynet_t packet;
    packet.sequential_num = sequential_num;
    packet.target_unit = target_unit;
    packet.command = command;
    packet.bitmask = bitmask;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SKYNET_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SKYNET;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SKYNET_MIN_LEN, MAVLINK_MSG_ID_SKYNET_LEN, MAVLINK_MSG_ID_SKYNET_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_SKYNET_MIN_LEN, MAVLINK_MSG_ID_SKYNET_LEN);
#endif
}

/**
 * @brief Pack a skynet message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param sequential_num   
 * @param target_unit   
 * @param command   
 * @param bitmask   
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_skynet_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float sequential_num,float target_unit,float command,float bitmask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SKYNET_LEN];
    _mav_put_float(buf, 0, sequential_num);
    _mav_put_float(buf, 4, target_unit);
    _mav_put_float(buf, 8, command);
    _mav_put_float(buf, 12, bitmask);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_SKYNET_LEN);
#else
    mavlink_skynet_t packet;
    packet.sequential_num = sequential_num;
    packet.target_unit = target_unit;
    packet.command = command;
    packet.bitmask = bitmask;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_SKYNET_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_SKYNET;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_SKYNET_MIN_LEN, MAVLINK_MSG_ID_SKYNET_LEN, MAVLINK_MSG_ID_SKYNET_CRC);
}

/**
 * @brief Encode a skynet struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param skynet C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_skynet_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_skynet_t* skynet)
{
    return mavlink_msg_skynet_pack(system_id, component_id, msg, skynet->sequential_num, skynet->target_unit, skynet->command, skynet->bitmask);
}

/**
 * @brief Encode a skynet struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param skynet C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_skynet_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_skynet_t* skynet)
{
    return mavlink_msg_skynet_pack_chan(system_id, component_id, chan, msg, skynet->sequential_num, skynet->target_unit, skynet->command, skynet->bitmask);
}

/**
 * @brief Encode a skynet struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param skynet C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_skynet_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_skynet_t* skynet)
{
    return mavlink_msg_skynet_pack_status(system_id, component_id, _status, msg,  skynet->sequential_num, skynet->target_unit, skynet->command, skynet->bitmask);
}

/**
 * @brief Send a skynet message
 * @param chan MAVLink channel to send the message
 *
 * @param sequential_num   
 * @param target_unit   
 * @param command   
 * @param bitmask   
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_skynet_send(mavlink_channel_t chan, float sequential_num, float target_unit, float command, float bitmask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_SKYNET_LEN];
    _mav_put_float(buf, 0, sequential_num);
    _mav_put_float(buf, 4, target_unit);
    _mav_put_float(buf, 8, command);
    _mav_put_float(buf, 12, bitmask);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SKYNET, buf, MAVLINK_MSG_ID_SKYNET_MIN_LEN, MAVLINK_MSG_ID_SKYNET_LEN, MAVLINK_MSG_ID_SKYNET_CRC);
#else
    mavlink_skynet_t packet;
    packet.sequential_num = sequential_num;
    packet.target_unit = target_unit;
    packet.command = command;
    packet.bitmask = bitmask;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SKYNET, (const char *)&packet, MAVLINK_MSG_ID_SKYNET_MIN_LEN, MAVLINK_MSG_ID_SKYNET_LEN, MAVLINK_MSG_ID_SKYNET_CRC);
#endif
}

/**
 * @brief Send a skynet message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_skynet_send_struct(mavlink_channel_t chan, const mavlink_skynet_t* skynet)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_skynet_send(chan, skynet->sequential_num, skynet->target_unit, skynet->command, skynet->bitmask);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SKYNET, (const char *)skynet, MAVLINK_MSG_ID_SKYNET_MIN_LEN, MAVLINK_MSG_ID_SKYNET_LEN, MAVLINK_MSG_ID_SKYNET_CRC);
#endif
}

#if MAVLINK_MSG_ID_SKYNET_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_skynet_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float sequential_num, float target_unit, float command, float bitmask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, sequential_num);
    _mav_put_float(buf, 4, target_unit);
    _mav_put_float(buf, 8, command);
    _mav_put_float(buf, 12, bitmask);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SKYNET, buf, MAVLINK_MSG_ID_SKYNET_MIN_LEN, MAVLINK_MSG_ID_SKYNET_LEN, MAVLINK_MSG_ID_SKYNET_CRC);
#else
    mavlink_skynet_t *packet = (mavlink_skynet_t *)msgbuf;
    packet->sequential_num = sequential_num;
    packet->target_unit = target_unit;
    packet->command = command;
    packet->bitmask = bitmask;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_SKYNET, (const char *)packet, MAVLINK_MSG_ID_SKYNET_MIN_LEN, MAVLINK_MSG_ID_SKYNET_LEN, MAVLINK_MSG_ID_SKYNET_CRC);
#endif
}
#endif

#endif

// MESSAGE SKYNET UNPACKING


/**
 * @brief Get field sequential_num from skynet message
 *
 * @return   
 */
static inline float mavlink_msg_skynet_get_sequential_num(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field target_unit from skynet message
 *
 * @return   
 */
static inline float mavlink_msg_skynet_get_target_unit(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field command from skynet message
 *
 * @return   
 */
static inline float mavlink_msg_skynet_get_command(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field bitmask from skynet message
 *
 * @return   
 */
static inline float mavlink_msg_skynet_get_bitmask(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Decode a skynet message into a struct
 *
 * @param msg The message to decode
 * @param skynet C-struct to decode the message contents into
 */
static inline void mavlink_msg_skynet_decode(const mavlink_message_t* msg, mavlink_skynet_t* skynet)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    skynet->sequential_num = mavlink_msg_skynet_get_sequential_num(msg);
    skynet->target_unit = mavlink_msg_skynet_get_target_unit(msg);
    skynet->command = mavlink_msg_skynet_get_command(msg);
    skynet->bitmask = mavlink_msg_skynet_get_bitmask(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_SKYNET_LEN? msg->len : MAVLINK_MSG_ID_SKYNET_LEN;
        memset(skynet, 0, MAVLINK_MSG_ID_SKYNET_LEN);
    memcpy(skynet, _MAV_PAYLOAD(msg), len);
#endif
}
