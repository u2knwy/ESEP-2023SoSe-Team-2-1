/*
 * demos.cpp
 *
 *  Created on: 18.04.2023
 *      Author: Maik
 */
#include "demos.h"
#include <iostream>
#include <ostream>
#include <chrono>
#include <thread>
#include "../hal/hal.h"

void actuatorDemo() {
	std::cout << "Actuator Demo" << std::endl;

	// Create a shared pointer which holds a reference to the HAL.
	// This can be shared across multiple classes and the destructor will automatically be called,
	// if there are no more references to it.
	std::shared_ptr<HAL> hal = std::make_shared<HAL>();

	hal->motorStop();

	// ALL ON
	std::cout << "Switch all actuators on" << std::endl;
	hal->GreenLampOn();
	hal->YellowLampOn();
	hal->RedLampOn();
	hal->StartLedOn();
	hal->ResetLedOn();
	hal->Q1LedOn();
	hal->Q2LedOn();
	hal->openSwitch();
	std::this_thread::sleep_for(std::chrono::seconds(5));

	// ALL OFF
	std::cout << "Switch all actuators off" << std::endl;
	hal->GreenLampOff();
	hal->YellowLampOff();
	hal->RedLampOff();
	hal->StartLedOff();
	hal->ResetLedOff();
	hal->Q1LedOff();
	hal->Q2LedOff();
	hal->closeSwitch();
	std::this_thread::sleep_for(std::chrono::seconds(5));

	std::cout << "Motor right fast" << std::endl;
	hal->motorFast();
	hal->motorRight();
	std::this_thread::sleep_for(std::chrono::seconds(2));

	std::cout << "Motor right slow" << std::endl;
	hal->motorSlow();
	hal->motorRight();
	std::this_thread::sleep_for(std::chrono::seconds(2));

	std::cout << "Motor left fast" << std::endl;
	hal->motorFast();
	hal->motorLeft();
	std::this_thread::sleep_for(std::chrono::seconds(2));

	std::cout << "Motor left slow" << std::endl;
	hal->motorSlow();
	hal->motorLeft();
	std::this_thread::sleep_for(std::chrono::seconds(2));

	hal->motorStop();
}

void sensorDemo() {
	std::cout << "Sensor Demo" << std::endl;

	std::shared_ptr<HAL> hal = std::make_shared<HAL>();
	std::this_thread::sleep_for(std::chrono::seconds(999));

	std::cout << "Stop Sensor Demo" << std::endl;
}

void adcDemo() {
	using namespace std;

	std::shared_ptr<HAL> hal = std::make_shared<HAL>();

	// Start motor slow for demo
	hal->motorRight();
	hal->motorSlow();

	hal->startEventLoop();
	hal->startHeightMeasurement();

#define DEMO_DURATION 5
	cout << "Demo time for " << DEMO_DURATION << " seconds..." << endl;
	this_thread::sleep_for(chrono::seconds(DEMO_DURATION));
	cout << "Stopping in..." << endl;
	cout << "3" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "2" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "1" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "NOW!" << endl;

	hal->stopEventLoop();
	hal->stopHeightMeasurement();

	// Stop motor for demo
	hal->motorStop();
	// ### END ADC TEST
}
