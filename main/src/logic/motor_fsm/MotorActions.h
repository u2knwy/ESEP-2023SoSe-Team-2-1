/*
 * MotorActions.h
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */
#pragma once

#include "events/EventManager.h"
#include "events/IEventSender.h"
#include <memory>

class MotorActions {
public:
	MotorActions(std::shared_ptr<IEventManager> mngr, IEventSender* eventSender, bool master);
	virtual ~MotorActions();
	void motorStop();
	void motorRightFast();
	void motorRightSlow();

	std::shared_ptr<IEventManager> eventManager;
private:
	IEventSender* eventSender;
	bool isMaster;
};
