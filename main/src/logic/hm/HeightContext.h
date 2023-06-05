/*
 * HeightSensorFSM.h
 *
 *  Created on: 11.05.2023
 *      Author: Maik
 */

#pragma once

#include "HeightActions.h"
#include "HeightBasestate.h"
#include "HeightContextData.h"
#include "hal/IHeightSensor.h"
#include "events/IEventHandler.h"
#include "events/EventManager.h"
#include "events/events.h"

#include <memory>


class HeightContext : public IEventHandler {
public:
	HeightContext(std::shared_ptr<EventManager> mngr, std::shared_ptr<IHeightSensor> sensor);
	virtual ~HeightContext();
	HeightState getCurrentState();

	/**
	 * A new measurement from HeightSensor was received.
	 * This function handles the received value and updates the state of HeightSensorFSM
	 *
	 * @param valueMM Received value in mm
	 */
	void heightValueReceived(float valueMM);
	HeightResult getCurrentResult();
	void handleEvent(Event event) override;
private:
	std::shared_ptr<EventManager> eventManager;
	HeightActions* actions;
	HeightBasestate* state;
	HeightContextData* data;
	std::shared_ptr<IHeightSensor> sensor;
	bool running;
	void subscribeToEvents();
};