/*
 * Actuators.h
 *
 *  Created on: 26.05.2023
 *      Author: Maik
 */
#pragma once

#include "events/IEventHandler.h"
#include "events/EventManager.h"

#include <memory>
#include <sys/neutrino.h>
#include <sys/procmgr.h>
#include "Sensors.h"


class Actuators : public IEventHandler {
public:
	Actuators(std::shared_ptr<EventManager> mngr);
	virtual ~Actuators();

	void handleEvent(Event event) override;

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
	 * Sets the 'Motor stop' pin.
	 */
	void motorStop(bool stop);

	/**
	 * Sets the 'Motor slow' pin.
	 */
	void motorSlow(bool slow);

	/**
	 * Sets the 'Motor right' pin and clears the 'Motor left' pin.
	 */
	void motorRight();

	/**
	 * Sets the 'Motor left' pin and clears the 'Motor right' pin.
	 */
	void motorLeft();

	/**
	 * Opens the switch to let a workpiece pass.
	 */
	void openSwitch();

	/**
	 * Closes the switch to let a workpiece fall into the slide.
	 */
	void closeSwitch();

private:
	uintptr_t gpio_bank_1;
	uintptr_t gpio_bank_2;
	std::shared_ptr<EventManager> eventManager;
	void configurePins();
	void subscribeToEvents();
};
