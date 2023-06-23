/*
 * MotorActions.cpp
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */

#include "MotorActions.h"
#include "configuration/Configuration.h"
#include "logger/logger.hpp"


MotorActions::MotorActions(std::shared_ptr<IEventManager> mngr, IEventSender* eventSender, bool master) {
    this->eventManager = mngr;
    this->eventSender = eventSender;
    this->isMaster = master;
    if (eventSender->connect(eventManager)) {
        Logger::debug("[MotorActions] Connected to EventManager");
    } else {
        Logger::error("[MotorActions] Error while connecting to EventManager");
    }
}

MotorActions::~MotorActions() {
	eventSender->disconnect();
	delete eventSender;
}

void MotorActions::motorStop() {
    Event ev;
	if(isMaster) {
		Logger::debug("[MotorFSM_M] Motor stop");
		ev.type = EventType::MOTOR_M_STOP;
	} else {
		Logger::debug("[MotorFSM_S] Motor stop");
		ev.type = EventType::MOTOR_S_STOP;
	}
    eventSender->sendEvent(ev);
}

void MotorActions::motorRightFast() {
    Event ev;
	if(isMaster) {
		Logger::debug("[MotorFSM_M] Motor right fast");
		ev.type = EventType::MOTOR_M_FAST;
	} else {
		Logger::debug("[MotorFSM_S] Motor right fast");
		ev.type = EventType::MOTOR_S_FAST;
	}
    eventSender->sendEvent(ev);
}

void MotorActions::motorRightSlow() {
    Event ev;
	if(isMaster) {
		Logger::debug("[MotorFSM_M] Motor right slow");
		ev.type = EventType::MOTOR_M_SLOW;
	} else {
		Logger::debug("[MotorFSM_S] Motor right slow");
		ev.type = EventType::MOTOR_S_SLOW;
	}
    eventSender->sendEvent(ev);
}
