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
#include <thread>
#include <chrono>
#include <memory>
#include <sys/neutrino.h>
#include <sys/procmgr.h>

#include "events/IEventHandler.h"
#include "events/EventManager.h"

/* Helper macros */
#define BIT_MASK(x) (1 << (x))

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
#define GPIO_OE_REGISTER		0x134
#define GPIO_LEVELDETECT0 		0x140
#define GPIO_LEVELDETECT1 		0x144
#define GPIO_RISINGDETECT 		0x148
#define GPIO_FALLINGDETECT 		0x14C

#define GPIO_IRQSTATUS_0 		0x2C
#define GPIO_IRQSTATUS_1 		0x30
#define GPIO_IRQSTATUS_SET_0 	0x34
#define GPIO_IRQSTATUS_SET_1 	0x38


class HAL : public IEventHandler {
public:
	HAL(std::shared_ptr<EventManager> mngr);
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
	 * Sets all actuators when Standby mode is entered:
	 * - all off
	 */
	void standbyMode();
	/**
	 * Sets all actuators when Running mode is entered:
	 * - green lamp on
	 */
	void runningMode();

	/**
	 * Sets all actuators when Service mode is entered
	 * - green lamp flashing
	 */
	void serviceMode();
	/**
	 * Sets all actuators when Error mode is entered
	 * - red lamp flashing fast
	 * - motor stop
	 */
	void errorMode();
	/**
	 * Sets all actuators when EStop mode is entered:
	 * - all off
	 */
	void estopMode();
	/**
	 * Turns the green lamp on
	 */
	void greenLampOn();
	/**
	 * Turns the green lamp off
	 */
	void greenLampOff();
	/**
	 * Lets the green lamp blink
	 */
	void greenLampBlinking();
	/**
	 * Turns the yellow lamp on
	 */
	void yellowLampOn();
	/**
	 * Turns the yellow lamp off
	 */
	void yellowLampOff();
	/**
	 * Turns the red lamp on
	 */
	void redLampOn();
	/**
	 * Lets the red lamp blink fast
	 */
	void redLampBlinkFast();
	/**
	 * Lets the red lamp blink slow
	 */
	void redLampBlinkSlow();
	/**
	 * Turns the red lamp off
	 */
	void redLampOff();
	/**
	 * Turns the LED at the 'Start' button on
	 */
	void startLedOn();
	/**
	 * Turns the LED at the 'Start' button off
	 */
	void startLedOff();
	/**
	 * Turns the LED at the 'Reset' button on
	 */
	void resetLedOn();
	/**
	 * Turns the LED at the 'Reset' button off
	 */
	void resetLedOff();
	/**
	 * Turns the 'Q1' LED on
	 */
	void q1LedOn();
	/**
	 * Turns the 'Q1' LED off
	 */
	void q1LedOff();
	/**
	 * Turns the 'Q2' LED on
	 */
	void q2LedOn();
	/**
	 * Turns the 'Q2' LED off
	 */
	void q2LedOff();
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

private:
	uintptr_t gpio_bank_0;
	uintptr_t gpio_bank_1;
	uintptr_t gpio_bank_2;
	bool receivingRunning{false};
	int interruptID;
	int chanID;
	int conID;
	std::thread eventLoopThread;
	std::shared_ptr<EventManager> eventManager;

	/**
	 * Configure all Pins as input / outputs
	 */
	void configurePins();
	/**
	 * Initialize all interrupts on GPIO pins and ADC.
	 */
	void initInterrupts();
	/**
	 * Subscribes to events from EventManager
	 */
	void subscribeToEvents();
	/**
	 * Check the latest GPIO interrupt and handle it.
	 */
	void handleGpioInterrupt();
	/**
	 * Continuously receive ADC and GPIO events
	 */
	void eventLoop();
};
