/*
 * HeightSensor.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#pragma once

#include "adc/ADC.h"
#include "events/events.h"
#include "logic/hm/HeightSensorFSM.h"
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
   HEIGHT SENSOR CONFIGURATION
----------------------------------------------------------------------------- */
#define HEIGHT_CONV_MAX 2
#define HEIGHT_FLAT 21
#define HEIGHT_HIGH 25
#define HEIGHT_HOLE 6
#define ADC_DEFAULT_OFFSET 3648
#define ADC_DEFAULT_HIGH 2323
// use N samples for averaging / max. value (sliding window)
#define ADC_SAMPLE_SIZE 100

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

class HeightSensor {
public:
	HeightSensor(std::shared_ptr<HeightSensorFSM> fsm);
	virtual ~HeightSensor();
	void start();
	void stop();
	void calibrateOffset(int offsetValue);
	void calibrateRefHigh(int highValue);
	float getAverageHeight();
	float getMaxHeight();
private:
	std::shared_ptr<HeightSensorFSM> fsm;
	TSCADC tsc;
	ADC* adc;
	int chanID;
	int conID;
    std::thread measureThread;
    std::vector<int> window;
    size_t windowCapacity;
    std::mutex mtx;
    void addValue(int value);
    bool running{false};
    void threadFunction();
    int adcOffset;
    int adcIncPerMillimeter;
    float adcValueToMillimeter(int adcValue);
};
