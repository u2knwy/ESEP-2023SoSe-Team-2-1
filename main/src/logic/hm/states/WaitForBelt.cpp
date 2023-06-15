/*
 * WaitForWorkpiece.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include "hal/HeightSensor.h"
#include "WaitForWorkpiece.h"
#include "WaitForBelt.h"

void WaitForBelt::entry() {
	Logger::debug("[HFSM] Waiting for belt...");
}

HeightState WaitForBelt::getCurrentState() {
	return HeightState::WAIT_FOR_BELT;
}

bool WaitForBelt::beltDetected() {
	actions->sendHeightResult();
	actions->sendMotorSlowRequest(false);
	exit();
	new(this) WaitForWorkpiece;
	entry();
	return true;
}

bool WaitForBelt::unknownDetected() {
	return false;
}
