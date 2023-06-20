/*
 * MotorActions.cpp
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */

#include "MotorActions.h"
#include "configuration/Configuration.h"
#include "logger/logger.hpp"


MotorActions::MotorActions(std::shared_ptr<EventManager> mngr, bool master) {
    this->eventManager = mngr;
    this->isMaster = master;
    if (connect(eventManager)) {
        Logger::debug("[MotorActions] Connected to EventManager");
    } else {
        Logger::error("[MotorActions] Error while connecting to EventManager");
    }
}

MotorActions::~MotorActions() { disconnect(); }

void MotorActions::motorStop() {
    Event ev;
	if(isMaster) {
		Logger::debug("[MotorFSM_M] Motor stop");
		ev.type = EventType::MOTOR_M_STOP;
	} else {
		Logger::debug("[MotorFSM_S] Motor stop");
		ev.type = EventType::MOTOR_S_STOP;
	}
    sendEvent(ev);
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
    sendEvent(ev);
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
    sendEvent(ev);
}
