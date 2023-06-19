/*
 * SubServiceModeCalOffset.cpp
 *
 *  Created on: 06.06.2023
 *      Author: Maik
 */

#include "SubServiceModeCalOffset.h"
#include "SubServiceModeCalRef.h"
#include "SubServiceModeTestsFailed.h"
#include "logger/logger.hpp"

#include <iostream>

void SubServiceModeCalOffset::entry() {
    Logger::debug("[ServiceMode] Calibrating HeightSensor offset (Belt)");
    Logger::user_info(
        "Make sure no workpiece is below the HeightSensor at both Master and "
        "Slave and press START to calibrate offset.");
    actions->btnStartLedOn();
    actions->btnResetLedOff();
    done = false;
}

void SubServiceModeCalOffset::exit() {}

bool SubServiceModeCalOffset::master_btnStart_PressedShort() {
    actions->calibrateOffset();
    Logger::user_info(
        "Calibration done. Press RESET button to continue or START to repeat");
    actions->btnResetLedOn();
    done = true;
    return true;
}

bool SubServiceModeCalOffset::master_btnReset_Pressed() {
    if (done) {
        exit();
        new (this) SubServiceModeCalRef;
        entry();
        return true;
    } else {
        return false;
    }
}

bool SubServiceModeCalOffset::slave_btnStart_PressedShort() {
    actions->calibrateOffset();
    Logger::user_info(
        "Calibration done. Press RESET button to continue or START to repeat");
    done = true;
    return true;
}

bool SubServiceModeCalOffset::slave_btnReset_Pressed() {
    if (done) {
        exit();
        new (this) SubServiceModeCalRef;
        entry();
        return true;
    } else {
        return false;
    }
}
