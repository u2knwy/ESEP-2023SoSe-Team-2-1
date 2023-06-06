/*
 * SubEStopOnePressed.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "SubServiceModeSelftestSensors.h"
#include "SubServiceModeSelftestActuators.h"
#include "SubServiceModeTestsFailed.h"
#include "logger/logger.hpp"

#include <iostream>

void SubServiceModeSelftestSensors::entry() {
	Logger::debug("SubServiceModeSelftestSensors::entry");
	Logger::info("Interrupt all sensors and watch console outputs. Press START button if ok.");
}

void SubServiceModeSelftestSensors::exit() {
	Logger::debug("SubServiceModeSelftestSensors::exit");
}

bool SubServiceModeSelftestSensors::master_btnStart_PressedShort() {
	exit();
	new(this) SubServiceModeSelftestActuators;
	entry();
	return true;
}

bool SubServiceModeSelftestSensors::master_btnReset_Pressed() {
	exit();
	new(this) SubServiceModeTestsFailed;
	entry();
	return true;
}

bool SubServiceModeSelftestSensors::slave_btnStart_PressedShort() {
	exit();
	new(this) SubServiceModeSelftestActuators;
	entry();
	return true;
}

bool SubServiceModeSelftestSensors::slave_btnReset_Pressed() {
	exit();
	new(this) SubServiceModeTestsFailed;
	entry();
	return true;
}
