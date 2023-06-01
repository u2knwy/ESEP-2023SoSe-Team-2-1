/*
 * MainActions.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "MainActions.h"

MainActions::MainActions(std::shared_ptr<EventManager> mngr) {
	// TODO Auto-generated constructor stub

}

MainActions::~MainActions() {
	// TODO Auto-generated destructor stub
}


void MainActions::setMotorStop(bool stop) {
	Event event;
	event.type = EventType::HALmotorStop;
	event.data = (int) stop;
	eventManager->sendEvent(event);
}

void MainActions::setMotorFast(bool fast) {
	Event event;
	event.type = EventType::HALmotorFastRight;
	event.data = (int) fast;
	eventManager->sendEvent(event);
}

void MainActions::setMotorSlow(bool slow) {
	Event event;
	event.type = EventType::HALmotorSlowRight;
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
