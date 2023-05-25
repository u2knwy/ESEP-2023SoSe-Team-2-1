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

	/**
	 * Height BELT was detected
	 */
	virtual bool beltDetected() {
		return false;
	}

	/**
	 * Height FLAT was detected
	 */
	virtual bool flatDetected() {
		return false;
	}

	/**
	 * Height HIGH was detected
	 */
	virtual bool highDetected() {
		return false;
	}

	/**
	 * Height HOLE was detected
	 */
	virtual bool holeDetected() {
		return false;
	}

	/**
	 * Height UNKNOWN was detected
	 */
	virtual bool unknownDetected() {
		return false;
	}
};
