/*
 * HeightSensor.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#pragma once

#include <errno.h>
#include <hw/inout.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/neutrino.h>
#include <sys/procmgr.h>

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "IHeightSensor.h"
#include "adc/ADC.h"
#include "configuration/Configuration.h"
#include "events/EventManager.h"
#include "events/events.h"

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

class HeightSensor : public IHeightSensor, public IEventHandler {
 public:
  HeightSensor(std::shared_ptr<EventManager> mngr);
  virtual ~HeightSensor();
  void handleEvent(Event event) override;
  void registerOnNewValueCallback(HeightCallback callback) override;
  void unregisterOnNewValueCallback() override;
  void start() override;
  void stop() override;
  float getAverageHeight() override;
  float getMaxHeight() override;
  float getMedianHeight() override;
  int getLastRawValue() override;

 private:
  TSCADC tsc;
  ADC* adc;
  HeightCallback heightValueCallback = nullptr;
  int chanID;
  int conID;
  std::thread measureThread;
  std::vector<int> window;
  int nMeasurements;
  std::mutex mtx;
  void addValue(int value);
  bool running{false};
  void threadFunction();
  float adcValueToMillimeter(int adcValue);
};
