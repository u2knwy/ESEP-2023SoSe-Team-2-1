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
	EventType currentType;
	int avgValue;
	int maxValue;
	void setCurrentType(EventType type);
};
