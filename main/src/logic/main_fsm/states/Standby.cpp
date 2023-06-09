/*
 * Standby.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "Standby.h"

#include <iostream>

#include "EStop.h"
#include "Running.h"
#include "ServiceMode.h"
#include "logger/logger.hpp"
#include "configuration/Configuration.h"

MainState Standby::getCurrentState() { return MainState::STANDBY; };

void Standby::entry() {
    Logger::info("Entered Standby mode");
    Logger::user_info("Press Start button shortly (< 2 seconds) to go to Running mode or long to start ServiceMode");
    actions->master_sendMotorStopRequest(true);
    actions->slave_sendMotorStopRequest(true);
    actions->setStandbyMode();
}

void Standby::exit() {
	previousState = MainState::STANDBY;
	actions->master_sendMotorStopRequest(false);
	actions->slave_sendMotorStopRequest(false);
}

bool Standby::master_btnStart_PressedShort() {
	if(!Configuration::getInstance().calibrationValid()) {
		Logger::error("Calibration invalid - please calibrate HeightSensor!");
		return false;
	}
    exit();
    new (this) Running;
    entry();
    return true;
}

bool Standby::master_btnStart_PressedLong() {
    exit();
    new (this) ServiceMode;
    entry();
    return true;
}

bool Standby::master_EStop_Pressed() {
    exit();
    new (this) EStop;
    entry();
    return true;
}

bool Standby::slave_btnStart_PressedShort() {
	if(!Configuration::getInstance().calibrationValid()) {
		Logger::error("Calibration invalid - please calibrate HeightSensor!");
		return false;
	}
    exit();
    new (this) Running;
    entry();
    return true;
}

bool Standby::slave_btnStart_PressedLong() {
    exit();
    new (this) ServiceMode;
    entry();
    return true;
}

bool Standby::slave_EStop_Pressed() {
    exit();
    new (this) EStop;
    entry();
    return true;
}

bool Standby::master_btnReset_PressedLong() {
	data->wpManager->reset_wpm();
	actions->master_sendMotorRightRequest(false);
	actions->slave_sendMotorRightRequest(false);
	return true;
}

bool Standby::slave_btnReset_PressedLong() {
	data->wpManager->reset_wpm();
	actions->master_sendMotorRightRequest(false);
	actions->slave_sendMotorRightRequest(false);
	return true;
}
