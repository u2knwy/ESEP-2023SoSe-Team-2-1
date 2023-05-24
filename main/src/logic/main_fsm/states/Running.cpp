/*
 * Running.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "Running.h"
#include "Standby.h"
#include "logger/logger.hpp"

#include <iostream>

MainState Running::getCurrentState() {
	return MainState::RUNNING;
};

void Running::entry() {
	Logger::debug("Running::entry");
}

void Running::exit() {
	Logger::debug("Running::exit");
}

bool Running::master_LBA_Blocked() {
	Logger::debug("Running::master_LBA_Blocked");
	return true;
}

bool Running::master_LBA_Unblocked() {
	Logger::debug("Running::master_LBA_Unblocked");
	return true;
}

bool Running::master_LBW_Blocked() {
	Logger::debug("Running::master_LBW_Blocked");
	return true;
}

bool Running::master_LBW_Unblocked() {
	Logger::debug("Running::master_LBW_Unblocked");
	return true;
}

bool Running::master_LBE_Blocked() {
	Logger::debug("Running::master_LBE_Blocked");
	return true;
}

bool Running::master_LBE_Unblocked() {
	Logger::debug("Running::master_LBE_Unblocked");
	return true;
}

bool Running::master_LBR_Blocked() {
	Logger::debug("Running::master_LBR_Blocked");
	return true;
}

bool Running::master_LBR_Unblocked() {
	Logger::debug("Running::master_LBR_Unblocked");
	return true;
}

bool Running::slave_LBA_Blocked() {
	Logger::debug("Running::slave_LBA_Blocked");
	return true;
}

bool Running::slave_LBA_Unblocked() {
	Logger::debug("Running::slave_LBA_Unblocked");
	return true;
}

bool Running::slave_LBW_Blocked() {
	Logger::debug("Running::slave_LBW_Blocked");
	return true;
}

bool Running::slave_LBW_Unblocked() {
	Logger::debug("Running::slave_LBW_Unblocked");
	return true;
}

bool Running::slave_LBE_Blocked() {
	Logger::debug("Running::slave_LBE_Blocked");
	return true;
}

bool Running::slave_LBE_Unblocked() {
	Logger::debug("Running::slave_LBE_Unblocked");
	return true;
}

bool Running::slave_LBR_Blocked() {
	Logger::debug("Running::slave_LBR_Blocked");
	return true;
}

bool Running::slave_LBR_Unblocked() {
	Logger::debug("Running::slave_LBR_Unblocked");
	return true;
}

bool Running::master_btnStop_Pressed() {
	Logger::debug("Running::master_btnStop_Pressed");
	exit();
	new(this) Standby;
	entry();
	return true;
}

bool Running::slave_btnStop_Pressed() {
	Logger::debug("Running::slave_btnStop_Pressed");
	exit();
	new(this) Standby;
	entry();
	return true;
}

bool Running::master_EStop_Pressed() {
	Logger::debug("Running::master_EStop_Pressed");
	return true;
}

bool Running::slave_EStop_Pressed() {
	Logger::debug("Running::slave_EStop_Pressed");
	return true;
}
