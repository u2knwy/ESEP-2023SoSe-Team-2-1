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
    Logger::error("[ServiceMode] Tests have failed. Press RESET to return to "
                  "Standby mode.");
    actions->master_warningOn();
    actions->btnStartLedOff();
    actions->btnResetLedOff();
}

void SubServiceModeTestsFailed::exit() { actions->master_warningOff(); }

bool SubServiceModeTestsFailed::isSubEndState() { return true; }
