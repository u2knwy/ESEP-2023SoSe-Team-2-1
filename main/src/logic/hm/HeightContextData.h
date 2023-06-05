/*
 * HeightContextData.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#pragma once

#include "events/eventtypes_enum.h"
#include "configuration/Configuration.h"

struct HeightResult {
	WorkpieceType type{WorkpieceType::UNKNOWN};
	float average{0.0};
	float max{0.0};
};

class HeightContextData {
public:
	HeightContextData();
	virtual ~HeightContextData();

	/**
	 * Gets the currently detected workpiece type
	 *
	 * @return current type
	 */
	WorkpieceType getCurrentType();

	/**
	 * Updates the currently detected workpiece type
	 *
	 * @return new type
	 */
	void setCurrentType(WorkpieceType type);

	/**
	 * Adds a new measurement value and recalculate the actual average and maximum value
	 *
	 * @param value New value received
	 */
	void updateAvgAndMaxValue(float value);

	/**
	 * Get the current average value
	 *
	 * @return average in mm
	 */
	float getAverageValue();

	/**
	 * Get the current maximum value
	 *
	 * @return max. value in mm
	 */
	float getMaximumValue();

	/**
	 * Resets the current measurement of average and maximum value
	 */
	void resetMeasurement();
	HeightResult getCurrentResult();
private:
	WorkpieceType currentType;
	float avgValue;
	float maxValue;
	int nMeasurements;
};
