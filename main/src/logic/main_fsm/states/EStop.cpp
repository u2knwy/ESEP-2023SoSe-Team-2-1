/*
 * EStop.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "EStop.h"
#include <iostream>
#include "logger/logger.hpp"

MainState EStop::getCurrentState() {
	return MainState::ESTOP;
};

void EStop::entry() {
	Logger::debug("EStop::entry");
}

void EStop::exit() {
	Logger::debug("EStop::exit");
}

bool EStop::master_EStop_Pressed() {
	Logger::debug("EStop::master_EStop_Pressed");
	return true;
}

bool EStop::master_EStop_Released() {
	Logger::debug("EStop::master_EStop_Released");
	return true;
}

bool EStop::slave_EStop_Pressed() {
	Logger::debug("EStop::slave_EStop_Pressed");
	return true;
}

bool EStop::slave_EStop_Released() {
	Logger::debug("EStop::slave_EStop_Released");
	return true;
}

bool EStop::master_btnReset_Pressed() {
	Logger::debug("EStop::master_btnReset_Pressed");
	return true;
}

bool EStop::slave_btnReset_Pressed() {
	Logger::debug("EStop::slave_btnReset_Pressed");
	return true;
}
