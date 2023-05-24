/*
 * MainContext.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "MainContext.h"
#include "MainContextData.h"
#include "states/Standby.h"
#include "logger/logger.hpp"

#include <memory>

MainContext::MainContext() {
	this->data = std::unique_ptr<MainContextData>(new MainContextData());
	this->state = new Standby();
}

MainContext::~MainContext() {
	delete state;
}

MainState MainContext::getCurrentState() {
	return state->getCurrentState();
}

void MainContext::master_LBA_Blocked() {
	Logger::debug("MainContext::master_LBA_Blocked");
}

void MainContext::master_LBA_Unblocked() {
	Logger::debug("MainContext::master_LBA_Unblocked");
}

void MainContext::master_LBW_Blocked() {
	Logger::debug("MainContext::master_LBW_Blocked");
}

void MainContext::master_LBW_Unblocked() {
	Logger::debug("MainContext::master_LBW_Unblocked");
}

void MainContext::master_LBE_Blocked() {
	Logger::debug("MainContext::master_LBE_Blocked");
}

void MainContext::master_LBE_Unblocked() {
	Logger::debug("MainContext::master_LBE_Unblocked");
}

void MainContext::master_LBR_Blocked() {
	Logger::debug("MainContext::master_LBR_Blocked");
}

void MainContext::master_LBR_Unblocked() {
	Logger::debug("MainContext::master_LBR_Unblocked");
}

void MainContext::slave_LBA_Blocked() {
	Logger::debug("MainContext::slave_LBA_Blocked");
}

void MainContext::slave_LBA_Unblocked() {
	Logger::debug("MainContext::slave_LBA_Unblocked");
}

void MainContext::slave_LBW_Blocked() {
	Logger::debug("MainContext::slave_LBW_Blocked");
}

void MainContext::slave_LBW_Unblocked() {
	Logger::debug("MainContext::slave_LBW_Unblocked");
}

void MainContext::slave_LBE_Blocked() {
	Logger::debug("MainContext::slave_LBE_Blocked");
}

void MainContext::slave_LBE_Unblocked() {
	Logger::debug("MainContext::slave_LBE_Unblocked");
}

void MainContext::slave_LBR_Blocked() {
	Logger::debug("MainContext::slave_LBR_Blocked");
}

void MainContext::slave_LBR_Unblocked() {
	Logger::debug("MainContext::slave_LBR_Unblocked");
}

void MainContext::master_btnStart_PressedShort() {
	Logger::debug("MainContext::master_btnStart_PressedShort");
}

void MainContext::master_btnStart_PressedLong() {
	Logger::debug("MainContext::master_btnStart_PressedLong");
}

void MainContext::master_btnStop_Pressed() {
	Logger::debug("MainContext::master_btnStop_Pressed");
}

void MainContext::master_btnReset_Pressed() {
	Logger::debug("MainContext::master_btnReset_Pressed");
}

void MainContext::slave_btnStart_PressedShort() {
	Logger::debug("MainContext::slave_btnStart_PressedShort");
}

void MainContext::slave_btnStart_PressedLong() {
	Logger::debug("MainContext::slave_btnStart_PressedLong");
}

void MainContext::slave_btnStop_Pressed() {
	Logger::debug("MainContext::slave_btnStop_Pressed");
}

void MainContext::slave_btnReset_Pressed() {
	Logger::debug("MainContext::slave_btnReset_Pressed");
}

void MainContext::master_EStop_Pressed() {
	Logger::debug("MainContext::master_EStop_Pressed");
}

void MainContext::master_EStop_Released() {
	Logger::debug("MainContext::master_EStop_Released");
}

void MainContext::slave_EStop_Pressed() {
	Logger::debug("MainContext::slave_EStop_Pressed");
}

void MainContext::slave_EStop_Released() {
	Logger::debug("MainContext::slave_EStop_Released");
}
