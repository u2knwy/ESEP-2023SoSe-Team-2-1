/*
 * WaitForWorkpiece.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include "WaitForWorkpiece.h"
#include "hal/HeightSensor.h"
#include "WaitForBelt.h"
#include "High.h"
#include "logger/logger.hpp"

void WaitForWorkpiece::entry() {
	data->resetMeasurement();
	Logger::debug("[HFSM] Waiting for new workpiece...");
}

void WaitForWorkpiece::exit() {
}

HeightState WaitForWorkpiece::getCurrentState() {
	return HeightState::WAIT_FOR_WS;
}

bool WaitForWorkpiece::flatDetected() {
	data->setCurrentType(WorkpieceType::WS_F);
	Logger::debug("[HFSM] Current type: WS_F");
	actions->setMotorSlow(true);
	exit();
	new(this) WaitForBelt;
	entry();
	return true;
}

bool WaitForWorkpiece::highDetected() {
	data->setCurrentType(WorkpieceType::WS_OB);
	Logger::debug("[HFSM] Current type: WS_OB");
	actions->setMotorSlow(true);
	exit();
	new(this) High;
	entry();
	return true;
}
