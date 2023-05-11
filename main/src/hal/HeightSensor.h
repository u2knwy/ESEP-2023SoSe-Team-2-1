/*
 * HeightSensor.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#pragma once

#include "adc/ADC.h"
#include "events/events.h"

#include <sys/mman.h>
#include <hw/inout.h>

#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/procmgr.h>
#include <thread>
#include <chrono>

/*---------------------------------------------------------------------------
   HEIGHT SENSOR CONFIGURATION
----------------------------------------------------------------------------- */
#define HEIGHT_FLAT 18
#define HEIGHT_HIGH 21
#define HEIGHT_HOLE 10
#define ADC_OFFSET_CONV 3600

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
	HeightSensor();
	virtual ~HeightSensor();
	void start();
	void stop();
	void calibrateOffset(int offsetValue);
	void calibrateRefHigh(int highValue);
private:
	TSCADC tsc;
	ADC* adc;
	int chanID;
	int conID;
    std::thread measureThread;
    bool running{false};
    void threadFunction();
    int adcOffset;
    int adcIncPerMillimeter;
    int adcValueToMillimeter(int adcValue);
};
