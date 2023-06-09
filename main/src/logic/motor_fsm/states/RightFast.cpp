/*
 * RightFast.cpp
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */

#include "RightFast.h"
#include "Stopped.h"
#include "RightSlow.h"
#include "logger/logger.hpp"

void RightFast::entry() {
	Logger::debug("[MotorFSM] RightFast entry");
	actions->motorRightFast();
}

void RightFast::exit() {

}

MotorState RightFast::getCurrentState() {
	return MotorState::RIGHT_FAST;
};

bool RightFast::handleFlagsUpdated() {
	std::stringstream ss;
	if(data->getStop() || (!data->getFast() && !data->getSlow())) {
		exit();
		new(this) Stopped;
		entry();
		return true;
	} else if(!data->getStop() && data->getSlow()) {
		exit();
		new(this) RightSlow;
		entry();
		return true;
	}
	return false;
}
