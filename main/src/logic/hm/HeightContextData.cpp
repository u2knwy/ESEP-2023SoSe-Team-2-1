/*
 * HeightContextData.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include <logic/hm/HeightContextData.h>
#include "logger/logger.hpp"
#include "events/events.h"

HeightContextData::HeightContextData() : avgValue(0.0), maxValue(0.0), currentType(EventType::HM_M_WS_UNKNOWN) {
}

HeightContextData::~HeightContextData() {
	// TODO Auto-generated destructor stub
}


void HeightContextData::setCurrentType(EventType type) {
	this->currentType = type;
}
