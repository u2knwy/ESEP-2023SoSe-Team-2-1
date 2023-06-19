/*
 * SubServiceModeSelftestActuators.cpp
 *
 *  Created on: 06.06.2023
 *      Author: Maik
 */

#include "SubServiceModeSelftestActuators.h"
#include "SubServiceModeEndState.h"
#include "SubServiceModeTestsFailed.h"
#include "logger/logger.hpp"


#include <iostream>

void SubServiceModeSelftestActuators::entry() {
    Logger::info("[ServiceMode] +++ SELFTEST ACTUATORS +++");
    actions->allActuatorsOn();
    Logger::user_info("Are all actuators on? Press START button if ok.");
    actions->btnStartLedOn();
    actions->btnResetLedOff();
}

void SubServiceModeSelftestActuators::exit() {
    actions->allActuatorsOff();
    actions->btnStartLedOff();
}

bool SubServiceModeSelftestActuators::master_btnStart_PressedShort() {
    exit();
    new (this) SubServiceModeEndState;
    entry();
    return true;
}

bool SubServiceModeSelftestActuators::master_btnReset_Pressed() {
    exit();
    new (this) SubServiceModeTestsFailed;
    entry();
    return true;
}

bool SubServiceModeSelftestActuators::slave_btnStart_PressedShort() {
    exit();
    new (this) SubServiceModeEndState;
    entry();
    return true;
}

bool SubServiceModeSelftestActuators::slave_btnReset_Pressed() {
    exit();
    new (this) SubServiceModeTestsFailed;
    entry();
    return true;
}
