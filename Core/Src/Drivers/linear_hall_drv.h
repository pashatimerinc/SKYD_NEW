/*
 * adc_drv.h
 *
 *  Created on: 10 лип. 2026 р.
 *      Author: Admin
 */

#ifndef SRC_DRIVERS_LINEAR_HALL_DRV_H_
#define SRC_DRIVERS_LINEAR_HALL_DRV_H_

enum {
	ADC_POWER_IN,
	ADC_START_SWITCH,
	ADC_FINISH_SWITCH,
	ADC_CHANNELS
};

enum {
	VOLT_5,
	VOLT_12,
	VOLTAGE_LEVELS
};




void adc_conv_callback(ADC_HandleTypeDef* hadc);

#endif /* SRC_DRIVERS_LINEAR_HALL_DRV_H_ */
