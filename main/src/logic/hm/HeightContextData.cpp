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
	currentType = WorkpieceType::WS_UNKNOWN;
	measurements = std::vector<float>(1000);
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
	//measurements.shrink_to_fit();
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
//	std::stringstream ss;
//	ss << "New value: " << std::setprecision(2) << newValue << " mm -> n=" << nMeasurements << "avg=" << avgValue << ", max=" << maxValue << " mm";
//	Logger::debug(ss.str());
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
	HeightResult result;

	int totalValues = measurements.size();
	Logger::debug("HeightContextData::getCurrentResultV2 -> n=" + std::to_string(totalValues));
	if(totalValues == 0) {
		result.type = WorkpieceType::WS_UNKNOWN;
		result.average = 0.0;
		result.max = 0.0;
		return result;
	}

	// Throw out first and last 10 % of measurements
	int startIndex = totalValues * 0.05;
	int endIndex = totalValues * 0.95;
	int nValues = endIndex - startIndex + 1;
	double sum = 0;
	for(auto it = measurements.begin() + startIndex; it != measurements.end(); ++it) {
		float val = *it;
		sum += val;
		if(val > maxValue)
			maxValue = val;
	}
	float begin = measurements.at(startIndex);
	float middle = measurements.at(totalValues/2);
	float end = measurements.at(endIndex);
	std::stringstream ss;
	ss << "[HFSM] GET RESULT -> begin=" << begin << ", middle=" << middle << ", end=" << end;
	Logger::debug(ss.str());

	if(isFlat(begin) && isFlat(middle) && isFlat(end)) {
		result.type = WorkpieceType::WS_F;
	} else if(isHigh(begin) && isHigh(middle) && isHigh(end)) {
		result.type = WorkpieceType::WS_OB;
	} else if(isHigh(begin) && isHole(middle) && isHigh(end)) {
		result.type = WorkpieceType::WS_BOM;
	} else {
		result.type = WorkpieceType::WS_UNKNOWN;
	}
	result.average = (float) (sum / nValues);
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
