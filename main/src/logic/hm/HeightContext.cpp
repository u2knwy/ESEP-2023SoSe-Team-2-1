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

HeightContext::HeightContext(std::shared_ptr<EventManager> mngr, std::shared_ptr<IHeightSensor> sensor) {
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
}

HeightContext::~HeightContext() {
	if(sensor != nullptr) {
		sensor->stop();
	}
	delete data;
	delete actions;
	delete state;
}

void HeightContext::subscribeToEvents() {
	eventManager->subscribe(EventType::HALmotorFastRight, std::bind(&HeightContext::handleEvent, this, std::placeholders::_1));
	eventManager->subscribe(EventType::HALmotorSlowRight, std::bind(&HeightContext::handleEvent, this, std::placeholders::_1));
	eventManager->subscribe(EventType::HALmotorStop, std::bind(&HeightContext::handleEvent, this, std::placeholders::_1));

//	eventManager->subscribe(EventType::MODE_RUNNING, std::bind(&HeightContext::handleEvent, this, std::placeholders::_1));
//	eventManager->subscribe(EventType::MODE_SERVICE, std::bind(&HeightContext::handleEvent, this, std::placeholders::_1));
}

void HeightContext::handleEvent(Event event) {
	Logger::debug("[HFSM] handleEvent: " + EVENT_TO_STRING(event.type));
	if(event.type == EventType::HALmotorFastRight || event.type == EventType::HALmotorSlowRight) {
		this->running = true;
		sensor->start();
	} else if(event.type == EventType::HALmotorStop) {
		this->running = false;
		sensor->stop();
	} /*else if(event.type == EventType::MODE_RUNNING) {
		sensor = std::make_shared<HeightSensor>();
		this->running = true;
		sensor->start();
	} else if(event.type == EventType::MODE_SERVICE) {
		this->running = false;
		sensor->stop();
		sensor.reset();
	}*/ else {
		Logger::warn("Event " + EVENT_TO_STRING(event.type) + " was not handled by HeightFSM");
	}
}

void HeightContext::heightValueReceived(float valueMM) {
	// Handle new value only if motor is running
	if(running) {
		data->updateAvgAndMaxValue(valueMM);

		bool handled;
		if(valueMM > HEIGHT_FLAT-HEIGHT_TOL && valueMM < HEIGHT_FLAT+HEIGHT_TOL) {
			handled = state->flatDetected();
		} else if(valueMM > HEIGHT_HIGH-HEIGHT_TOL && valueMM < HEIGHT_HIGH+HEIGHT_TOL) {
			handled = state->highDetected();
		} else if(valueMM < HEIGHT_CONV_MAX) {
			handled = state->beltDetected();
		} else if(valueMM > HEIGHT_HOLE-HEIGHT_TOL && valueMM < HEIGHT_HOLE+HEIGHT_TOL) {
			handled = state->holeDetected();
		}
		/* TODO: We still have problems dealing with some "edge cases" at valid workpieces
		 *  else {
			handled = state->unknownDetected();
		}*/

		if(handled) {
			HeightResult res = data->getCurrentResult();
			std::stringstream ss;
			ss << "New value handled: " << std::setprecision(2) << valueMM << " mm -> type=" << res.type << ", avg=" << res.average << ", max=" << res.max << " mm";
			Logger::debug(ss.str());
		}
	}
}

HeightResult HeightContext::getCurrentResult() {
	return data->getCurrentResult();
}

HeightState HeightContext::getCurrentState() {
	return state->getCurrentState();
}
