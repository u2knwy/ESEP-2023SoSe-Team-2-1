/*
 * MainActions.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "MainActions.h"
#include "logger/logger.hpp"
#include "hal/HeightSensor.h"
#include "hal/IActuators.h"
#include "configuration/Configuration.h"
#include <thread>
#include <chrono>

#define HM_CAL_MEASUREMENTS 10

MainActions::MainActions(std::shared_ptr<EventManager> mngr) {
	this->eventManager = mngr;
}

MainActions::~MainActions() {
}


void MainActions::master_sendMotorStopRequest(bool stop) {
	Event event;
	event.type = EventType::MOTOR_M_STOP_REQ;
	event.data = (int) stop;
	eventManager->sendEvent(event);
}

void MainActions::master_sendMotorRightRequest(bool right) {
	Event event;
	event.type = EventType::MOTOR_M_RIGHT_REQ;
	event.data = (int) right;
	eventManager->sendEvent(event);
}

void MainActions::slave_sendMotorStopRequest(bool stop) {
	Event event;
	event.type = EventType::MOTOR_S_STOP_REQ;
	event.data = (int) stop;
	eventManager->sendEvent(event);
}

void MainActions::slave_sendMotorRightRequest(bool right) {
	Event event;
	event.type = EventType::MOTOR_S_RIGHT_REQ;
	event.data = (int) right;
	eventManager->sendEvent(event);
}

void MainActions::master_openGate(bool open) {
	// (EventData) 0: sort out, 1: open gate
	int eventData = open ? 0 : 1;
	eventManager->sendEvent(Event{SORT_M_OUT, eventData});
}

void MainActions::slave_openGate(bool open) {
	// (EventData) 0: sort out, 1: open gate
	int eventData = open ? 0 : 1;
	eventManager->sendEvent(Event{SORT_S_OUT, eventData});
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
	eventManager->sendEvent(Event{EventType::LAMP_M_GREEN, (int) LampState::ON});
	eventManager->sendEvent(Event{EventType::LAMP_M_YELLOW, (int) LampState::ON});
	eventManager->sendEvent(Event{EventType::LAMP_M_RED, (int) LampState::ON});
	eventManager->sendEvent(Event{EventType::LED_M_START, (int) LampState::ON});
	eventManager->sendEvent(Event{EventType::LED_M_RESET, (int) LampState::ON});
	eventManager->sendEvent(Event{EventType::LED_M_Q1, (int) LampState::ON});
	eventManager->sendEvent(Event{EventType::LED_M_Q2, (int) LampState::ON});

	eventManager->sendEvent(Event{EventType::LAMP_S_GREEN, (int) LampState::ON});
	eventManager->sendEvent(Event{EventType::LAMP_S_YELLOW, (int) LampState::ON});
	eventManager->sendEvent(Event{EventType::LAMP_S_RED, (int) LampState::ON});
	eventManager->sendEvent(Event{EventType::LED_S_START, (int) LampState::ON});
	eventManager->sendEvent(Event{EventType::LED_S_RESET, (int) LampState::ON});
	eventManager->sendEvent(Event{EventType::LED_S_Q1, (int) LampState::ON});
	eventManager->sendEvent(Event{EventType::LED_S_Q2, (int) LampState::ON});
}

void MainActions::allActuatorsOff() {
	eventManager->sendEvent(Event{EventType::LAMP_M_GREEN, (int) LampState::OFF});
	eventManager->sendEvent(Event{EventType::LAMP_M_YELLOW, (int) LampState::OFF});
	eventManager->sendEvent(Event{EventType::LAMP_M_RED, (int) LampState::OFF});
	eventManager->sendEvent(Event{EventType::LED_M_START, (int) LampState::OFF});
	eventManager->sendEvent(Event{EventType::LED_M_RESET, (int) LampState::OFF});
	eventManager->sendEvent(Event{EventType::LED_M_Q1, (int) LampState::OFF});
	eventManager->sendEvent(Event{EventType::LED_M_Q2, (int) LampState::OFF});

	eventManager->sendEvent(Event{EventType::LAMP_S_GREEN, (int) LampState::OFF});
	eventManager->sendEvent(Event{EventType::LAMP_S_YELLOW, (int) LampState::OFF});
	eventManager->sendEvent(Event{EventType::LAMP_S_RED, (int) LampState::OFF});
	eventManager->sendEvent(Event{EventType::LED_S_START, (int) LampState::OFF});
	eventManager->sendEvent(Event{EventType::LED_S_RESET, (int) LampState::OFF});
	eventManager->sendEvent(Event{EventType::LED_S_Q1, (int) LampState::OFF});
	eventManager->sendEvent(Event{EventType::LED_S_Q2, (int) LampState::OFF});
}

void MainActions::master_warningOn() {
	eventManager->sendEvent(Event{EventType::LAMP_M_YELLOW, (int) LampState::FLASHING_SLOW});
}

void MainActions::master_warningOff() {
	eventManager->sendEvent(Event{EventType::LAMP_M_YELLOW, (int) LampState::OFF});
}

void MainActions::slave_warningOn() {
	eventManager->sendEvent(Event{EventType::LAMP_S_YELLOW, (int) LampState::FLASHING_SLOW});
}

void MainActions::slave_warningOff() {
	eventManager->sendEvent(Event{EventType::LAMP_S_YELLOW, (int) LampState::OFF});
}

void MainActions::calibrateOffset() {
	Logger::info("Calibrating HeightSensor offset...");
	eventManager->sendEvent(Event{HM_M_CAL_OFFSET});
	eventManager->sendEvent(Event{HM_S_CAL_OFFSET});
}

void MainActions::calibrateReference() {
	Logger::info("Calibrating HeightSensor reference (high)...");
	eventManager->sendEvent(Event{HM_M_CAL_REF});
	eventManager->sendEvent(Event{HM_S_CAL_REF});
}

void MainActions::saveCalibration() {
	Configuration& conf = Configuration::getInstance();
	conf.saveCurrentConfigToFile();
	Calibration cal = conf.getCalibration();
	std::stringstream ss;
	ss << "HeightSensor calibration: CAL_OFFSET=" << cal.calOffset;
	ss << "; CAL_REF=" << cal.calRef;
	ss << " -> saved to file!" << std::endl;
	Logger::debug(ss.str());
}

void MainActions::btnStartLedOn() {
	eventManager->sendEvent(Event{EventType::LED_M_START, (int) LampState::ON});
	eventManager->sendEvent(Event{EventType::LED_S_START, (int) LampState::ON});
}

void MainActions::btnResetLedOn() {
	eventManager->sendEvent(Event{EventType::LED_M_RESET, (int) LampState::ON});
	eventManager->sendEvent(Event{EventType::LED_S_RESET, (int) LampState::ON});
}

void MainActions::btnStartLedOff() {
	eventManager->sendEvent(Event{EventType::LED_M_START, (int) LampState::OFF});
	eventManager->sendEvent(Event{EventType::LED_S_START, (int) LampState::OFF});
}

void MainActions::btnResetLedOff() {
	eventManager->sendEvent(Event{EventType::LED_M_RESET, (int) LampState::OFF});
	eventManager->sendEvent(Event{EventType::LED_S_RESET, (int) LampState::OFF});
}

