/*
 * HeightSensorFSM.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include "HeightSensorFSM.h"
#include "states/WaitForWorkpiece.h"
#include "HeightActions.h"

HeightSensorFSM::HeightSensorFSM() {
	data = new HeightContextData();
	state = new WaitForWorkpiece();
	state->setData(data);
	actions = new HeightActions();
	state->setAction(actions);
}

HeightSensorFSM::~HeightSensorFSM() {
	delete data;
	delete actions;
	delete state;
}

void HeightSensorFSM::heightValueReceived(float valueMM) {
	state->heightValueReceived(valueMM);
}

EventType HeightSensorFSM::getDetectedWorkpieceType() {
	return data->getCurrentType();
}

HeightBasestate* HeightSensorFSM::getCurrentState() {
	return state;
}
