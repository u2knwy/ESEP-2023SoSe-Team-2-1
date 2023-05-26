/*
 * WaitForWorkpiece.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include <logic/hm/states/WaitForWorkpiece.h>
#include "hal/HeightSensor.h"
#include "WaitForBelt.h"
#include "High.h"
#include "logger/logger.hpp"

void WaitForWorkpiece::entry() {
	data->avgValue = 0;
	data->maxValue = 0;
	Logger::debug("[HFSM] Waiting for new workpiece...");
}

void WaitForWorkpiece::exit() {
	actions->newWorkpieceDetected();
}

HeightState WaitForWorkpiece::getCurrentState() {
	return HeightState::WAIT_FOR_WS;
}

bool WaitForWorkpiece::flatDetected() {
	data->setCurrentType(WorkpieceType::WS_F);
	Logger::debug("[HFSM] Current type: WS_F");
	exit();
	new(this) WaitForBelt;
	entry();
	return true;
}

bool WaitForWorkpiece::highDetected() {
	data->setCurrentType(WorkpieceType::WS_OB);
	Logger::debug("[HFSM] Current type: WS_OB");
	exit();
	new(this) High;
	entry();
	return true;
}
