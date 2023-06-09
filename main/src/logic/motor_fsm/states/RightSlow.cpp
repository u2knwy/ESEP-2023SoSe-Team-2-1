/*
 * RightSlow.cpp
 *
 *  Created on: 09.06.2023
 *      Author: Maik
 */

#include "RightSlow.h"
#include "Stopped.h"
#include "RightFast.h"
#include "logger/logger.hpp"

void RightSlow::entry() {
	Logger::debug("[MotorFSM] Stopped entry");
	actions->motorRightSlow();
}

void RightSlow::exit() {

}

MotorState RightSlow::getCurrentState() {
	return MotorState::RIGHT_SLOW;
};

bool RightSlow::handleFlagsUpdated() {
	if(!data->getStop() && data->getFast() && !data->getSlow()) {
		exit();
		new(this) RightFast;
		entry();
		return true;
	} else if(data->getStop() || (!data->getFast() && !data->getSlow())) {
		exit();
		new(this) Stopped;
		entry();
		return true;
	}
	return true;
}
