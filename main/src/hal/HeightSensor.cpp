/*
 * HeightSensor.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include "HeightSensor.h"
#include "logger/logger.hpp"
#ifdef SIM_ACTIVE
	#include "simqnxgpioapi.h" // must be last include !!!
	#include "simqnxirqapi.h"
#endif

HeightSensor::HeightSensor() : chanID(-1), conID(-1) {
	adc = new ADC(tsc);
	windowCapacity = ADC_SAMPLE_SIZE;
	window.reserve(windowCapacity);
	Configuration &conf = Configuration::getInstance();
	Calibration cal = conf.getCalibration();
	calibrateOffset(cal.calOffset);
	calibrateRefHigh(cal.calRef);
	nMeasurements = 0;
}

HeightSensor::~HeightSensor() {
	stop();
	delete adc;
}

void HeightSensor::registerMeasurementCallback(HeightCallback callback) {
	this->heightValueCallback = callback;
}

void HeightSensor::unregisterNewMeasurementCallback() {
	this->heightValueCallback = 0;
}

void HeightSensor::start() {
	Logger::info("[HM] Starting...");

	/* ### Create channel to receive ADC values ### */
	chanID = ChannelCreate(0);
	if (chanID < 0) {
		perror("Could not create a channel!\n");
	}

	//Connect to channel.
	conID = ConnectAttach(0, 0, chanID, _NTO_SIDE_CHANNEL, 0);
	if (conID < 0) {
		perror("Could not connect to channel!");
	}


	/* ### Setup ### */
	ThreadCtl(_NTO_TCTL_IO, 0);	//Request IO privileges for process.

	// Request interrupt and IO abilities.
	int procmgr_status = procmgr_ability(0,
	PROCMGR_ADN_ROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_INTERRUPT,
	PROCMGR_ADN_NONROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_INTERRUPT,
	PROCMGR_ADN_ROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_IO,
	PROCMGR_ADN_NONROOT | PROCMGR_AOP_ALLOW | PROCMGR_AID_IO,
	PROCMGR_AID_EOL);
	if (procmgr_status != EOK) {
		perror("Requested abilities failed or denied!");
		exit(EXIT_FAILURE);
	}

	InterruptEnable(); //Enables interrupts

	adc->registerAdcISR(conID, PULSE_ADC_SAMPLING_DONE);

	// ### Start thread for handling interrupt messages.
	measureThread = std::thread(&HeightSensor::threadFunction, this);

	adc->sample();
}

void HeightSensor::stop() {
	// Detach interrupts.
	adc->adcDisable();
	adc->unregisterAdcISR();

	if (measureThread.joinable()) {
		Logger::debug("[HM] Stopping receiving thread...");
		// Stop receiving thread.
		MsgSendPulse(conID, -1, PULSE_STOP_THREAD, 0);
		measureThread.join();
	}

	int detachStatus = ConnectDetach(conID);
	if (detachStatus != EOK) {
		Logger::debug("Detaching ADC channel failed!");
	}

	int destroyStatus = ChannelDestroy(chanID);
	if (destroyStatus != EOK) {
		Logger::debug("Destroying ADC channel failed!");
	}
}

float HeightSensor::adcValueToMillimeter(int adcValue) {
	int abs = adcOffset-adcValue;
	float mm = ((float) abs / adcIncPerMillimeter);
	if(mm < 0)
		return 0.0;
	return mm;
}

void HeightSensor::addValue(int value) {
	std::lock_guard<std::mutex> lock(mtx);
	if (window.size() == ADC_SAMPLE_SIZE) {
		// If window is full: Remove first element
		window.erase(window.begin());
	}
	// Add value to window
	window.push_back(value);
}

void HeightSensor::threadFunction() {
	ThreadCtl(_NTO_TCTL_IO, 0); // Request IO privileges for this thread.

	Logger::debug("[HM] ADC receiving routine started...");
	Calibration adcCal = Configuration::getInstance().getCalibration();
	calibrateOffset(adcCal.calOffset);
	calibrateRefHigh(adcCal.calRef);

	using namespace std;
	_pulse msg;
	running = true;
	while (running) {
		int recvid = MsgReceivePulse(chanID, &msg, sizeof(_pulse), nullptr);

		if (recvid < 0) {
			Logger::error("[HM] Error while receiving pulse message");
		}

		if (recvid == 0) { // pulse received.

			// Stop thread while it blocks.
			if (msg.code == PULSE_STOP_THREAD) {
				Logger::debug("[HM] ADC thread kill code received");
				running = false;
				continue;
			}

			// ADC interrupt value.
			if (msg.code == PULSE_ADC_SAMPLING_DONE) {
				int heightRaw = msg.value.sival_int;
				addValue(heightRaw);
				nMeasurements++;
				// Every x measurements -> notify via callback
				if((nMeasurements % HM_SEND_INTERVAL) == 0) {
					nMeasurements = 0;
					//float heightMillimeter = adcValueToMillimeter(heightRaw);
					//float heightMillimeter = getAverageHeight();
					float heightMillimeter = getMedianHeight();
					//Logger::debug("[HM] New value: " + std::to_string(heightMillimeter));
					if(heightValueCallback != nullptr) {
						heightValueCallback(heightMillimeter);
					}
				}
				this_thread::sleep_for(chrono::milliseconds(10));
				adc->sample();
			}

			// Do not ignore OS pulses!
		}
	}

	Logger::debug("ADC thread has stopped.");
}

float HeightSensor::getAverageHeight() {
	if(window.empty())
		return 0.0;
	long sum = 0;
	for(int val : window) {
		sum += val;
	}
	int avg = sum / window.size();
	return adcValueToMillimeter(avg);
}

float HeightSensor::getMedianHeight() {
    std::sort(window.begin(), window.end());
    size_t size = window.size();
    int median;
	if (size % 2 == 0) {
		// Even number of elements, calculate the average of the two middle elements
		median = (window[size / 2 - 1] + window[size / 2]) / 2.0;
	} else {
		// Odd number of elements, return the middle element
		median = window[size / 2];
	}
	return adcValueToMillimeter(median);
}

float HeightSensor::getMaxHeight() {
	int max = 0;
	for(int val : window) {
		if(val > max) {
			max = val;
		}
	}
	return adcValueToMillimeter(max);
}

int HeightSensor::getLastRawValue() {
	return window.back();
}
