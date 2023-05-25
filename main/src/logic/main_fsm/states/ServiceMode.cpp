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

#include <iostream>

MainState ServiceMode::getCurrentState() {
	return MainState::SERVICEMODE;
};

void ServiceMode::entry() {
	Logger::debug("ServiceMode::entry");
}

void ServiceMode::exit() {
	Logger::debug("ServiceMode::exit");
}

bool ServiceMode::master_LBA_Blocked() {
	Logger::debug("ServiceMode::master_LBA_Blocked");
	return true;
}

bool ServiceMode::master_LBA_Unblocked() {
	Logger::debug("ServiceMode::master_LBA_Unblocked");
	return true;
}

bool ServiceMode::master_LBW_Blocked() {
	Logger::debug("ServiceMode::master_LBW_Blocked");
	return true;
}

bool ServiceMode::master_LBW_Unblocked() {
	Logger::debug("ServiceMode::master_LBW_Unblocked");
	return true;
}

bool ServiceMode::master_LBE_Blocked() {
	Logger::debug("ServiceMode::master_LBE_Blocked");
	return true;
}

bool ServiceMode::master_LBE_Unblocked() {
	Logger::debug("ServiceMode::master_LBE_Unblocked");
	return true;
}

bool ServiceMode::master_LBR_Blocked() {
	Logger::debug("ServiceMode::master_LBR_Blocked");
	return true;
}

bool ServiceMode::master_LBR_Unblocked() {
	Logger::debug("ServiceMode::master_LBR_Unblocked");
	return true;
}

bool ServiceMode::slave_LBA_Blocked() {
	Logger::debug("ServiceMode::slave_LBA_Blocked");
	return true;
}

bool ServiceMode::slave_LBA_Unblocked() {
	Logger::debug("ServiceMode::slave_LBA_Unblocked");
	return true;
}

bool ServiceMode::slave_LBW_Blocked() {
	Logger::debug("ServiceMode::slave_LBW_Blocked");
	return true;
}

bool ServiceMode::slave_LBW_Unblocked() {
	Logger::debug("ServiceMode::slave_LBW_Unblocked");
	return true;
}

bool ServiceMode::slave_LBE_Blocked() {
	Logger::debug("ServiceMode::slave_LBE_Blocked");
	return true;
}

bool ServiceMode::slave_LBE_Unblocked() {
	Logger::debug("ServiceMode::slave_LBE_Unblocked");
	return true;
}

bool ServiceMode::slave_LBR_Blocked() {
	Logger::debug("ServiceMode::slave_LBR_Blocked");
	return true;
}

bool ServiceMode::slave_LBR_Unblocked() {
	Logger::debug("ServiceMode::slave_LBR_Unblocked");
	return true;
}

bool ServiceMode::master_btnStop_Pressed() {
	Logger::debug("ServiceMode::master_btnStop_Pressed");
	exit();
	new(this) Standby;
	entry();
	return true;
}

bool ServiceMode::slave_btnStop_Pressed() {
	Logger::debug("ServiceMode::slave_btnStop_Pressed");
	exit();
	new(this) Standby;
	entry();
	return true;
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
