/*
 * HeightContextData.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include <logic/hm/HeightContextData.h>
#include "logger/logger.hpp"
#include "events/events.h"

HeightContextData::HeightContextData() {
	currentType = WorkpieceType::UNKNOWN;
	resetMeasurement();
}

HeightContextData::~HeightContextData() {
	// TODO Auto-generated destructor stub
}

WorkpieceType HeightContextData::getCurrentType() {
	return currentType;
}

void HeightContextData::setCurrentType(WorkpieceType type) {
	Logger::debug("[HFSM] Current type changed: " + std::to_string(type));
	this->currentType = type;
}

void HeightContextData::resetMeasurement() {
	Logger::debug("Measurements were resetted.");
	avgValue = 0.0;
	maxValue = 0.0;
	nMeasurements = 0;
}

void HeightContextData::updateAvgAndMaxValue(float newValue) {
	if(nMeasurements == 0) {
		avgValue = newValue;
	} else {
		avgValue = ((avgValue*nMeasurements) + newValue) / (nMeasurements+1);
	}
	nMeasurements++;
	if(newValue > maxValue)
		maxValue = newValue;
/*	std::stringstream ss;
	ss << "New value: " << std::setprecision(2) << newValue << " mm -> n=" << nMeasurements << "avg=" << avgValue << ", max=" << maxValue << " mm";
	Logger::debug(ss.str());*/
}

float HeightContextData::getAverageValue() {
	return avgValue;
}

float HeightContextData::getMaximumValue() {
	return maxValue;
}

HeightResult HeightContextData::getCurrentResult() {
	HeightResult result;
	result.type = currentType;
	result.average = avgValue;
	result.max = maxValue;
	return result;
}
