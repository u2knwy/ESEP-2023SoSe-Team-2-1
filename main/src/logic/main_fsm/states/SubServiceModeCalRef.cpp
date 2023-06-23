/*
 * SubServiceModeCalRef.cpp
 *
 *  Created on: 06.06.2023
 *      Author: Maik
 */

#include "SubServiceModeCalRef.h"
#include "SubServiceModeSelftestSensors.h"
#include "SubServiceModeTestsFailed.h"
#include "logger/logger.hpp"


#include <iostream>

void SubServiceModeCalRef::entry() {
    Logger::info("[ServiceMode] Calibrating HeightSensor reference (High)");
    Logger::user_info(
        "Place a high workpiece (25 mm) below the HeightSensor at both Master and "
        "Slave and press START to calibrate reference.");
    actions->master_btnStartLedOn();
    actions->master_btnResetLedOff();
    done = false;
}

void SubServiceModeCalRef::exit() {}

bool SubServiceModeCalRef::master_btnStart_PressedShort() {
    actions->calibrateReference();
    Logger::user_info("Calibration done. Press RESET button to continue or START to repeat");
    actions->master_btnResetLedOn();
    done = true;
    return true;
}

bool SubServiceModeCalRef::master_btnReset_Pressed() {
    if (done) {
        exit();
        new (this) SubServiceModeSelftestSensors;
        entry();
        return true;
    } else {
        return false;
    }
}

bool SubServiceModeCalRef::slave_btnStart_PressedShort() {
    actions->calibrateReference();
    Logger::user_info("Calibration done. Press RESET button to continue or START to repeat");
    actions->master_btnResetLedOn();
    done = true;
    return true;
}

bool SubServiceModeCalRef::slave_btnReset_Pressed() {
    if (done) {
        exit();
        new (this) SubServiceModeSelftestSensors;
        entry();
        return true;
    } else {
        return false;
    }
}
