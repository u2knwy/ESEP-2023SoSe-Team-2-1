/*
 * WaitForWorkpiece.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include "WaitForBelt.h"
#include "WaitForWorkpiece.h"
#include "hal/HeightSensor.h"


void WaitForBelt::entry() {
	Logger::debug("[HFSM] Waiting for belt...");
    Logger::to_file("\n+++ [HM] NEW WORKPIECE +++");
}

HeightState WaitForBelt::getCurrentState() {
    return HeightState::WAIT_FOR_BELT;
}

bool WaitForBelt::beltDetected() {
    actions->sendHeightResult();
    actions->sendMotorSlowRequest(false);
    exit();
    new (this) WaitForWorkpiece;
    entry();
    return true;
}

bool WaitForBelt::workpieceHeightDetected(float height) {
	data->addValue(height);
	return true;
}
