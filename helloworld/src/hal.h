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

#define GPIO_SIZE 			0x1000 // s. data sheet
#define GPIO_BANK_0 		0x44E07000
#define GPIO_BANK_1 		0x4804C000
#define GPIO_BANK_2 		0x481AC000
#define GPIO_BANK_3 		0x481AE000
#define GPIO_CLEARDATAOUT 	0x190
#define GPIO_SETDATAOUT 	0x194

/* Interrupt numbers  (spruh73l.pdf S.465 ff.) */
#define INTR_GPIO_PORT0 97
#define INTR_GPIO_PORT1 99
#define INTR_GPIO_PORT2 33

/* GPIO port addresses (spruh73l.pdf S.177 ff.) */
#define GPIO_PORT0 0x44E07000
#define GPIO_PORT1 0x4804C000
#define GPIO_PORT2 0x481AC000

/* GPIO port registers length */
#define GPIO_REGISTER_LENGTH 0x1000

/* GPIO register offsets (spruh73l.pdf S.4877) */
#define GPIO_LEVELDETECT0 0x140
#define GPIO_LEVELDETECT1 0x144
#define GPIO_RISINGDETECT 0x148
#define GPIO_FALLINGDETECT 0x14C

#define GPIO_IRQSTATUS_0 0x2C
#define GPIO_IRQSTATUS_1 0x30
#define GPIO_IRQSTATUS_SET_0 0x34
#define GPIO_IRQSTATUS_SET_1 0x38

#define GPIO_DATAIN 	0x138
#define GPIO_SETDATAOUT 0x194

/* Actuators pin mapping */
#define LAMP_RED_PIN 	16
#define LAMP_YELLOW_PIN 17
#define LAMP_GREEN_PIN 	18
#define MOTOR_RIGHT_PIN 12
#define MOTOR_LEFT_PIN 	13
#define MOTOR_SLOW_PIN 	13
#define MOTOR_STOP_PIN	15

/* Sensors pin mapping (Aufgabenbeschreibung o. Schaltplan) */
#define LIGHTBARRIER_FRONT_PIN 	2	//active low
#define BUTTON_START_PIN 		22	//active high


class HAL {
public:
	HAL();
	virtual ~HAL();
	void greenLampOn();
	void greenLampOff();
private:
	uintptr_t gpio_bank_1;
};
