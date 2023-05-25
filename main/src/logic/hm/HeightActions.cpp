/*
 * HeightActions.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include "HeightActions.h"
#include "logger/logger.hpp"

HeightActions::HeightActions() {
	// TODO Auto-generated constructor stub

}

HeightActions::~HeightActions() {
	// TODO Auto-generated destructor stub
}

void HeightActions::newWorkpieceDetected() {
	// TODO: Send event to indicate the measurement starts now
}

void HeightActions::sendHeightResultFBM1(WorkpieceType type, int avg) {
	EventType event;
	bool master = true; // TODO: Get from config
	if(master) {
		switch(type) {
		case WS_F:
			event = EventType::HM_M_WS_F;
			break;
		case WS_OB:
			event = EventType::HM_M_WS_OB;
			break;
		case WS_BOM:
			event = EventType::HM_M_WS_BOM;
			break;
		default:
			event = EventType::HM_M_WS_UNKNOWN;
		}
	} else {
		switch(type) {
		case WS_F:
			event = EventType::HM_S_WS_F;
			break;
		case WS_OB:
			event = EventType::HM_S_WS_OB;
			break;
		case WS_BOM:
			event = EventType::HM_S_WS_BOM;
			break;
		default:
			event = EventType::HM_S_WS_UNKNOWN;
		}
	}

	Logger::debug("[HM] Height Result at FBM1: " + EVENT_TO_STRING(event) + ", average (mm): " + std::to_string(avg));
}

void HeightActions::sendHeightResultFBM2(WorkpieceType type, int max) {
	Logger::debug("[HM] Height Result at FBM2: " + EVENT_TO_STRING(type) + ", max (mm): " + std::to_string(max));
}
