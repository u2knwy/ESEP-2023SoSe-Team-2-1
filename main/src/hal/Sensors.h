/*
 * hal.h
 *
 *  Created on: 31.03.2023
 *      Author: Maik
 */

#pragma once

#include "hal.h"

#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <thread>
#include <chrono>
#include <memory>

#include "events/IEventHandler.h"
#include "events/EventManager.h"
#include "events/EventSender.h"

// Time in ms where a button press is recognized as "pressed long".
#define BTN_LONG_PRESSED_TIME_MS 2000

class Sensors : public IEventHandler, public EventSender {
public:
	Sensors(std::shared_ptr<EventManager> mngr);
	virtual ~Sensors();

	void handleEvent(Event eventType) override;

	/**
	 * Starts receiving HAL events in an infinite loop in a seperate thread
	 */
	void startEventLoop();
	/**
	 * Stops the HAL event loop
	 */
	void stopEventLoop();

	bool lbStartBlocked();
	bool lbStartUnblocked();
	bool lbSwitchBlocked();
	bool lbSwitchUnblocked();
	bool lbRampBlocked();
	bool lbRampUnblocked();
	bool metalDetected();
	bool lbEndBlocked();
	bool lbEndUnblocked();

	bool startPressed();
	bool stopPressed();
	bool resetPressed();
	bool eStopPressed();
	bool eStopReleased();

private:
	uintptr_t gpio_bank_0;
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
