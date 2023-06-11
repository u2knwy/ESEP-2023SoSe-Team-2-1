/*
 * MainActions.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "MainActions.h"
#include "logger/logger.hpp"
#include "hal/HeightSensor.h"
#include "configuration/Configuration.h"
#include <thread>
#include <chrono>

#define HM_CAL_MEASUREMENTS 10

MainActions::MainActions(std::shared_ptr<EventManager> mngr) {
	this->eventManager = mngr;
}

MainActions::~MainActions() {
}


void MainActions::setMotorStopMaster(bool stop) {
	Event event;
	event.type = EventType::MOTOR_M_STOP_REQ;
	event.data = (int) stop;
	eventManager->sendEvent(event);
}

void MainActions::setMotorFastMaster(bool fast) {
	Event event;
	event.type = EventType::MOTOR_M_RIGHT_REQ;
	event.data = (int) fast;
	eventManager->sendEvent(event);
}

void MainActions::setMotorSlowMaster(bool slow) {
	Event event;
	event.type = EventType::MOTOR_M_SLOW_REQ;
	event.data = (int) slow;
	eventManager->sendEvent(event);
}

void MainActions::setMotorStopSlave(bool stop) {
	Event event;
	event.type = EventType::MOTOR_S_STOP_REQ;
	event.data = (int) stop;
	eventManager->sendEvent(event);
}

void MainActions::setMotorFastSlave(bool fast) {
	Event event;
	event.type = EventType::MOTOR_S_RIGHT_REQ;
	event.data = (int) fast;
	eventManager->sendEvent(event);
}

void MainActions::setMotorSlowSlave(bool slow) {
	Event event;
	event.type = EventType::MOTOR_S_SLOW_REQ;
	event.data = (int) slow;
	eventManager->sendEvent(event);
}

void MainActions::setStandbyMode() {
	Event event;
	event.type = EventType::MODE_STANDBY;
	eventManager->sendEvent(event);
}

void MainActions::setRunningMode() {
	Event event;
	event.type = EventType::MODE_RUNNING;
	eventManager->sendEvent(event);
}

void MainActions::setServiceMode() {
	Event event;
	event.type = EventType::MODE_SERVICE;
	eventManager->sendEvent(event);
}

void MainActions::setErrorMode() {
	Event event;
	event.type = EventType::MODE_ERROR;
	eventManager->sendEvent(event);
}

void MainActions::setEStopMode() {
	Event event;
	event.type = EventType::MODE_ESTOP;
	eventManager->sendEvent(event);
}

void MainActions::allActuatorsOn() {

}

void MainActions::allActuatorsOff() {
}

void MainActions::displayWarning() {
	Event event;
	event.data = 1;

	event.type = EventType::WARNING_M;
	eventManager->sendEvent(event);

	event.type = EventType::WARNING_S;
	eventManager->sendEvent(event);
}

void MainActions::warningOff() {
	Event event;
	event.data = 0;

	event.type = EventType::WARNING_M;
	eventManager->sendEvent(event);

	event.type = EventType::WARNING_S;
	eventManager->sendEvent(event);
}

void MainActions::calibrateOffset() {
	Logger::info("Calibrating HeightSensor offset...");
	HeightSensor sensor;
	sensor.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	int sum = 0;
	for(int i = 0; i < HM_CAL_MEASUREMENTS; i++) {
		sum += sensor.getLastRawValue();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	int offset = sum / HM_CAL_MEASUREMENTS;
	Logger::info("Calibrating HeightSensor offset -> value = " + std::to_string(offset));
	Configuration::getInstance().setOffsetCalibration(offset);
}

void MainActions::calibrateReference() {
	Logger::info("Calibrating HeightSensor reference (high)...");
	HeightSensor sensor;
	sensor.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	int sum = 0;
	for(int i = 0; i < HM_CAL_MEASUREMENTS; i++) {
		sum += sensor.getLastRawValue();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	int ref = sum / HM_CAL_MEASUREMENTS;
	Logger::info("Calibrating HeightSensor reference (high) -> value = " + std::to_string(ref));
	Configuration::getInstance().setReferenceCalibration(ref);
}

void MainActions::saveCalibration() {
	Configuration& conf = Configuration::getInstance();
	conf.saveCurrentConfigToFile();
	Calibration cal = conf.getCalibration();
	std::stringstream ss;
	ss << "HeightSensor calibration: CAL_OFFSET=" << cal.calOffset;
	ss << "; CAL_REF=" << cal.calRef;
	ss << " -> saved to file!" << std::endl;
}

void MainActions::btnStartLedOn() {
	Event ev;
	ev.data = 1;
	ev.type = EventType::LED_M_START;
	eventManager->sendEvent(ev);
	ev.type = EventType::LED_S_START;
	eventManager->sendEvent(ev);
}

void MainActions::btnResetLedOn() {
	Event ev;
	ev.data = 1;
	ev.type = EventType::LED_M_RESET;
	eventManager->sendEvent(ev);
	ev.type = EventType::LED_S_RESET;
	eventManager->sendEvent(ev);
}

void MainActions::btnStartLedOff() {
	Event ev;
	ev.data = 0;
	ev.type = EventType::LED_M_START;
	eventManager->sendEvent(ev);
	ev.type = EventType::LED_S_START;
	eventManager->sendEvent(ev);
}

void MainActions::btnResetLedOff() {
	Event ev;
	ev.data = 0;
	ev.type = EventType::LED_M_RESET;
	eventManager->sendEvent(ev);
	ev.type = EventType::LED_S_RESET;
	eventManager->sendEvent(ev);
}

