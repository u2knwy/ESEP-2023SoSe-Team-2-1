/*
 * Actuators.h
 *
 *  Created on: 26.05.2023
 *      Author: Maik
 */
#pragma once

#include "IActuators.h"
#include "events/IEventHandler.h"
#include "events/EventManager.h"
#include "events/events.h"

#include <memory>
#include <thread>
#include <mutex>
#include <sys/neutrino.h>
#include <sys/procmgr.h>
#include "Sensors.h"

#define ON_TIME_FAST_MS 500
#define ON_TIME_SLOW_MS 1000
#define ON_TIME_PUSHER_MS 300
#define ON_TIME_SWITCH_MS 1000

class Actuators : public IActuators {
public:
	Actuators(std::shared_ptr<EventManager> mngr);
	virtual ~Actuators();

	//void handleEvent(Event event) override;

	void standbyMode() override;
	void runningMode() override;
	void serviceMode() override;
	void errorMode() override;
	void estopMode() override;
	void greenLampOn() override;
	void setGreenBlinking(bool on) override;
	void greenLampOff() override;
	void yellowLampOn() override;
	void setYellowBlinking(bool on) override;
	void yellowLampOff() override;
	void redLampOn() override;
	void setRedBlinking(bool on, bool fast) override;
	void redLampOff() override;
	void startLedOn() override;
	void startLedOff() override;
	void resetLedOn() override;
	void resetLedOff() override;
	void q1LedOn() override;
	void q1LedOff() override;
	void q2LedOn() override;
	void q2LedOff() override;
	void motorStop() override;
	void motorSlow() override;
	void motorFast() override;
	void openSwitch() override;
	void closeSwitch() override;
	void sortOut() override;
	void letPass() override;
private:
	uintptr_t gpio_bank_1;
	uintptr_t gpio_bank_2;
	bool isMaster;
	bool greenBlinking;
	bool yellowBlinking;
	bool redBlinking;
	bool hasPusher;
	std::thread th_GreenBlinking;
	std::thread th_YellowBlinking;
	std::thread th_RedBlinking;
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
	void configurePins();
	void thGreenLampFlashing(bool fast);
	void thYellowLampFlashing(bool fast);
	void thRedLampFlashing(bool fast);
	bool handleLampEvent(EventType event, LampState state);
};
