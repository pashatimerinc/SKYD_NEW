/*
 * flasher.c
 *
 *  Created on: 27 трав. 2026 р.
 *      Author: Admin
 */

#include <stdint.h>
#include "flasher.h"
#include "stm32f1xx_hal.h"
#include "rtc.h"


#define BOOTLOADER_MAGIC 0xDEAD
#define BOOTLOADER_RTC_REG RTC_BKP_DR1

extern RTC_HandleTypeDef hrtc;


void request_bootloader(void) {
    RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
    PWR->CR      |= PWR_CR_DBP;
    BKP->DR1      = 0xDEAD;
    NVIC_SystemReset();  // reset → custom bootloader takes over
}

void restart_controller(void) {
	BKP->DR1 = 0x0000;
	NVIC_SystemReset();
}



