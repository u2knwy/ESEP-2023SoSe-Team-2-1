/*
 * hal.h
 *
 *  Created on: 31.03.2023
 *      Author: Maik
 */

#pragma once

/* Für die Hardware: */
#include <sys/mman.h>
#include <hw/inout.h>

#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/procmgr.h>
#include <thread>
#include <chrono>

// For ADC
#include "adc/ADC.h"
#define ADC_BASE 0x44E0D000
#define ADC_LENGTH 0x2000

/* Code configuration */
#define DEMO true         // true for demo time, false to spin forever.
#define DEMO_DURATION 120 // Duration of demo time in minutes.
/* Varialbes */
//bool receivingRunning = false;

/* TSC_ADC register offsets (spruh73l.pdf S.1747) */
#define ADC_IRQ_ENABLE_SET 0x2c
#define ADC_IRQ_ENABLE_CLR 0x30
#define ADC_IRQ_STATUS 0x28
#define ADC_CTRL 0x40
#define ADC_DATA 0x100

/* ADC irq pin mask */
#define ADC_IRQ_PIN_MASK 0x2

/* My pulse codes */
#define PULSE_STOP_THREAD _PULSE_CODE_MINAVAIL + 1
#define PULSE_ADC_SAMPLING_DONE _PULSE_CODE_MINAVAIL + 2

#define SIZE 				0x1000 // s. data sheet
#define GPIO_BANK_1 		0x4804C000 // spruh73l.pdf page 4877
#define GPIO_CLEARDATAOUT 	0x190
#define GPIO_SETDATAOUT 	0x194

#define LAMP_RED_PIN 		(1 << 16)
#define LAMP_YELLOW_PIN 	(1 << 17)
#define LAMP_GREEN_PIN 		(1 << 18)
#define MOTOR_RIGHT_PIN		(1 << 12)
#define MOTOR_LEFT_PIN		(1 << 13)
#define MOTOR_SLOW_PIN		(1 << 14)
#define MOTOR_STOP_PIN		(1 << 15)


class HAL {
public:
	HAL();
	virtual ~HAL();
	void GreenLampOn();
	void GreenLampOff();
	void measureHeight();
	void heightReceivingRoutine(int channelID);
	void motorSlow();
	void motorFast();
	void motorStop();
private:
	uintptr_t gpio_bank_1;
	ADC* adc;
	bool receivingRunning{false};
};
