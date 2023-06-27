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
    Logger::info("[ServiceMode] +++ SELFTEST SENSORS +++");
    Logger::user_info("Place any workpiece at Start of FBM1 and wait until it arrives at the end of FBM2.");
    actions->master_btnStartLedOn();
    actions->slave_btnStartLedOn();
    actions->master_btnResetLedOff();
    actions->slave_btnResetLedOff();
    data->resetSelftestSensorsResult();
}

void SubServiceModeSelftestSensors::exit() {}

bool SubServiceModeSelftestSensors::master_btnStart_PressedShort() {
    if (data->getSelftestSensorsResult()) {
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
    data->ssResult.master_lbStartOk = true;
    actions->master_sendMotorRightRequest(true);
    return true;
}

bool SubServiceModeSelftestSensors::master_LBW_Blocked() {
	data->ssResult.master_lbSwitchOk = true;
    actions->master_openGate(true);
    return true;
}

bool SubServiceModeSelftestSensors::master_LBE_Blocked() {
	data->ssResult.master_lbEndOk = true;
    return true;
}

bool SubServiceModeSelftestSensors::slave_LBA_Blocked() {
	data->ssResult.slave_lbStartOk = true;
    actions->master_sendMotorRightRequest(false);
    actions->slave_sendMotorRightRequest(true);
    return true;
}

bool SubServiceModeSelftestSensors::slave_LBW_Blocked() {
	data->ssResult.slave_lbSwitchOk = true;
    actions->slave_openGate(true);
    return true;
}

bool SubServiceModeSelftestSensors::slave_LBE_Blocked() {
	data->ssResult.slave_lbEndOk = true;
    actions->slave_sendMotorRightRequest(false);
    Logger::user_info("[ServiceMode] Block ramp at FBM1 and FBM2 and then press START to continue");
    return true;
}

bool SubServiceModeSelftestSensors::master_LBR_Blocked() {
	data->ssResult.master_lbRampOk = true;
    return true;
}

bool SubServiceModeSelftestSensors::slave_LBR_Blocked() {
	data->ssResult.slave_lbRampOk = true;
    return true;
}
