/*
 * IHeightSensor.h
 *
 *  Created on: 05.06.2023
 *      Author: Maik
 */
#pragma once
#include <functional>
#include <memory>
#include <mutex>
#include <vector>
#include "logger/logger.hpp"

/*---------------------------------------------------------------------------
         HEIGHT SENSOR CONFIGURATION
----------------------------------------------------------------------------- */
#define HEIGHT_CONV_MAX    2
#define HEIGHT_FLAT        21
#define HEIGHT_HIGH        25
#define HEIGHT_HOLE_MIN    4
#define HEIGHT_HOLE_MAX    9
#define HEIGHT_TOL         2
#define ADC_DEFAULT_OFFSET 3333
#define ADC_DEFAULT_HIGH   2222
// use N samples for averaging / max. value (sliding window)
#define ADC_SAMPLE_SIZE  100
#define HM_SEND_INTERVAL 5

class IHeightSensor {
  public:
    using HeightCallback = std::function<void(float)>;
    virtual void registerOnNewValueCallback(HeightCallback callback) = 0;
    virtual void unregisterOnNewValueCallback() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual float getAverageHeight() = 0;
    virtual float getMaxHeight() = 0;
    virtual float getMedianHeight() = 0;
    virtual int getLastRawValue() = 0;

  protected:
    IHeightSensor() {}
    virtual ~IHeightSensor() {}
    int adcOffset{0};
    int adcIncPerMillimeter{0};
    HeightCallback heightValueCallback = nullptr;
    std::vector<int> window;
    std::mutex mutex_cal;
    bool running{false};
    void calibrateOffset(int offsetValue) {
        std::lock_guard<std::mutex> lock(mutex_cal);
        adcOffset = offsetValue;
        Logger::debug("[HeightSensor] Calibrated offset: " + std::to_string(adcOffset));
    }
    void calibrateRefHigh(int highValue) {
        std::lock_guard<std::mutex> lock(mutex_cal);
        adcIncPerMillimeter = (adcOffset - highValue) / HEIGHT_HIGH;
        Logger::debug("[HeightSensor] Calibrated reference (25.0mm): " + std::to_string(highValue) + " -> " + std::to_string(adcIncPerMillimeter) + " inc/mm");
    }
};
