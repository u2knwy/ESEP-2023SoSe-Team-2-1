/*
 * Standby.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "Standby.h"
#include "Running.h"
#include "EStop.h"
#include "ServiceMode.h"
#include "logger/logger.hpp"

#include <iostream>

MainState Standby::getCurrentState() {
	return MainState::STANDBY;
};

void Standby::entry() {
	Logger::info("Entered Standby mode");
	actions->master_sendMotorStopRequest(true);
	actions->slave_sendMotorStopRequest(true);
	actions->setStandbyMode();
}

void Standby::exit() {
}

bool Standby::master_btnStart_PressedShort() {
	exit();
	new(this) Running;
	entry();
	return true;
}

bool Standby::master_btnStart_PressedLong() {
	exit();
	new(this) ServiceMode;
	entry();
	return true;
}

bool Standby::master_EStop_Pressed() {
	exit();
	new(this) EStop;
	entry();
	return true;
}

bool Standby::slave_btnStart_PressedShort() {
	exit();
	new(this) Running;
	entry();
	return true;
}

bool Standby::slave_btnStart_PressedLong() {
	exit();
	new(this) ServiceMode;
	entry();
	return true;
}

bool Standby::slave_EStop_Pressed() {
	exit();
	new(this) EStop;
	entry();
	return true;
}

