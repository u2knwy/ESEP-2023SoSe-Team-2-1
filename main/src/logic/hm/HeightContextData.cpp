/*
 * HeightContextData.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include <logic/hm/HeightContextData.h>

#include "events/events.h"
#include "logger/logger.hpp"

HeightContextData::HeightContextData() {
    measurements = std::vector<float>(1000);
    resetMeasurement();
}

HeightContextData::~HeightContextData() { measurements.clear(); }

void HeightContextData::resetMeasurement() {
    avgValue = 0.0;
    maxValue = 0.0;
    nMeasurements = 0;
    measurements.clear();
}

void HeightContextData::addValue(float newValue) {
    measurements.push_back(newValue);
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2);
	ss << "hm_value: " << newValue << " mm";
	Logger::to_file(ss.str());

    if (nMeasurements == 0) {
        avgValue = newValue;
    } else {
        avgValue =
            ((avgValue * nMeasurements) + newValue) / (nMeasurements + 1);
    }
    nMeasurements++;
    if (newValue > maxValue)
        maxValue = newValue;
}

float HeightContextData::getAverageValue() { return avgValue; }

float HeightContextData::getMaximumValue() { return maxValue; }

HeightResult HeightContextData::getCurrentResult() {
    HeightResult result;

    int totalValues = measurements.size();
    Logger::to_file("[HFSM] Get Result -> # of measurements: " + std::to_string(totalValues));
    if (totalValues == 0) {
        result.type = WorkpieceType::WS_UNKNOWN;
        result.average = 0.0;
        result.max = 0.0;
        return result;
    }

    // Throw out first and last 10 % of measurements
    int startIndex = totalValues * 0.05;
    int endIndex = totalValues * 0.95;
    int nValues = endIndex - startIndex;
    double sum = 0;
    for (auto it = measurements.begin() + startIndex;
         it != measurements.begin() + endIndex; ++it) {
        float val = *it;
        sum += val;
        if (val > maxValue)
            maxValue = val;
    }
    float begin = measurements.at(startIndex);
    float middle = measurements.at(totalValues / 2);
    float end = measurements.at(endIndex);

    if (isFlat(begin) && isFlat(middle) && isFlat(end)) {
        result.type = WorkpieceType::WS_F;
    } else if (isHigh(begin) && isHigh(middle) && isHigh(end)) {
        result.type = WorkpieceType::WS_OB;
    } else if (isHigh(begin) && isHole(middle) && isHigh(end)) {
        result.type = WorkpieceType::WS_BOM;
    } else {
        result.type = WorkpieceType::WS_UNKNOWN;
    }
    result.average = (float) (sum / nValues);
    result.max = maxValue;

    std::stringstream ss;
    ss.precision(1);
    ss << std::fixed << std::setprecision(2);
    ss << "[HFSM] Get Result -> begin=" << begin
       << ", middle=" << middle
       << ", end=" << end
	   << ", avg=" << result.average
	   << ", max=" << result.max
	   << ", type=" << WP_TYPE_TO_STRING(result.type);
    Logger::debug(ss.str());
    Logger::to_file(ss.str());

    return result;
}

bool HeightContextData::isFlat(float value) {
    if (value > HEIGHT_FLAT - HEIGHT_TOL && value < HEIGHT_FLAT + HEIGHT_TOL) {
        return true;
    } else {
        return false;
    }
}

bool HeightContextData::isHigh(float value) {
    if (value > HEIGHT_HIGH - HEIGHT_TOL && value < HEIGHT_HIGH + HEIGHT_TOL) {
        return true;
    } else {
        return false;
    }
}

bool HeightContextData::isHole(float value) {
    if (value >= HEIGHT_HOLE_MIN && value <= HEIGHT_HOLE_MAX) {
        return true;
    } else {
        return false;
    }
}

bool HeightContextData::isUnknown(float value) {
    if (!isFlat(value) && !isHigh(value) && !isHole(value)) {
        return true;
    } else {
        return false;
    }
}
