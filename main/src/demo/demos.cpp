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
#include "logger/logger.hpp"
#include "hal/hal.h"
#include "hal/Sensors.h"
#include "hal/Actuators.h"
#include "hal/HeightSensor.h"
#include "logic/hm/HeightContext.h"
#include "logic/main_fsm/MainContext.h"
#include "events/EventManager.h"

void actuatorDemo() {
	Logger::info("Actuator Demo");

	// Create a shared pointer which holds a reference to the HAL.
	// This can be shared across multiple classes and the destructor will automatically be called,
	// if there are no more references to it.
	std::shared_ptr<EventManager> mngr = std::make_shared<EventManager>();
	std::shared_ptr<Actuators> actuators = std::make_shared<Actuators>(mngr);

	actuators->motorStop(true);

	// ALL ON
	Logger::info("Switch all actuators on");
	actuators->greenLampOn();
	actuators->yellowLampOn();
	actuators->redLampOn();
	actuators->startLedOn();
	actuators->resetLedOn();
	actuators->q1LedOn();
	actuators->q2LedOn();
	actuators->openSwitch();
	std::this_thread::sleep_for(std::chrono::seconds(5));

	// ALL OFF
	Logger::info("Switch all actuators off");
	actuators->greenLampOff();
	actuators->yellowLampOff();
	actuators->redLampOff();
	actuators->startLedOff();
	actuators->resetLedOff();
	actuators->q1LedOff();
	actuators->q2LedOff();
	actuators->closeSwitch();
	std::this_thread::sleep_for(std::chrono::seconds(5));

	actuators->motorStop(false);

	actuators->motorRight();
	Logger::info("Motor right fast");
	actuators->motorSlow(false);
	std::this_thread::sleep_for(std::chrono::seconds(2));

	Logger::info("Motor right slow");
	actuators->motorSlow(true);
	std::this_thread::sleep_for(std::chrono::seconds(2));

	actuators->motorLeft();
	Logger::info("Motor left fast");
	actuators->motorSlow(false);
	std::this_thread::sleep_for(std::chrono::seconds(2));

	Logger::info("Motor left slow");
	actuators->motorSlow(true);
	std::this_thread::sleep_for(std::chrono::seconds(2));

	actuators->motorStop(true);
}

void sensorDemo() {
	Logger::info("Sensor Demo");

	std::shared_ptr<EventManager> mngr = std::make_shared<EventManager>();
	std::shared_ptr<Sensors> sensors = std::make_shared<Sensors>(mngr);
	sensors->startEventLoop();
	std::this_thread::sleep_for(std::chrono::seconds(999));
	sensors->stopEventLoop();

	Logger::info("Stop Sensor Demo");
}

void adcDemo() {
	using namespace std;

	std::shared_ptr<EventManager> mngr = std::make_shared<EventManager>();
	std::shared_ptr<Actuators> actuators = std::make_shared<Actuators>(mngr);

	// Start motor slow for demo
	//hal->motorRight();
	//hal->motorSlow();

	Configuration& conf = Configuration::getInstance();
	conf.saveCalibration(3648, 2323);

	// HeightSensor with FSM
	std::shared_ptr<HeightSensor> heightSensor = std::make_shared<HeightSensor>();
	std::shared_ptr<HeightContext> heightFSM = std::make_shared<HeightContext>(mngr, heightSensor);
	HeightSensor hm;
	hm.start();

#define DEMO_DURATION 5
	cout << "Demo time for " << DEMO_DURATION << " seconds..." << endl;
	//this_thread::sleep_for(chrono::seconds(DEMO_DURATION));
	for(int i = 0; i < 10; i++) {
		float avg = hm.getAverageHeight();
		std::ostringstream oss;
		oss << std::fixed << std::setprecision(2) << avg;
		Logger::info("AVG Height: " + oss.str() + " mm");
		this_thread::sleep_for(chrono::seconds(1));
	}
	cout << "Stopping in..." << endl;
	cout << "3" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "2" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "1" << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "NOW!" << endl;

	hm.stop();

	// Stop motor for demo
	actuators->motorStop(true);
	// ### END ADC TEST
}

void fsmDemo() {
	std::shared_ptr<EventManager> mngr = std::make_shared<EventManager>();
	MainContext fsm(mngr);
	fsm.master_EStop_Pressed();
	fsm.slave_EStop_Pressed();
	fsm.master_EStop_Released();
	fsm.slave_EStop_Released();
	fsm.master_btnReset_Pressed();
	fsm.slave_btnReset_Pressed();
}
