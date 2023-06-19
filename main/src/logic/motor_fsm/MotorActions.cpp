/*
 * MotorActions.cpp
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */

#include "MotorActions.h"
#include "logger/logger.hpp"
#include "configuration/Configuration.h"

MotorActions::MotorActions(std::shared_ptr<EventManager> mngr, bool master) {
	this->eventManager = mngr;
	this->isMaster = master;
	if(connect(eventManager)) {
		Logger::debug("[MotorActions] Connected to EventManager");
	} else {
		Logger::error("[MotorActions] Error while connecting to EventManager");
	}
}

MotorActions::~MotorActions() {
	disconnect();
}

void MotorActions::motorStop() {
	Logger::debug("[MotorFSM] Motor stop");
	Event ev;
	ev.type = isMaster ? EventType::MOTOR_M_STOP : EventType::MOTOR_S_STOP;
	sendEvent(ev);
}

void MotorActions::motorRightFast() {
	Logger::debug("[MotorFSM] Motor right fast");
	Event ev;
	ev.type = isMaster ? EventType::MOTOR_M_FAST : EventType::MOTOR_S_FAST;
	sendEvent(ev);
}

void MotorActions::motorRightSlow() {
	Logger::debug("[MotorFSM] Motor right slow");
	Event ev;
	ev.type = isMaster ? EventType::MOTOR_M_SLOW : EventType::MOTOR_S_SLOW;
	sendEvent(ev);
}
