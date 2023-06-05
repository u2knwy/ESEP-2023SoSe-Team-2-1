/*
 * HeightSensor.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#pragma once

#include "IHeightSensor.h"
#include "adc/ADC.h"
#include "events/events.h"
#include "configuration/Configuration.h"

#include <sys/mman.h>
#include <hw/inout.h>

#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/procmgr.h>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>

/*---------------------------------------------------------------------------
   ADC CONFIGURATION
----------------------------------------------------------------------------- */
#define ADC_BASE 0x44E0D000
#define ADC_LENGTH 0x2000

// TSC_ADC register offsets (spruh73l.pdf S.1747)
#define ADC_IRQ_ENABLE_SET 0x2c
#define ADC_IRQ_ENABLE_CLR 0x30
#define ADC_IRQ_STATUS 0x28
#define ADC_CTRL 0x40
#define ADC_DATA 0x100

// ADC IRQ pin mask
#define ADC_IRQ_PIN_MASK 0x2

class HeightSensor : public IHeightSensor {
public:
	HeightSensor();
	virtual ~HeightSensor();
	void registerMeasurementCallback(HeightCallback callback) override;
	void unregisterNewMeasurementCallback() override;
	void start() override;
	void stop() override;
	float getAverageHeight() override;
	float getMaxHeight() override;
	int getLastRawValue() override;
private:
	TSCADC tsc;
	ADC* adc;
	HeightCallback heightValueCallback = nullptr;
	int chanID;
	int conID;
    std::thread measureThread;
    std::vector<int> window;
    size_t windowCapacity;
    std::mutex mtx;
    void addValue(int value);
    bool running{false};
    void threadFunction();
    float adcValueToMillimeter(int adcValue);
};
