/*
 * HeightBasestate.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */
#pragma once

#include "HeightContextData.h"
#include "HeightActions.h"
#include "logger/logger.hpp"

class HeightBasestate {
protected:
	HeightContextData* data;
	HeightActions* actions;

public:
	virtual ~HeightBasestate(){};

	virtual void setData(HeightContextData* data) {
		this->data = data;
	}
	virtual void setAction(HeightActions* actions) {
		this->actions = actions;
	}

	virtual void entry() {}
	virtual void exit() {}

	virtual bool heightValueReceived(float valueMM) {
		return false;
	}
};
