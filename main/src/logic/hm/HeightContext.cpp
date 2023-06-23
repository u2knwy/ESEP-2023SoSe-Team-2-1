/*
 * HeightSensorFSM.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include <logic/hm/HeightContext.h>

#include "HeightActions.h"
#include "hal/IHeightSensor.h"
#include "states/WaitForWorkpiece.h"

HeightContext::HeightContext(HeightActions* actions, HeightContextData* data, std::shared_ptr<IHeightSensor> heightSensor) {
	this->isMaster = Configuration::getInstance().systemIsMaster();
    this->actions = actions;
    this->sensor = heightSensor;
    this->data = data;
    state = new WaitForWorkpiece();
    state->setData(data);
    state->setAction(actions);
    state->entry();
    running = false;
    subscribeToEvents();
    sensor->registerOnNewValueCallback(std::bind(
        &HeightContext::heightValueReceived, this, std::placeholders::_1));
    sensor->start();
}

HeightContext::~HeightContext() {
    if (sensor != nullptr) {
        sensor->stop();
    }
    delete state;
    delete actions;
    delete data;
}

void HeightContext::subscribeToEvents() {
	if(isMaster) {
	    actions->eventManager->subscribe(
	        EventType::MOTOR_M_FAST,
	        std::bind(&HeightContext::handleEvent, this, std::placeholders::_1));
	    actions->eventManager->subscribe(
	        EventType::MOTOR_M_SLOW,
	        std::bind(&HeightContext::handleEvent, this, std::placeholders::_1));
	    actions->eventManager->subscribe(
	        EventType::MOTOR_M_STOP,
	        std::bind(&HeightContext::handleEvent, this, std::placeholders::_1));
	} else {
	    actions->eventManager->subscribe(
	        EventType::MOTOR_S_FAST,
	        std::bind(&HeightContext::handleEvent, this, std::placeholders::_1));
	    actions->eventManager->subscribe(
	        EventType::MOTOR_S_SLOW,
	        std::bind(&HeightContext::handleEvent, this, std::placeholders::_1));
	    actions->eventManager->subscribe(
	        EventType::MOTOR_S_STOP,
	        std::bind(&HeightContext::handleEvent, this, std::placeholders::_1));
	}
}

void HeightContext::handleEvent(Event event) {
    Logger::debug("[HFSM] handleEvent: " + EVENT_TO_STRING(event.type));
    switch (event.type) {
    case EventType::MOTOR_M_FAST:
    case EventType::MOTOR_S_FAST:
    case EventType::MOTOR_M_SLOW:
    case EventType::MOTOR_S_SLOW: {
    	Logger::debug("[HM] Motor running -> start measurement");
        this->running = true;
        // sensor->start();
        break;
    }
    case EventType::MOTOR_M_STOP:
    case EventType::MOTOR_S_STOP: {
    	Logger::debug("[HM] Motor stopped -> stop measurement");
        this->running = false;
        // sensor->stop();
        break;
    }
    default:
        Logger::warn("[HFSM] Event was not handled: " +
                     EVENT_TO_STRING(event.type));
    }
}

void HeightContext::heightValueReceived(float valueMM) {
    // Handle new value only if motor is running
    if (running) {
        if (getCurrentState() != HeightState::WAIT_FOR_WS) {
            data->addValue(valueMM);
            //Logger::debug("[HM] New value: " + std::to_string(valueMM));
        }

        bool handled;
        if (valueMM > HEIGHT_FLAT - HEIGHT_TOL &&
            valueMM < HEIGHT_FLAT + HEIGHT_TOL) {
            handled = state->flatDetected();
        } else if (valueMM > HEIGHT_HIGH - HEIGHT_TOL &&
                   valueMM < HEIGHT_HIGH + HEIGHT_TOL) {
            handled = state->highDetected();
        } else if (valueMM < HEIGHT_CONV_MAX) {
            handled = state->beltDetected();
        } else if (valueMM >= HEIGHT_HOLE_MIN &&
                   valueMM <= HEIGHT_HOLE_MAX) {
            handled = state->holeDetected();
        } else {
            handled = state->unknownDetected();
        }
    }
}

HeightResult HeightContext::getCurrentResult() {
    return data->getCurrentResult();
}

HeightState HeightContext::getCurrentState() {
    return state->getCurrentState();
}
