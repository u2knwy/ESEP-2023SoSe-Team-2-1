/*
 * IHeightSensor.h
 *
 *  Created on: 05.06.2023
 *      Author: Maik
 */
#pragma once
#include <functional>
#include <vector>

/*---------------------------------------------------------------------------
   HEIGHT SENSOR CONFIGURATION
----------------------------------------------------------------------------- */
#define HEIGHT_CONV_MAX 2
#define HEIGHT_FLAT 21
#define HEIGHT_HIGH 25
#define HEIGHT_HOLE 6
#define HEIGHT_TOL 2
#define ADC_DEFAULT_OFFSET 3648
#define ADC_DEFAULT_HIGH 2323
// use N samples for averaging / max. value (sliding window)
#define ADC_SAMPLE_SIZE 10
#define HM_SEND_INTERVAL 5

class IHeightSensor {
public:
	using HeightCallback = std::function<void(float)>;
	virtual ~IHeightSensor() {};
	virtual void registerMeasurementCallback(HeightCallback callback) = 0;
	virtual void unregisterNewMeasurementCallback() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual float getAverageHeight() = 0;
	virtual float getMaxHeight() = 0;
	virtual float getMedianHeight() = 0;
	virtual int getLastRawValue() = 0;
protected:
    int adcOffset;
    int adcIncPerMillimeter;
	HeightCallback heightValueCallback = nullptr;
    std::vector<int> window;
    size_t windowCapacity;
    bool running{false};
	void calibrateOffset(int offsetValue) {
		adcOffset = offsetValue;
	}
	void calibrateRefHigh(int highValue) {
		adcIncPerMillimeter = (adcOffset - highValue) / HEIGHT_HIGH;
	}
};
