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
	Logger::debug("[HM] Waiting for new workpiece...");
}

void WaitForWorkpiece::exit() {
	actions->newWorkpieceDetected();
}

bool WaitForWorkpiece::flatDetected() {
	data->setCurrentType(WorkpieceType::WS_F);
	exit();
	new(this) WaitForBelt;
	entry();
	return true;
}

bool WaitForWorkpiece::highDetected() {
	data->setCurrentType(WorkpieceType::WS_OB);
	exit();
	new(this) High;
	entry();
	return true;
}
