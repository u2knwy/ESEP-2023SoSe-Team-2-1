/*
 * HeightSensorMock.cpp
 *
 *  Created on: 05.06.2023
 *      Author: Maik
 */

#include "HeightSensorMock.h"
#include "logger/logger.hpp"

HeightSensorMock::HeightSensorMock() {}

void HeightSensorMock::registerOnNewValueCallback(HeightCallback callback) {
    this->heightValueCallback = callback;
}

void HeightSensorMock::unregisterOnNewValueCallback() {
    this->heightValueCallback = nullptr;
}

void HeightSensorMock::start() { running = true; }

void HeightSensorMock::stop() { running = false; }

float HeightSensorMock::getAverageHeight() { return 0.0; }

float HeightSensorMock::getMaxHeight() { return 0.0; }

float HeightSensorMock::getMedianHeight() { return 0.0; }

int HeightSensorMock::getLastRawValue() { return 0; }
