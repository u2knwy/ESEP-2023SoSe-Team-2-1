/*
 * Standby.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "Standby.h"
#include <iostream>
#include "logger/logger.hpp"

MainState Standby::getCurrentState() {
	return MainState::STANDBY;
};

void Standby::entry() {
	Logger::debug("Standby::entry");
}

void Standby::exit() {
	Logger::debug("Standby::exit");
}

bool Standby::master_btnStart_PressedShort() {
	Logger::debug("Standby::master_btnStart_PressedShort");
	return true;
}

bool Standby::master_btnStart_PressedLong() {
	Logger::debug("Standby::master_btnStart_PressedLong");
	return true;
}

bool Standby::master_EStop_Pressed() {
	Logger::debug("Standby::master_EStop_Pressed");
	return true;
}

bool Standby::slave_btnStart_PressedShort() {
	Logger::debug("Standby::slave_btnStart_PressedShort");
	return true;
}

bool Standby::slave_btnStart_PressedLong() {
	Logger::debug("Standby::slave_btnStart_PressedLong");
	return true;
}

bool Standby::slave_EStop_Pressed() {
	Logger::debug("Standby::slave_EStop_Pressed");
	return true;
}

