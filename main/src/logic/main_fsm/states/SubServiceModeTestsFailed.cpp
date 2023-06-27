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
    actions->master_warningOn();
    actions->slave_warningOn();
    actions->master_btnStartLedOff();
    actions->slave_btnStartLedOff();
    actions->master_btnResetLedOff();
    actions->slave_btnResetLedOff();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    Logger::error("[ServiceMode] Tests have failed. Press RESET to return to Standby mode.");
}

void SubServiceModeTestsFailed::exit() {
	actions->master_warningOff();
    actions->slave_warningOff();
}

bool SubServiceModeTestsFailed::isSubEndState() { return true; }
