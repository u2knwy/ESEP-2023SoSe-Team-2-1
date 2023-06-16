/*
 * MotorContext.cpp
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */

#include "MotorContext.h"
#include "MotorActions.h"
#include "MotorContextData.h"
#include "states/Stopped.h"

#include "configuration/Configuration.h"
#include "logger/logger.hpp"

MotorContext::MotorContext(std::shared_ptr<EventManager> mngr, bool master) {
	isMaster = Configuration::getInstance().systemIsMaster();
	this->eventManager = mngr;
	this->actions = new MotorActions(mngr, master);
	this->data = new MotorContextData();
	this->state = new Stopped();
	state->setAction(actions);
	state->setData(data);
	state->entry();
	subscribeToEvents();
}

MotorContext::~MotorContext() {
	delete actions;
	delete data;
	delete state;
}

MotorState MotorContext::getCurrentState() {
	return state->getCurrentState();
}

void MotorContext::subscribeToEvents() {
	if(isMaster) {
		eventManager->subscribe(EventType::MOTOR_M_STOP_REQ, std::bind(&MotorContext::handleEvent, this, std::placeholders::_1));
		eventManager->subscribe(EventType::MOTOR_M_SLOW_REQ, std::bind(&MotorContext::handleEvent, this, std::placeholders::_1));
		eventManager->subscribe(EventType::MOTOR_M_RIGHT_REQ, std::bind(&MotorContext::handleEvent, this, std::placeholders::_1));
	} else {
		eventManager->subscribe(EventType::MOTOR_S_STOP_REQ, std::bind(&MotorContext::handleEvent, this, std::placeholders::_1));
		eventManager->subscribe(EventType::MOTOR_S_SLOW_REQ, std::bind(&MotorContext::handleEvent, this, std::placeholders::_1));
		eventManager->subscribe(EventType::MOTOR_S_RIGHT_REQ, std::bind(&MotorContext::handleEvent, this, std::placeholders::_1));
	}
}

void MotorContext::handleEvent(Event event) {
	Logger::debug("[MotorFSM] Event received: " + EVENT_TO_STRING(event.type));
	switch(event.type) {
	case EventType::MOTOR_M_STOP_REQ:
	case EventType::MOTOR_S_STOP_REQ:
		data->setStopFlag(event.data == 1);
		state->handleFlagsUpdated();
		break;
	case EventType::MOTOR_M_SLOW_REQ:
	case EventType::MOTOR_S_SLOW_REQ:
		data->setSlowFlag(event.data == 1);
		state->handleFlagsUpdated();
		break;
	case EventType::MOTOR_M_RIGHT_REQ:
	case EventType::MOTOR_S_RIGHT_REQ:
		data->setRightFlag(event.data == 1);
		state->handleFlagsUpdated();
		break;
	default:
		Logger::warn("[MotorFSM] Event was not handled by FSM -> " + EVENT_TO_STRING(event.type));
	}
}

