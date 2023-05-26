/*
 * HeightSensorFSM.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include "HeightSensorFSM.h"
#include "states/WaitForWorkpiece.h"
#include "HeightActions.h"
#include "hal/HeightSensor.h"

HeightSensorFSM::HeightSensorFSM() {
	data = new HeightContextData();
	state = new WaitForWorkpiece();
	state->setData(data);
	actions = new HeightActions();
	state->setAction(actions);
	state->entry();
}

HeightSensorFSM::~HeightSensorFSM() {
	delete data;
	delete actions;
	delete state;
}

void HeightSensorFSM::heightValueReceived(float valueMM) {
	if(valueMM > HEIGHT_FLAT-1 && valueMM < HEIGHT_FLAT+1) {
		state->flatDetected();
	} else if(valueMM > HEIGHT_HIGH-1 && valueMM < HEIGHT_HIGH+1) {
		state->highDetected();
	} else if(valueMM < HEIGHT_CONV_MAX) {
		state->beltDetected();
	} else if(valueMM > HEIGHT_HOLE-1 && valueMM < HEIGHT_HOLE+1) {
		state->holeDetected();
	} else {
		state->unknownDetected();
	}
}

WorkpieceType HeightSensorFSM::getDetectedWorkpieceType() {
	return data->getCurrentType();
}

HeightState HeightSensorFSM::getCurrentState() {
	return state->getCurrentState();
}
