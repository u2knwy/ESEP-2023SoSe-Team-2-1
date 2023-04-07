/*
 * hal.cpp
 *
 *  Created on: 31.03.2023
 *      Author: Maik
 */

#include "hal.h"
#include <string>

HAL::HAL() {
	std::cout << "Create HAL" << std::endl;
	gpio_bank_1 = mmap_device_io(SIZE, (uint64_t) GPIO_BANK_1);

	TSCADC tsc;
	adc = new ADC(tsc);

	receivingRunning = false;
}

HAL::~HAL() {
	std::cout << "Destroy HAL" << std::endl;
	munmap_device_io(gpio_bank_1, SIZE);

	delete adc;
}

void HAL::GreenLampOn() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_SETDATAOUT), LAMP_GREEN_PIN); // set register
}

void HAL::GreenLampOff() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), LAMP_GREEN_PIN); // clear register
}

void HAL::motorSlow() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), MOTOR_LEFT_PIN);
	out32((uintptr_t) (gpio_bank_1 + GPIO_SETDATAOUT), MOTOR_RIGHT_PIN);
	out32((uintptr_t) (gpio_bank_1 + GPIO_SETDATAOUT), MOTOR_SLOW_PIN);
}

void HAL::motorFast() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), MOTOR_LEFT_PIN);
	out32((uintptr_t) (gpio_bank_1 + GPIO_SETDATAOUT), MOTOR_RIGHT_PIN);
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), MOTOR_SLOW_PIN);
}

void HAL::motorStop() {
	out32((uintptr_t) (gpio_bank_1 + GPIO_SETDATAOUT), MOTOR_STOP_PIN);
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), MOTOR_LEFT_PIN);
	out32((uintptr_t) (gpio_bank_1 + GPIO_CLEARDATAOUT), MOTOR_RIGHT_PIN);
}

void HAL::heightReceivingRoutine(int channelID) {
	ThreadCtl(_NTO_TCTL_IO, 0); // Request IO privileges for this thread.

	_pulse msg;
	receivingRunning = true;

	using namespace std;

	cout << "Receiving routine started." << endl;

	while (receivingRunning) {
		int recvid = MsgReceivePulse(channelID, &msg, sizeof(_pulse), nullptr);

		if (recvid < 0) {
			perror("MsgReceivePulse failed!");
			exit(EXIT_FAILURE);
		}

		if (recvid == 0) { // pulse received.

			// Stop thread while it blocks.
			if (msg.code == PULSE_STOP_THREAD) {
				cout << "Thread kill code received!" << endl;
				receivingRunning = false;
				continue;
			}

			// ADC interrupt value.
			if (msg.code == PULSE_ADC_SAMPLING_DONE) {
				cout << "Value from adc with value " << msg.value.sival_int << endl;
				adc->sample();
			}

			// Do not ignore OS pulses!
		}
	}

	cout << "Message thread stops..." << endl;
}

void HAL::measureHeight() {
	using namespace std;

	// Start motor slow for demo
	motorSlow();

	// ### START ADC TEST
	int chanID = ChannelCreate(0); // Create channel to receive interrupt pulse messages.
	if (chanID < 0) {
		perror("Could not create a channel!\n");
	}

	int conID = ConnectAttach(0, 0, chanID, _NTO_SIDE_CHANNEL, 0); // Connect to channel.
	if (conID < 0) {
		perror("Could not connect to channel!");
	}

	adc->registerAdcISR(conID, PULSE_ADC_SAMPLING_DONE);

	// ### Start thread for handling interrupt messages.
	thread receivingThread(&HAL::heightReceivingRoutine, this, chanID);

	adc->sample();

	cout << "Demo time for " << DEMO_DURATION << " minutes..." << endl;
	this_thread::sleep_for(chrono::seconds(DEMO_DURATION));
	cout << "Stopping in..." << endl;
	cout << "3" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "2" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "1" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "NOW!" << endl;

	// ### Cleaning up ###

	// Detach interrupts.
	adc->adcDisable();
	adc->unregisterAdcISR();

	// Stop receiving thread.
	MsgSendPulse(conID, -1, PULSE_STOP_THREAD, 0); // using prio of calling thread.
	receivingThread.join();

	// Close channel
	int detach_status = ConnectDetach(conID);
	if (detach_status != EOK) {
		perror("Detaching channel failed!");
		exit(EXIT_FAILURE);
	}

	int destroy_status = ChannelDestroy(chanID);
	if (destroy_status != EOK) {
		perror("Destroying channel failed!");
		exit(EXIT_FAILURE);
	}

	// Stop motor for demo
	motorStop();
	// ### END ADC TEST
}
