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
	measurements = std::vector<float>(100);
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
	measurements.clear();
}

void HeightContextData::updateAvgAndMaxValue(float newValue) {
	measurements.push_back(newValue);
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
/*	HeightResult result;
	result.type = currentType;
	result.average = avgValue;
	result.max = maxValue;
	return result;*/
	return getCurrentResultV2();
}

HeightResult HeightContextData::getCurrentResultV2() {
	// Throw out first x % of measurements
	int totalValues = measurements.size();
	int startIndex = totalValues * 0.1;
	int nValues = measurements.size() - startIndex;
	float sum = 0.0;
	for(auto it = measurements.begin() + startIndex; it != measurements.end(); ++it) {
		float val = *it;
		sum += val;
		if(val > maxValue)
			maxValue = val;
	}
	float begin = measurements.at(startIndex);
	float middle = measurements.at(totalValues/2);
	float end = *(measurements.end());

	HeightResult result;
	if(isFlat(begin) && isFlat(middle) && isFlat(end)) {
		result.type = WorkpieceType::WS_F;
	} else if(isHigh(begin) && isHigh(middle) && isHigh(end)) {
		result.type = WorkpieceType::WS_OB;
	} else if(isHigh(begin) && isHole(middle) && isHigh(end)) {
		result.type = WorkpieceType::WS_BOM;
	} else {
		result.type = WorkpieceType::UNKNOWN;
	}
	result.average = sum / nValues;
	result.max = maxValue;
	return result;
}

bool HeightContextData::isFlat(float value) {
	if(value > HEIGHT_FLAT-HEIGHT_TOL && value < HEIGHT_FLAT+HEIGHT_TOL) {
		return true;
	} else {
		return false;
	}
}

bool HeightContextData::isHigh(float value) {
	if(value > HEIGHT_HIGH-HEIGHT_TOL && value < HEIGHT_HIGH+HEIGHT_TOL) {
		return true;
	} else {
		return false;
	}
}

bool HeightContextData::isHole(float value) {
	if(value > HEIGHT_HOLE-HEIGHT_TOL && value < HEIGHT_HOLE+HEIGHT_TOL) {
		return true;
	} else {
		return false;
	}
}

bool HeightContextData::isUnknown(float value) {
	if(!isFlat(value) && !isHigh(value) && !isHole(value)) {
		return true;
	} else {
		return false;
	}
}
