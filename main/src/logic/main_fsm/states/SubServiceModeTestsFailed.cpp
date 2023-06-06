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
	Logger::debug("SubServiceModeTestsFailed::entry");
	actions->displayWarning();
	Logger::info("Tests failed. Press STOP to return to Standby mode.");
}

void SubServiceModeTestsFailed::exit() {
	Logger::debug("SubServiceModeTestsFailed::exit");
}

bool SubServiceModeTestsFailed::isSubEndState() {
	return true;
}

