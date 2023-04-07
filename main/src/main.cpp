#include <iostream>
#include <thread>
#include "hal/hal.h"

using namespace std;

int main() {

	// Create a shared pointer which holds a reference to the HAL.
	// This can be shared across multiple classes and the destructor will automatically be called,
	// if there are no more references to it.
	shared_ptr<HAL> hal = std::make_shared<HAL>();

#define RUNS 10
	cout << "Switch all lights on / off" << endl;
	for(int i = 0; i < RUNS; i++){
		cout << "loop " << i+1 << "/" << RUNS << endl;

		// ALL ON
		hal->GreenLampOn();
		hal->YellowLampOn();
		hal->RedLampOn();
		hal->StartLedOn();
		hal->ResetLedOn();
		hal->Q1LedOn();
		hal->Q2LedOn();
		this_thread::sleep_for(chrono::seconds(1));

		// ALL OFF
        hal->GreenLampOff();
		hal->YellowLampOff();
		hal->RedLampOff();
		hal->StartLedOff();
		hal->ResetLedOff();
		hal->Q1LedOff();
		hal->Q2LedOff();
        this_thread::sleep_for(chrono::seconds(1));
	}

	cout << "Motor right fast" << endl;
	hal->motorFast();
	hal->motorRight();
    this_thread::sleep_for(chrono::seconds(5));

	cout << "Motor right slow" << endl;
	hal->motorSlow();
	hal->motorRight();
    this_thread::sleep_for(chrono::seconds(5));

	cout << "Motor left fast" << endl;
	hal->motorFast();
	hal->motorLeft();
    this_thread::sleep_for(chrono::seconds(5));

	cout << "Motor left slow" << endl;
	hal->motorSlow();
	hal->motorLeft();
    this_thread::sleep_for(chrono::seconds(5));

    cout << "Starting ADC demo" << endl;
    hal->adcDemo();

    cout << "Starting GPIO demo" << endl;
    hal->gpioDemo();

	return 0;
}
