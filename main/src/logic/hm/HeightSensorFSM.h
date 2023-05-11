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

class HeightSensorFSM {
public:
	HeightSensorFSM();
	virtual ~HeightSensorFSM();
	void heightValueReceived(float valueMM);
private:
	HeightActions* actions;
	HeightBasestate* state;
	HeightContextData* data;
};
