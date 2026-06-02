/*
 * fw_flags.c
 *
 *  Created on: 28 трав. 2026 р.
 *      Author: Admin
 */


#include <stdint.h>

// This gets placed at 0x0801FC00 by the linker
const uint32_t firmware_valid_flag
    __attribute__((section(".fw_flag")))
    __attribute__((used))
    = 0xCAFECAFE;
