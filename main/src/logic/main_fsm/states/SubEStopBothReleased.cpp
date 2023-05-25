/*
 * SubEStopTwoPressed.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "SubEStopBothReleased.h"
#include "SubEStopOnePressed.h"
#include "SubEStopEndState.h"
#include "Standby.h"
#include "logger/logger.hpp"

#include <iostream>

void SubEStopBothReleased::entry() {
	Logger::debug("SubEStopBothReleased::entry");
	masterReset = false;
	slaveReset = false;
}

void SubEStopBothReleased::exit() {
	Logger::debug("SubEStopBothReleased::exit");
}

bool SubEStopBothReleased::master_btnReset_Pressed() {
	Logger::debug("SubEStopBothReleased::master_btnReset_Pressed");
	this->masterReset = true;
	if(masterReset && slaveReset) {
		Logger::debug("EStop was resetted -> leave EStop mode");
		exit();
		new(this) SubEStopEndState;
		entry();
		return true;
	}
	return false;
}

bool SubEStopBothReleased::slave_btnReset_Pressed() {
	Logger::debug("SubEStopBothReleased::slave_btnReset_Pressed");
	this->slaveReset = true;
	if(masterReset && slaveReset) {
		Logger::debug("EStop was resetted -> leave EStop mode");
		exit();
		new(this) SubEStopEndState;
		entry();
		return true;
	}
	return false;
}

bool SubEStopBothReleased::master_EStop_Pressed() {
	Logger::debug("SubEStopBothReleased::master_EStop_Pressed");
	exit();
	new(this) SubEStopOnePressed;
	entry();
	return true;
}

bool SubEStopBothReleased::slave_EStop_Pressed() {
	Logger::debug("SubEStopBothReleased::slave_EStop_Pressed");
	exit();
	new(this) SubEStopOnePressed;
	entry();
	return true;
}
