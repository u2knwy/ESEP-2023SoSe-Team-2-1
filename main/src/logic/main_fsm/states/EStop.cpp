/*
 * EStop.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "EStop.h"
#include "Standby.h"
#include "SubEStopOnePressed.h"
#include <iostream>
#include "logger/logger.hpp"

MainState EStop::getCurrentState() {
	return MainState::ESTOP;
};

void EStop::entry() {
	Logger::info("Entered EStop mode");
	initSubStateEStop();
	actions->setEStopMode();
	actions->setMotorStop(true);
}

void EStop::exit() {
	actions->setMotorStop(false);
	delete substateEStop;
}

void EStop::initSubStateEStop() {
	substateEStop = new SubEStopOnePressed;
	substateEStop->entry();
}

bool EStop::master_EStop_Pressed() {
	return substateEStop->master_EStop_Pressed();
}

bool EStop::master_EStop_Released() {
	return substateEStop->master_EStop_Released();
}

bool EStop::slave_EStop_Pressed() {
	return substateEStop->slave_EStop_Pressed();
}

bool EStop::slave_EStop_Released() {
	return substateEStop->slave_EStop_Released();
}

bool EStop::master_btnReset_Pressed() {
	bool handled = substateEStop->master_btnReset_Pressed();
	if(substateEStop->isSubEndState()) {
		exit();
		new(this) Standby;
		entry();
		return true;
	}
	return handled;
}

bool EStop::slave_btnReset_Pressed() {
	bool handled = substateEStop->slave_btnReset_Pressed();
	if(substateEStop->isSubEndState()) {
		exit();
		new(this) Standby;
		entry();
		return true;
	}
	return handled;
}
