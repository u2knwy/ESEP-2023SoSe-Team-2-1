/*
 * HeightActions.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include <logic/hm/HeightActions.h>
#include "logger/logger.hpp"

HeightActions::HeightActions() {
	// TODO Auto-generated constructor stub

}

HeightActions::~HeightActions() {
	// TODO Auto-generated destructor stub
}

void HeightActions::sendHeightResultFBM1(EventType type, int avg) {
	Logger::debug("[HM] Height Result at FBM1: " + EVENT_TO_STRING(type) + ", average (mm): " + std::to_string(avg));
}

void HeightActions::sendHeightResultFBM2(EventType type, int max) {
	Logger::debug("[HM] Height Result at FBM2: " + EVENT_TO_STRING(type) + ", max (mm): " + std::to_string(max));
}
