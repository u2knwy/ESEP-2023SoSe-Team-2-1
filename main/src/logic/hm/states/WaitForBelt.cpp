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
	Logger::debug("[HM] Waiting for belt...");
}

bool WaitForBelt::beltDetected() {
	actions->sendHeightResultFBM1(data->getCurrentType(), data->avgValue);
	exit();
	new(this) WaitForWorkpiece;
	entry();
	return true;
}

bool WaitForBelt::unknownDetected() {
	data->setCurrentType(WorkpieceType::UNKNOWN);
	return false;
}
