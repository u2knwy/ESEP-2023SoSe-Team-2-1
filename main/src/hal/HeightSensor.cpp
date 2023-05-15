/*
 * HeightSensor.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include "HeightSensor.h"
#include "logger/logger.hpp"

HeightSensor::HeightSensor(std::shared_ptr<HeightSensorFSM> fsm) : fsm(fsm), chanID(-1), conID(-1) {
	adc = new ADC(tsc);
	adcOffset = ADC_OFFSET_CONV;
	adcIncPerMillimeter = 100;
	windowCapacity = ADC_SAMPLE_SIZE;
	window.reserve(windowCapacity);
}

HeightSensor::~HeightSensor() {
	stop();
	delete adc;
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

void HeightSensor::calibrateOffset(int offsetValue) {
	adcOffset = offsetValue;
	Logger::debug("Calibrated OFFSET: " + std::to_string(adcOffset) + " inc");
}

void HeightSensor::calibrateRefHigh(int highValue) {
	adcIncPerMillimeter = (adcOffset - highValue) / HEIGHT_HIGH;
	Logger::debug("Calibrated REF High: " + std::to_string(adcIncPerMillimeter) + " inc/mm");
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
				float heightMillimeter = adcValueToMillimeter(heightRaw);
				//Logger::debug("[HM] Value from ADC: " + std::to_string(heightRaw));
				//Logger::debug("[HM] Height in mm: " + std::to_string(heightMillimeter));
				fsm->heightValueReceived(heightMillimeter);
				//this_thread::sleep_for(chrono::milliseconds(100));
				adc->sample();
			}

			// Do not ignore OS pulses!
		}
	}

	Logger::debug("ADC thread has stopped.");
}

float HeightSensor::getAverageHeight() {
	long sum = 0;
	for(int val : window) {
		sum += val;
	}
	int avg = sum / window.size();
	return adcValueToMillimeter(avg);
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
