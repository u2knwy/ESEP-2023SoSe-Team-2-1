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

#include "events/IEventHandler.h"

/* Helper macros */
#define BIT_MASK(x) (1 << (x))

// My pulse codes
#define PULSE_STOP_THREAD 		_PULSE_CODE_MINAVAIL + 1
#define PULSE_ADC_SAMPLING_DONE _PULSE_CODE_MINAVAIL + 2
#define PULSE_INTR_ON_PORT0 	_PULSE_CODE_MINAVAIL + 3

/*---------------------------------------------------------------------------
   GPIO CONFIGURATION
----------------------------------------------------------------------------- */
#define GPIO_SIZE 			0x1000
#define GPIO_BANK_0 		0x44E07000
#define GPIO_BANK_1 		0x4804C000
#define GPIO_BANK_2 		0x481AC000
#define GPIO_BANK_3 		0x481AE000
#define GPIO_CLEARDATAOUT 	0x190
#define GPIO_SETDATAOUT 	0x194
#define GPIO_DATAIN			0x138

/* GPIO0 PINS -> INPUTS */
#define LB_START_PIN		(1 << 2)	// Werkstück im Einlauf = low when true
#define LB_HEIGHT_PIN		(1 << 3)	// Werkstück in Höhenmessung = low when true
#define LB_HEIGHT_OK_PIN	(1 << 4)	// Werkstück Höhe OK = high when true
#define LB_SWITCH_PIN		(1 << 5)	// Werkstück in Weiche = low when true
#define SE_METAL_PIN		(1 << 7)	// Werkstück Metall = high when true
#define SE_SWITCH_PIN		(1 << 14)	// Weiche offen = high when true
#define LB_RAMP_PIN			(1 << 15)	// Rutsche voll = low when true
#define LB_END_PIN			(1 << 20)	// Werkstück im Auslauf = low when true
#define KEY_START_PIN		(1 << 22)	// Taste Start = high, wenn betätigt
#define KEY_STOP_PIN		(1 << 23)	// Taste Stop = low, wenn betätigt
#define KEY_RESET_PIN		(1 << 26)	// Taste Reset = high, wenn betätigt
#define ESTOP_PIN			(1 << 27)	// E-Stop = low, wenn betätigt

/* GPIO1 PINS -> OUTPUTS */
#define MOTOR_RIGHT_PIN		(1 << 12)	// Motor Rechtslauf
#define MOTOR_LEFT_PIN		(1 << 13)	// Motor Linkslauf
#define MOTOR_SLOW_PIN		(1 << 14)	// Motor langsam
#define MOTOR_STOP_PIN		(1 << 15)	// Motor Stop
#define LAMP_RED_PIN 		(1 << 16)	// Rote Lampe an
#define LAMP_YELLOW_PIN 	(1 << 17)	// Gelbe Lampe an
#define LAMP_GREEN_PIN 		(1 << 18)	// Grüne Lampe an
#define SWITCH_PIN			(1 << 19)	// Weiche öffnen

/* GPIO2 PINS -> OUTPUTS */
#define LED_START_PIN 		(1 << 2)	// LED Taste Start
#define LED_RESET_PIN 		(1 << 3)	// LED Taste Reset
#define LED_Q1_PIN 			(1 << 4)	// Signalleuchte Q1
#define LED_Q2_PIN 			(1 << 5)	// Signalleuchte Q2

/* Interrupt numbers  (spruh73l.pdf S.465 ff.) */
#define INTR_GPIO_PORT0 		97
#define INTR_GPIO_PORT1 		99
#define INTR_GPIO_PORT2 		33

/* GPIO register offsets (spruh73l.pdf S.4877) */
#define GPIO_LEVELDETECT0 		0x140
#define GPIO_LEVELDETECT1 		0x144
#define GPIO_RISINGDETECT 		0x148
#define GPIO_FALLINGDETECT 		0x14C

#define GPIO_IRQSTATUS_0 		0x2C
#define GPIO_IRQSTATUS_1 		0x30
#define GPIO_IRQSTATUS_SET_0 	0x34
#define GPIO_IRQSTATUS_SET_1 	0x38

/*---------------------------------------------------------------------------
   ADC CONFIGURATION
----------------------------------------------------------------------------- */
#include "adc/ADC.h"
#define ADC_BASE 0x44E0D000
#define ADC_LENGTH 0x2000

// TSC_ADC register offsets (spruh73l.pdf S.1747)
#define ADC_IRQ_ENABLE_SET 0x2c
#define ADC_IRQ_ENABLE_CLR 0x30
#define ADC_IRQ_STATUS 0x28
#define ADC_CTRL 0x40
#define ADC_DATA 0x100

// ADC IRQ pin mask
#define ADC_IRQ_PIN_MASK 0x2

class HAL : public IEventHandler {
public:
	HAL();
	virtual ~HAL();

	void handleEvent(EventType eventType) override;

	/**
	 * Starts receiving HAL events in an infinite loop in a seperate thread
	 */
	void startEventLoop();
	/**
	 * Stops the HAL event loop
	 */
	void stopEventLoop();
	/**
	 * Turns the green lamp on
	 */
	void GreenLampOn();
	/**
	 * Turns the green lamp off
	 */
	void GreenLampOff();
	/**
	 * Turns the yellow lamp on
	 */
	void YellowLampOn();
	/**
	 * Turns the yellow lamp off
	 */
	void YellowLampOff();
	/**
	 * Turns the red lamp on
	 */
	void RedLampOn();
	/**
	 * Turns the red lamp off
	 */
	void RedLampOff();
	/**
	 * Turns the LED at the 'Start' button on
	 */
	void StartLedOn();
	/**
	 * Turns the LED at the 'Start' button off
	 */
	void StartLedOff();
	/**
	 * Turns the LED at the 'Reset' button on
	 */
	void ResetLedOn();
	/**
	 * Turns the LED at the 'Reset' button off
	 */
	void ResetLedOff();
	/**
	 * Turns the 'Q1' LED on
	 */
	void Q1LedOn();
	/**
	 * Turns the 'Q1' LED off
	 */
	void Q1LedOff();
	/**
	 * Turns the 'Q2' LED on
	 */
	void Q2LedOn();
	/**
	 * Turns the 'Q2' LED off
	 */
	void Q2LedOff();
	/**
	 * Sets the 'Motor slow' flag. This does NOT start or stop the motor.
	 */
	void motorSlow();
	/**
	 * Clears the 'Motor slow' flag. This does NOT start or stop the motor.
	 */
	void motorFast();
	/**
	 * Start motor in right direction.
	 */
	void motorRight();
	/**
	 * Start motor in left direction.
	 */
	void motorLeft();
	/**
	 * Stops the motor.
	 */
	void motorStop();
	/**
	 * Opens the switch to let a workpiece pass.
	 */
	void openSwitch();
	/**
	 * Closes the switch to let a workpiece fall into the slide.
	 */
	void closeSwitch();
	/**
	 * Starts the ADC height measurement
	 */
	void startHeightMeasurement();
	/**
	 * Stops the ADC height measurement
	 */
	void stopHeightMeasurement();
private:
	uintptr_t gpio_bank_0;
	uintptr_t gpio_bank_1;
	uintptr_t gpio_bank_2;
	TSCADC tsc;
	ADC* adc;
	bool receivingRunning{false};
	int interruptID;
	int chanID;
	int conID;
	int adcChanID;
	int adcConID;
	std::thread eventLoopThread;
	std::thread adcReceivingThread;
	/**
	 * Initialize all interrupts on GPIO pins and ADC.
	 */
	void initInterrupts();
	/**
	 * Check the latest GPIO interrupt and handle it.
	 */
	void handleGpioInterrupt();
	/**
	 * Continuously receive ADC and GPIO events
	 */
	void eventLoop();
	/**
	 * Receives ADC values from ADC channel in a continuous loop
	 */
	void adcReceivingRoutine();
};
