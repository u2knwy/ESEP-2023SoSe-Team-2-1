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
	Logger::info("[EStop] Both released");
	masterReset = false;
	slaveReset = false;
}

void SubEStopBothReleased::exit() {
}

bool SubEStopBothReleased::master_btnReset_Pressed() {
	Logger::info("[EStop] Master confirmed reset");
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
	Logger::info("[EStop] Slave confirmed reset");
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
	exit();
	new(this) SubEStopOnePressed;
	entry();
	return true;
}

bool SubEStopBothReleased::slave_EStop_Pressed() {
	exit();
	new(this) SubEStopOnePressed;
	entry();
	return true;
}
