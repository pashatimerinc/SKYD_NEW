/*
 * temparature.c
 *
 *  Created on: Dec 3, 2025
 *      Author: Admin
 */

#include "stm32f1xx_hal.h"
#include "temparature.h"

extern UART_HandleTypeDef huart3;

// --- Reset pulse using 9600 baud ---
uint8_t OneWire_SendAndReceive(uint8_t tx) {
    uint8_t rx = 0;
    HAL_UART_Transmit(&huart3, &tx, 1, HAL_MAX_DELAY);
    HAL_UART_Receive(&huart3, &rx, 1, HAL_MAX_DELAY);
    return rx;
}


uint8_t OneWire_Reset(void) {
    uint8_t rx;
    huart3.Init.BaudRate = 9600;
    HAL_HalfDuplex_Init(&huart3);

    rx = OneWire_SendAndReceive(0xF0);

    huart3.Init.BaudRate = 115200;
    HAL_HalfDuplex_Init(&huart3);

    return (rx != 0xF0); // presence detected
}


// --- Write one bit ---
void OneWire_WriteBit(uint8_t bit) {
    uint8_t tx = bit ? 0xFF : 0x00;
    HAL_UART_Transmit(&huart3, &tx, 1, HAL_MAX_DELAY);
}

// --- Read one bit ---
uint8_t OneWire_ReadBit(void) {
    uint8_t rx = OneWire_SendAndReceive(0xFF);
    return (rx == 0xFF) ? 1 : 0;
}


// --- Write one byte ---
void OneWire_WriteByte(uint8_t data) {
    for (int i = 0; i < 8; i++) {
        OneWire_WriteBit(data & 0x01);
        data >>= 1;
    }
}

// --- Read one byte ---
uint8_t OneWire_ReadByte(void) {
    uint8_t value = 0;
    for (int i = 0; i < 8; i++) {
        if (OneWire_ReadBit()) value |= (1 << i);
    }
    return value;
}

// --- Read temperature ---
float DS18B20_ReadTemp(void) {
    uint8_t temp_lsb, temp_msb;
    int16_t raw;

    if (!OneWire_Reset()) return -1000.0f; // error: no sensor

    OneWire_WriteByte(0xCC); // Skip ROM
    OneWire_WriteByte(0x4E); // Convert T


    OneWire_Reset();
    OneWire_WriteByte(0xCC); // Skip ROM
    OneWire_WriteByte(0xBE); // Read Scratchpad
    temp_lsb = OneWire_ReadByte();
    temp_msb = OneWire_ReadByte();

    raw = (temp_msb << 8) | temp_lsb;
    return (float)raw / 16.0f; // °C
}

