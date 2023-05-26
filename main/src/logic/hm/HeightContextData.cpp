/*
 * HeightContextData.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include <logic/hm/HeightContextData.h>
#include "logger/logger.hpp"
#include "events/events.h"

HeightContextData::HeightContextData() {
	avgValue = 0.0;
	maxValue = 0.0;
	currentType = WorkpieceType::UNKNOWN;
}

HeightContextData::~HeightContextData() {
	// TODO Auto-generated destructor stub
}

WorkpieceType HeightContextData::getCurrentType() {
	return currentType;
}

void HeightContextData::setCurrentType(WorkpieceType type) {
	Logger::debug("[HFSM] Current type changed: " + std::to_string(type));
	this->currentType = type;
}
