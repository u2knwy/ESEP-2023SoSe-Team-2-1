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
#include <thread>
#include <mutex>
#include <sys/neutrino.h>
#include <sys/procmgr.h>
#include "Sensors.h"

#define ON_TIME_FAST_MS 500
#define ON_TIME_SLOW_MS 1000

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
	 * Lets the green lamp blink or turns it off
	 *
	 * @param on Blinking on or off
	 */
	void setGreenBlinking(bool on);

	/**
	 * Turns the green lamp off
	 */
	void greenLampOff();

	/**
	 * Turns the yellow lamp on
	 */
	void yellowLampOn();

	/**
	 * Lets the yellow lamp blink or turns it off
	 *
	 * @param on Blinking on or off
	 */
	void setYellowBlinking(bool on);

	/**
	 * Turns the yellow lamp off
	 */
	void yellowLampOff();

	/**
	 * Turns the red lamp on
	 */
	void redLampOn();

	/**
	 * Lets the red lamp blink or turns it off
	 *
	 * @param on Blinking on or off
	 * @param fast Blink fast(1Hz) or slow (0,5Hz)
	 */
	void setRedBlinking(bool on, bool fast);

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

	void motorStop();
	void motorSlow();
	void motorFast();
	/**
	 * Sets the 'Motor stop' pin.
	 */
	void setMotorStop(bool stop);

	/**
	 * Sets the 'Motor slow' pin.
	 */
	void setMotorSlow(bool slow);

	/**
	 * Sets the 'Motor right' pin and clears the 'Motor left' pin.
	 */
	void setMotorRight(bool right);

	/**
	 * Sets the 'Motor left' pin and clears the 'Motor right' pin.
	 */
	void setMotorLeft(bool left);

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
	bool isMaster;
	bool greenBlinking;
	std::thread th_GreenBlinking;
	bool yellowBlinking;
	std::thread th_YellowBlinking;
	bool redBlinking;
	std::thread th_RedBlinking;
	void configurePins();
	void subscribeToEvents();
	void thGreenLampFlashing(bool fast);
	void thYellowLampFlashing(bool fast);
	void thRedLampFlashing(bool fast);
	std::mutex mutex;
	// TEST
	int stopCnt;
	int fastCnt;
	int slowCnt;
};
