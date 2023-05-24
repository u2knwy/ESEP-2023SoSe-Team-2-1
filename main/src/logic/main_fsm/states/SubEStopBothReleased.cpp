/*
 * SubEStopTwoPressed.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "SubEStopBothReleased.h"
#include "SubEStopOnePressed.h"
#include "Standby.h"
#include "logger/logger.hpp"

#include <iostream>

void SubEStopBothReleased::entry() {
	Logger::debug("SubEStopBothReleased::entry");
	masterReset = false;
	slaveReset = false;
}

bool SubEStopBothReleased::master_btnReset_Pressed() {
	Logger::debug("SubEStopBothReleased::master_btnReset_Pressed");
	this->masterReset = true;
	if(masterReset && slaveReset) {
		exit();
		new(this) Standby;
		entry();
	}
	return true;
}

bool SubEStopBothReleased::slave_btnReset_Pressed() {
	Logger::debug("SubEStopBothReleased::slave_btnReset_Pressed");
	this->slaveReset = true;
	if(masterReset && slaveReset) {
		exit();
		new(this) Standby;
		entry();
	}
	return true;
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
