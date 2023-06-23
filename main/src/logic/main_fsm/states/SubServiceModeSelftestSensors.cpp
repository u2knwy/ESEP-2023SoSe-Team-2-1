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
    Logger::info("+++ SELFTEST SENSORS +++");
    Logger::user_info("Place any workpiece at Start of FBM1 and wait until it arrives at the end of FBM2.");
    actions->master_btnStartLedOn();
    actions->master_btnResetLedOff();

    master_lbStartOk = false;
    master_lbSwitchOk = false;
    master_lbRampOk = false;
    master_lbEndOk = false;
    slave_lbStartOk = false;
    slave_lbSwitchOk = false;
    slave_lbRampOk = false;
    slave_lbEndOk = false;
}

void SubServiceModeSelftestSensors::exit() {}

bool SubServiceModeSelftestSensors::checkResult() {
    if (!master_lbStartOk) {
        Logger::error("[ServiceMode] LB Start at FBM1 not ok");
    }
    if (!master_lbSwitchOk) {
        Logger::error("[ServiceMode] LB Switch at FBM1 not ok");
    }
    if (!master_lbRampOk) {
        Logger::error("[ServiceMode] LB Ramp at FBM1 not ok");
    }
    if (!master_lbEndOk) {
        Logger::error("[ServiceMode] LB End at FBM1 not ok");
    }
    if (!slave_lbStartOk) {
        Logger::error("[ServiceMode] LB Start at FBM2 not ok");
    }
    if (!slave_lbSwitchOk) {
        Logger::error("[ServiceMode] LB Switch at FBM2 not ok");
    }
    if (!slave_lbRampOk) {
        Logger::error("[ServiceMode] LB Ramp at FBM2 not ok");
    }
    if (!slave_lbEndOk) {
        Logger::error("[ServiceMode] LB End at FBM2 not ok");
    }

    return master_lbStartOk && master_lbSwitchOk && master_lbRampOk &&
           master_lbEndOk && slave_lbStartOk && slave_lbSwitchOk &&
           slave_lbRampOk && slave_lbEndOk;
}

bool SubServiceModeSelftestSensors::master_btnStart_PressedShort() {
    if (checkResult()) {
        exit();
        new (this) SubServiceModeSelftestActuators;
        entry();
        return true;
    } else {
        exit();
        new (this) SubServiceModeTestsFailed;
        entry();
        return true;
    }
}

bool SubServiceModeSelftestSensors::master_btnReset_Pressed() {
    exit();
    new (this) SubServiceModeTestsFailed;
    entry();
    return true;
}

bool SubServiceModeSelftestSensors::slave_btnStart_PressedShort() {
    exit();
    new (this) SubServiceModeSelftestActuators;
    entry();
    return true;
}

bool SubServiceModeSelftestSensors::slave_btnReset_Pressed() {
    exit();
    new (this) SubServiceModeTestsFailed;
    entry();
    return true;
}

bool SubServiceModeSelftestSensors::master_LBA_Blocked() {
    actions->master_sendMotorRightRequest(true);
    master_lbStartOk = true;
    return true;
}

bool SubServiceModeSelftestSensors::master_LBW_Blocked() {
    actions->master_openGate(true);
    master_lbSwitchOk = true;
    return true;
}

bool SubServiceModeSelftestSensors::master_LBE_Blocked() {
    master_lbEndOk = true;
    return true;
}

bool SubServiceModeSelftestSensors::slave_LBA_Blocked() {
    actions->master_sendMotorRightRequest(false);
    actions->slave_sendMotorRightRequest(true);
    slave_lbStartOk = true;
    return true;
}

bool SubServiceModeSelftestSensors::slave_LBW_Blocked() {
    actions->slave_openGate(true);
    slave_lbSwitchOk = true;
    return true;
}

bool SubServiceModeSelftestSensors::slave_LBE_Blocked() {
    actions->slave_sendMotorRightRequest(false);
    Logger::user_info("[ServiceMode] Block ramp at FBM1 and FBM2 and then press START to continue");
    slave_lbEndOk = true;
    return true;
}

bool SubServiceModeSelftestSensors::master_LBR_Blocked() {
    master_lbRampOk = true;
    return true;
}

bool SubServiceModeSelftestSensors::slave_LBR_Blocked() {
    slave_lbRampOk = true;
    return true;
}
