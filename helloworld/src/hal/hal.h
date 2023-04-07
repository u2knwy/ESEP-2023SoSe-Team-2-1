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

#define GPIO_SIZE 				0x1000 // s. data sheet
#define GPIO_BANK_1 		0x4804C000 // spruh73l.pdf page 4877
#define GPIO_CLEARDATAOUT 	0x190
#define GPIO_SETDATAOUT 	0x194

#define LAMP_RED_PIN 	(1 << 16)
#define LAMP_YELLOW_PIN (1 << 17)
#define LAMP_GREEN_PIN 	(1 << 18)


class HAL {
public:
	HAL();
	virtual ~HAL();
	void GreenLampOn();
	void GreenLampOff();
private:
	uintptr_t gpio_bank_1;
};
