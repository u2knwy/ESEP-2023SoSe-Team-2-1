/*
 * MotorBasestate.h
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */
#pragma once

#include "MotorActions.h"
#include "MotorContextData.h"

enum MotorState {
	MOTOR_NONE,
	STOPPED,
	RIGHT_FAST,
	RIGHT_SLOW
};

class MotorBasestate {
protected:
	MotorActions* actions;
	MotorContextData* data;

public:
	virtual ~MotorBasestate(){}

	virtual void setData(MotorContextData* data) {
		this->data = data;
	}
	virtual void setAction(MotorActions* actions) {
		this->actions = actions;
	}

	virtual MotorState getCurrentState() {
		return MOTOR_NONE;
	};

	virtual void entry() {}
	virtual void exit() {}

	virtual bool handleFlagsUpdated() { return false; }
	virtual bool motorRightFast() { return false; }
	virtual bool motorRightSlow() { return false; }
	virtual bool motorStopped() { return false; }
};
