/*
 * WaitForWorkpiece.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include "WaitForWorkpiece.h"
#include "WaitForBelt.h"
#include "hal/HeightSensor.h"
#include "logger/logger.hpp"


void WaitForWorkpiece::entry() {
    Logger::debug("[HFSM] Waiting for new workpiece...");
}

void WaitForWorkpiece::exit() { data->resetMeasurement(); }

HeightState WaitForWorkpiece::getCurrentState() {
    return HeightState::WAIT_FOR_WS;
}

bool WaitForWorkpiece::flatDetected() {
    actions->sendMotorSlowRequest(true);
    exit();
    new (this) WaitForBelt;
    entry();
    return true;
}

bool WaitForWorkpiece::highDetected() {
    actions->sendMotorSlowRequest(true);
    exit();
    new (this) WaitForBelt;
    entry();
    return true;
}
