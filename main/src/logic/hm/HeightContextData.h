/*
 * HeightContextData.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#pragma once

#include <vector>

#include "configuration/Configuration.h"
#include "events/eventtypes_enum.h"
#include "hal/IHeightSensor.h"

struct HeightResult {
  WorkpieceType type{WorkpieceType::WS_UNKNOWN};
  float average{0.0};
  float max{0.0};
};

class HeightContextData {
 public:
  HeightContextData();
  virtual ~HeightContextData();

  /**
   * Adds a new measurement value and recalculate the actual average and maximum
   * value
   *
   * @param value New value received
   */
  void addValue(float value);

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
  static bool isFlat(float value);
  static bool isHigh(float value);
  static bool isHole(float value);
  static bool isUnknown(float value);

 private:
  float avgValue;
  float maxValue;
  int nMeasurements;
  std::vector<float> measurements;
};
