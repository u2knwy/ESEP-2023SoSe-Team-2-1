/*
 * WaitForWorkpiece.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include "hal/HeightSensor.h"
#include "WaitForWorkpiece.h"
#include "WaitForBelt.h"
#include "High.h"

void High::entry() {
	Logger::debug("[HM] High detected - waiting for belt or hole...");
}

HeightState High::getCurrentState() {
	return HeightState::HIGH;
}

bool High::beltDetected() {
	Logger::debug("[HM] Belt detected -> WS_OB");
	data->setCurrentType(WorkpieceType::WS_OB);
	actions->sendHeightResult();
	actions->setMotorSlow(false);
	exit();
	new(this) WaitForWorkpiece;
	entry();
	return true;
}

bool High::holeDetected() {
	Logger::debug("[HM] Hole detected -> WS_BOM");
	data->setCurrentType(WorkpieceType::WS_BOM);
	exit();
	new(this) WaitForBelt;
	entry();
	return true;
}

bool High::unknownDetected() {
	data->setCurrentType(WorkpieceType::WS_UNKNOWN);
	exit();
	new(this) WaitForBelt;
	entry();
	return true;
}
