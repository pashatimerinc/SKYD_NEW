/*
 * flasher.h
 *
 *  Created on: 27 трав. 2026 р.
 *      Author: Admin
 */

#ifndef SRC_APP_FLASHER_H_
#define SRC_APP_FLASHER_H_

void check_and_jump_bootloader(void);
void jump_to_bootloader(void);
void request_bootloader(void);
void restart_controller(void);

#endif /* SRC_APP_FLASHER_H_ */
