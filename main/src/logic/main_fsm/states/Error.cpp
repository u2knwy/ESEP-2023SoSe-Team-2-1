/*
 * Error.cpp
 *
 *  Created on: 14.06.2023
 *      Author: Maik
 */

#include "Error.h"

#include "Standby.h"
#include "Running.h"
#include "ServiceMode.h"
#include "SubErrorPendingUnresigned.h"
#include "logger/logger.hpp"

void Error::entry() {
	Logger::info("Error mode entered");
	initSubStateError();
	actions->master_sendMotorStopRequest(true);
	actions->slave_sendMotorStopRequest(true);
	actions->setErrorMode();
}

void Error::exit() {
	actions->master_sendMotorStopRequest(false);
	actions->slave_sendMotorStopRequest(false);
	previousState = MainState::ERROR;
}

void Error::initSubStateError() {
	substateError = new SubErrorPendingUnresigned;
	substateError->setAction(actions);
	substateError->setData(data);
	substateError->entry();
}

MainState Error::getCurrentState() {
	return MainState::ERROR;
}

bool Error::selfSolvableErrorOccurred() {
	return substateError->selfSolvableErrorOccurred();
}

bool Error::errorSelfSolved() {
	return substateError->errorSelfSolved();
}

bool Error::nonSelfSolvableErrorOccurred() {
	return substateError->nonSelfSolvableErrorOccurred();
}

bool Error::master_btnReset_Pressed() {
	substateError->master_btnReset_Pressed();
	if (substateError->isSubEndState()) {
		if (previousState == MainState::RUNNING) {
			exit();
			new (this) Running;
			entryHistory();
		} else if (previousState == MainState::SERVICEMODE) {
			exit();
			new (this) ServiceMode;
			entry();
		} else {
			exit();
			new (this) Standby;
			entry();
		}
		return true;
	}
	return false;
}

bool Error::slave_btnReset_Pressed() {
	substateError->slave_btnReset_Pressed();
	if (substateError->isSubEndState()) {
		if (previousState == MainState::RUNNING) {
			exit();
			new (this) Running;
			entryHistory();
		} else if (previousState == MainState::SERVICEMODE) {
			exit();
			new (this) ServiceMode;
			entry();
		} else {
			exit();
			new (this) Standby;
			entry();
		}
		return true;
	}
	return false;
}

bool Error::master_btnStart_PressedShort() {
	substateError->master_btnStart_PressedShort();
	if (substateError->isSubEndState()) {
		if (previousState == MainState::RUNNING) {
			exit();
			new (this) Running;
			entryHistory();
		} else if (previousState == MainState::SERVICEMODE) {
			exit();
			new (this) ServiceMode;
			entry();
		} else {
			exit();
			new (this) Standby;
			entry();
		}
		return true;
	}
	return false;
}

bool Error::slave_btnStart_PressedShort() {
	substateError->slave_btnStart_PressedShort();
	if (substateError->isSubEndState()) {
		exit();
		if (previousState == MainState::RUNNING) {
			exit();
			new (this) Running;
			entryHistory();
		} else if (previousState == MainState::SERVICEMODE) {
			exit();
			new (this) ServiceMode;
			entry();
		} else {
			exit();
			new (this) Standby;
			entry();
		}
		return true;
	}
	return false;
}
