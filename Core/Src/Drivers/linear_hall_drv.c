/*
 * adc_drv.c
 *
 *  Created on: 10 лип. 2026 р.
 *      Author: Admin
 */


#include <main.h>
#include <adc.h>
#include "voltage_drv.h"
#include "linear_hall_drv.h"

static uint8_t adc_ready = 0;
static uint16_t adc_buffer[ADC_CHANNELS];
static uint16_t CALIBRATION_DATA[ADC_CHANNELS][2];
static uint16_t current_threshold[ADC_CHANNELS];

static uint16_t hall_start = 2048;
static uint16_t hall_finish = 2048;


void adc_init()
{
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, ADC_CHANNELS);
	CALIBRATION_DATA[ADC_START_SWITCH][VOLT_5] = 400;
	CALIBRATION_DATA[ADC_START_SWITCH][VOLT_12] = 220;
	CALIBRATION_DATA[ADC_FINISH_SWITCH][VOLT_5] = 400;
	CALIBRATION_DATA[ADC_FINISH_SWITCH][VOLT_12] = 220;

	float raw_threshold

	current_threshold[ADC_START_SWITCH] = CALIBRATION_DATA[ADC_START_SWITCH][VOLT_12] +
			(CALIBRATION_DATA[ADC_START_SWITCH][VOLT_5] - CALIBRATION_DATA[ADC_START_SWITCH][VOLT_12]) / 2.4f *
			(voltage_drv_get() - 5.0f);

	current_threshold[ADC_FINISH_SWITCH] = CALIBRATION_DATA[ADC_FINISH_SWITCH][VOLT_12] +
				(CALIBRATION_DATA[ADC_FINISH_SWITCH][VOLT_5] - CALIBRATION_DATA[ADC_FINISH_SWITCH][VOLT_12]) / 2.4f *
				(voltage_drv_get() - 5.0f);

}

calculate

void adc_conv_callback(ADC_HandleTypeDef* hadc)
{
	adc_ready = 1;
}

void hall_adc_apply_filter(uint16_t new_sample, uint16_t* filtered_out)
{
	uint8_t length_of_filter = 32;
	static uint32_t sum_of_filter = 2048 * length_of_filter;

	sum_of_filter = (sum_of_filter - sum_of_filter / length_of_filter) + new_sample;

	&filtered_out = sum_of_filter / length_of_filter

}

uint8_t hall_adc_detect(uint16_t hall_measure)
{
	if ()
}

void hall_adc_tick(void)
{
	if(adc_ready)
	{

	}
}
