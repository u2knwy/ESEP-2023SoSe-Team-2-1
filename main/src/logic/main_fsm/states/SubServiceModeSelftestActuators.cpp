/*
 * SubServiceModeSelftestActuators.cpp
 *
 *  Created on: 06.06.2023
 *      Author: Maik
 */

#include "SubServiceModeSelftestActuators.h"
#include "SubServiceModeTestsFailed.h"
#include "logger/logger.hpp"

#include <iostream>

void SubServiceModeSelftestActuators::entry() {
	Logger::debug("SubServiceModeSelftestActuators::entry");
	actions->allActuatorsOn();
	Logger::info("Are all actuators on? Press START button if ok.");
}

void SubServiceModeSelftestActuators::exit() {
	Logger::debug("SubServiceModeSelftestActuators::exit");
}

bool SubServiceModeSelftestActuators::master_btnStart_PressedShort() {
	return true;
}

bool SubServiceModeSelftestActuators::master_btnReset_Pressed() {
	exit();
	new(this) SubServiceModeTestsFailed;
	entry();
	return true;
}

bool SubServiceModeSelftestActuators::slave_btnStart_PressedShort() {
	return true;
}

bool SubServiceModeSelftestActuators::slave_btnReset_Pressed() {
	exit();
	new(this) SubServiceModeTestsFailed;
	entry();
	return true;
}
