/*
 * MotorActions.h
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */
#pragma once

#include "events/EventManager.h"
#include <memory>

class MotorActions {
public:
	MotorActions(std::shared_ptr<EventManager> mngr, bool master);
	virtual ~MotorActions();
	void motorStop();
	void motorRightFast();
	void motorRightSlow();
private:
	std::shared_ptr<EventManager> eventManager;
	bool isMaster;
};

