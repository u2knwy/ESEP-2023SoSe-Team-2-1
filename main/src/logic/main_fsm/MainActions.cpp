/*
 * MainActions.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "MainActions.h"
#include "logger/logger.hpp"

MainActions::MainActions(std::shared_ptr<EventManager> mngr) {
	this->eventManager = mngr;
}

MainActions::~MainActions() {
}


void MainActions::setMotorStop(bool stop) {
	Logger::debug("MainActions::setMotorStop " + std::to_string(stop));
	Event event;
	event.type = EventType::HALmotorStop;
	event.data = (int) stop;
	eventManager->sendEvent(event);
}

void MainActions::setMotorFast(bool fast) {
	Logger::debug("MainActions::setMotorFast " + std::to_string(fast));
	Event event;
	event.type = EventType::HALmotorFastRight;
	event.data = (int) fast;
	eventManager->sendEvent(event);
}

void MainActions::setMotorSlow(bool slow) {
	Logger::debug("MainActions::setMotorSlow " + std::to_string(slow));
	Event event;
	event.type = EventType::HALmotorSlowRight;
	event.data = (int) slow;
	eventManager->sendEvent(event);
}

void MainActions::setStandbyMode() {
	Logger::debug("MainActions::setStandbyMode");
	Event event;
	event.type = EventType::MODE_STANDBY;
	eventManager->sendEvent(event);
}

void MainActions::setRunningMode() {
	Logger::debug("MainActions::setRunningMode");
	Event event;
	event.type = EventType::MODE_RUNNING;
	eventManager->sendEvent(event);
}

void MainActions::setServiceMode() {
	Logger::debug("MainActions::setServiceMode");
	Event event;
	event.type = EventType::MODE_SERVICE;
	eventManager->sendEvent(event);
}

void MainActions::setErrorMode() {
	Logger::debug("MainActions::setErrorMode");
	Event event;
	event.type = EventType::MODE_ERROR;
	eventManager->sendEvent(event);
}

void MainActions::setEStopMode() {
	Logger::debug("MainActions::setEStopMode");
	Event event;
	event.type = EventType::MODE_ESTOP;
	eventManager->sendEvent(event);
}

void MainActions::allActuatorsOn() {
	Logger::debug("MainActions::allActuatorsOn");
}

void MainActions::displayWarning() {
	Event event;

	event.type = EventType::WARNING_M;
	eventManager->sendEvent(event);

	event.type = EventType::WARNING_S;
	eventManager->sendEvent(event);
}
