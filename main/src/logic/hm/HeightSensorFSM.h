/*
 * HeightSensorFSM.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#pragma once

#include "HeightActions.h"
#include "HeightBasestate.h"
#include "HeightContextData.h"
#include "events/IEventHandler.h"

class HeightSensorFSM {
public:
	HeightSensorFSM();
	virtual ~HeightSensorFSM();
	void heightValueReceived(float valueMM);
	WorkpieceType getDetectedWorkpieceType();
	HeightState getCurrentState();
private:
	HeightActions* actions;
	HeightBasestate* state;
	HeightContextData* data;
};
