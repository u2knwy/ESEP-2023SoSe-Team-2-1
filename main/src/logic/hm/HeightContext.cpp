/*
 * HeightSensorFSM.cpp
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#include <logic/hm/HeightContext.h>
#include "HeightActions.h"
#include "hal/HeightSensor.h"
#include "states/WaitForWorkpiece.h"

HeightContext::HeightContext(std::shared_ptr<EventManager> mngr, std::shared_ptr<IHeightSensor> sensor)
{
	this->eventManager = mngr;
	data = new HeightContextData();
	actions = new HeightActions(data, mngr);
	state = new WaitForWorkpiece();
	state->setData(data);
	state->setAction(actions);
	state->entry();
	running = false;
	subscribeToEvents();
	this->sensor = sensor;
	sensor->registerMeasurementCallback(std::bind(&HeightContext::heightValueReceived, this, std::placeholders::_1));
	sensor->start();
}

HeightContext::~HeightContext()
{
	if (sensor != nullptr)
	{
		sensor->stop();
	}
	delete data;
	delete actions;
	delete state;
}

void HeightContext::subscribeToEvents()
{
	eventManager->subscribe(EventType::MOTOR_M_FAST, std::bind(&HeightContext::handleEvent, this, std::placeholders::_1));
	eventManager->subscribe(EventType::MOTOR_S_FAST, std::bind(&HeightContext::handleEvent, this, std::placeholders::_1));
	eventManager->subscribe(EventType::MOTOR_M_SLOW, std::bind(&HeightContext::handleEvent, this, std::placeholders::_1));
	eventManager->subscribe(EventType::MOTOR_S_SLOW, std::bind(&HeightContext::handleEvent, this, std::placeholders::_1));
	eventManager->subscribe(EventType::MOTOR_M_STOP, std::bind(&HeightContext::handleEvent, this, std::placeholders::_1));
	eventManager->subscribe(EventType::MOTOR_S_STOP, std::bind(&HeightContext::handleEvent, this, std::placeholders::_1));

}

void HeightContext::handleEvent(Event event)
{
	Logger::debug("[HFSM] handleEvent: " + EVENT_TO_STRING(event.type));
	switch(event.type) {
	case EventType::MOTOR_M_FAST:
	case EventType::MOTOR_S_FAST:
	case EventType::MOTOR_M_SLOW:
	case EventType::MOTOR_S_SLOW:
		this->running = true;
		//sensor->start();
		break;
	case EventType::MOTOR_M_STOP:
	case EventType::MOTOR_S_STOP:
		this->running = false;
		//sensor->stop();
		break;
	default:
		Logger::warn("[HFSM] Event was not handled: " + EVENT_TO_STRING(event.type));
	}
}

void HeightContext::heightValueReceived(float valueMM) {
	// Handle new value only if motor is running
	if (running)
	{
		if(getCurrentState() != HeightState::WAIT_FOR_WS) {
			data->updateAvgAndMaxValue(valueMM);
		}

		bool handled;
		if (valueMM > HEIGHT_FLAT - HEIGHT_TOL && valueMM < HEIGHT_FLAT + HEIGHT_TOL)
		{
			handled = state->flatDetected();
		}
		else if (valueMM > HEIGHT_HIGH - HEIGHT_TOL && valueMM < HEIGHT_HIGH + HEIGHT_TOL)
		{
			handled = state->highDetected();
		}
		else if (valueMM < HEIGHT_CONV_MAX)
		{
			handled = state->beltDetected();
		}
		else if (valueMM > HEIGHT_HOLE - HEIGHT_TOL && valueMM < HEIGHT_HOLE + HEIGHT_TOL)
		{
			handled = state->holeDetected();
		}
		else
		{
			handled = state->unknownDetected();
		}

	}
}

HeightResult HeightContext::getCurrentResult()
{
	return data->getCurrentResult();
}

HeightState HeightContext::getCurrentState()
{
	return state->getCurrentState();
}
