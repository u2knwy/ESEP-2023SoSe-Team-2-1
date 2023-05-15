/*
 * HeightContextData.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#pragma once

#include "events/eventtypes_enum.h"

class HeightContextData {
public:
	HeightContextData();
	virtual ~HeightContextData();
	int avgValue;
	int maxValue;
	EventType getCurrentType();
	void setCurrentType(EventType type);
private:
	EventType currentType;
};
