/*
 * HeightContextData.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#pragma once

#include "events/eventtypes_enum.h"
#include "configuration/Configuration.h"

class HeightContextData {
public:
	HeightContextData();
	virtual ~HeightContextData();
	int avgValue;
	int maxValue;
	WorkpieceType getCurrentType();
	void setCurrentType(WorkpieceType type);
private:
	WorkpieceType currentType;
};
