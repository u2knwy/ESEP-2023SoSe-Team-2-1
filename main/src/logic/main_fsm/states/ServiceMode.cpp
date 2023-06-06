/*
 * ServiceMode.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "ServiceMode.h"
#include "Standby.h"
#include "EStop.h"
#include "logger/logger.hpp"
#include "SubServiceModeSelftestSensors.h"

#include <iostream>

MainState ServiceMode::getCurrentState() {
	return MainState::SERVICEMODE;
};

void ServiceMode::entry() {
	Logger::info("Entered Service mode");
	initSubStateServiceMode();
	actions->setServiceMode();
}

void ServiceMode::exit() {
	Logger::debug("ServiceMode::exit");
}

void ServiceMode::initSubStateServiceMode() {
	substateServiceMode = new SubServiceModeSelftestSensors;
}

bool ServiceMode::master_LBA_Blocked() {
	Logger::debug("ServiceMode::master_LBA_Blocked");
	return substateServiceMode->master_LBA_Blocked();
}

bool ServiceMode::master_LBA_Unblocked() {
	Logger::debug("ServiceMode::master_LBA_Unblocked");
	return substateServiceMode->master_LBA_Unblocked();
}

bool ServiceMode::master_LBW_Blocked() {
	Logger::debug("ServiceMode::master_LBW_Blocked");
	return substateServiceMode->master_LBW_Blocked();
}

bool ServiceMode::master_LBW_Unblocked() {
	Logger::debug("ServiceMode::master_LBW_Unblocked");
	return substateServiceMode->master_LBW_Unblocked();
}

bool ServiceMode::master_LBE_Blocked() {
	Logger::debug("ServiceMode::master_LBE_Blocked");
	return substateServiceMode->master_LBE_Blocked();
}

bool ServiceMode::master_LBE_Unblocked() {
	Logger::debug("ServiceMode::master_LBE_Unblocked");
	return substateServiceMode->master_LBE_Unblocked();
}

bool ServiceMode::master_LBR_Blocked() {
	Logger::debug("ServiceMode::master_LBR_Blocked");
	return substateServiceMode->master_LBR_Blocked();
}

bool ServiceMode::master_LBR_Unblocked() {
	Logger::debug("ServiceMode::master_LBR_Unblocked");
	return substateServiceMode->master_LBR_Unblocked();
}

bool ServiceMode::slave_LBA_Blocked() {
	Logger::debug("ServiceMode::slave_LBA_Blocked");
	return substateServiceMode->slave_LBA_Blocked();
}

bool ServiceMode::slave_LBA_Unblocked() {
	Logger::debug("ServiceMode::slave_LBA_Unblocked");
	return substateServiceMode->slave_LBA_Unblocked();
}

bool ServiceMode::slave_LBW_Blocked() {
	Logger::debug("ServiceMode::slave_LBW_Blocked");
	return substateServiceMode->slave_LBW_Blocked();
}

bool ServiceMode::slave_LBW_Unblocked() {
	Logger::debug("ServiceMode::slave_LBW_Unblocked");
	return substateServiceMode->slave_LBW_Unblocked();
}

bool ServiceMode::slave_LBE_Blocked() {
	Logger::debug("ServiceMode::slave_LBE_Blocked");
	return substateServiceMode->slave_LBE_Blocked();
}

bool ServiceMode::slave_LBE_Unblocked() {
	Logger::debug("ServiceMode::slave_LBE_Unblocked");
	return substateServiceMode->slave_LBE_Unblocked();
}

bool ServiceMode::slave_LBR_Blocked() {
	Logger::debug("ServiceMode::slave_LBR_Blocked");
	return substateServiceMode->slave_LBR_Blocked();
}

bool ServiceMode::slave_LBR_Unblocked() {
	Logger::debug("ServiceMode::slave_LBR_Unblocked");
	return substateServiceMode->slave_LBR_Unblocked();
}

bool ServiceMode::master_btnStart_PressedShort() {
	return substateServiceMode->master_btnStart_PressedShort();
}

bool ServiceMode::master_btnStop_Pressed() {
	Logger::debug("ServiceMode::master_btnStop_Pressed");
	exit();
	new(this) Standby;
	entry();
	return true;
}

bool ServiceMode::master_btnReset_Pressed() {
	return substateServiceMode->master_btnReset_Pressed();
}

bool ServiceMode::slave_btnStart_PressedShort() {
	return substateServiceMode->master_btnStart_PressedShort();
}

bool ServiceMode::slave_btnStop_Pressed() {
	Logger::debug("ServiceMode::slave_btnStop_Pressed");
	exit();
	new(this) Standby;
	entry();
	return true;
}

bool ServiceMode::slave_btnReset_Pressed() {
	return substateServiceMode->slave_btnReset_Pressed();
}

bool ServiceMode::master_EStop_Pressed() {
	Logger::debug("ServiceMode::master_EStop_Pressed");
	exit();
	new(this) EStop;
	entry();
	return true;
}

bool ServiceMode::slave_EStop_Pressed() {
	Logger::debug("ServiceMode::slave_EStop_Pressed");
	exit();
	new(this) EStop;
	entry();
	return true;
}
