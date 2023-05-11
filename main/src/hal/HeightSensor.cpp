/*
 * HeightSensor.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include "HeightSensor.h"
#include "logger/logger.hpp"

HeightSensor::HeightSensor() : chanID(-1), conID(-1) {
	adc = new ADC(tsc);
	adcOffset = ADC_OFFSET_CONV;
	adcIncPerMillimeter = 100;
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
		Logger::debug("[HM] Stopping...");
		// Stop receiving thread.
		MsgSendPulse(conID, -1, PULSE_STOP_THREAD, 0);
		measureThread.join();
	}

	int detachStatus = ConnectDetach(conID);
	if (detachStatus != EOK) {
		Logger::error("Detaching ADC channel failed!");
	}

	int destroyStatus = ChannelDestroy(chanID);
	if (destroyStatus != EOK) {
		Logger::error("Destroying ADC channel failed!");
	}
}

void HeightSensor::calibrateOffset(int offsetValue) {
	adcOffset = offsetValue;
}

void HeightSensor::calibrateRefHigh(int highValue) {
	adcIncPerMillimeter = (adcOffset - highValue) / HEIGHT_HIGH;
}

int HeightSensor::adcValueToMillimeter(int adcValue) {
	return (int) ((adcOffset-adcValue) * adcIncPerMillimeter);
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
				Logger::debug("[HM] Value from ADC: " + std::to_string(heightRaw));
				Logger::debug("[HM] Height in mm: " + std::to_string(msg.value.sival_int));
				this_thread::sleep_for(chrono::milliseconds(100));
				adc->sample();
			}

			// Do not ignore OS pulses!
		}
	}

	std::cout << "ADC thread stops..." << std::endl;
}
