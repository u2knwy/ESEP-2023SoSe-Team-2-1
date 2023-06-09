/*
 * SubServiceModeTestsFailed.cpp
 *
 *  Created on: 06.06.2023
 *      Author: Maik
 */

#include "SubServiceModeTestsFailed.h"
#include "logger/logger.hpp"

#include <iostream>

void SubServiceModeTestsFailed::entry() {
	Logger::info("[ServiceMode] Tests have failed. Press RESET to return to Standby mode.");
	actions->displayWarning();
	actions->btnStartLedOff();
	actions->btnResetLedOff();
}

void SubServiceModeTestsFailed::exit() {
	actions->warningOff();
}

bool SubServiceModeTestsFailed::isSubEndState() {
	return true;
}
