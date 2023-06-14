/*
 * Running.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "Running.h"
#include "Standby.h"
#include "EStop.h"
#include "logger/logger.hpp"

#include <iostream>

MainState Running::getCurrentState() {
	return MainState::RUNNING;
};

void Running::entry() {
	Logger::info("Entered Running mode");
	actions->setRunningMode();
	actions->master_sendMotorRightRequest(false);
	actions->slave_sendMotorRightRequest(false);
	actions->master_sendMotorStopRequest(false);
	actions->slave_sendMotorStopRequest(false);
}

void Running::exit() {
	Logger::debug("Running::exit");
}

bool Running::master_LBA_Blocked() {
	Workpiece* wp = data->wpManager->addWorkpiece();
	Logger::info("New workpiece inserted: " + wp->getId());
	actions->master_sendMotorRightRequest(true);
	return true;
}

bool Running::master_LBA_Unblocked() {
	return true;
}

bool Running::master_LBW_Blocked() {
	return true;
}

bool Running::master_LBW_Unblocked() {
	return true;
}

bool Running::master_LBE_Blocked() {
	actions->master_sendMotorStopRequest(true);
	return true;
}

bool Running::master_LBE_Unblocked() {
	actions->master_sendMotorStopRequest(false);
	return true;
}

bool Running::master_LBR_Blocked() {
	data->setRampFBM1Blocked(true);
	actions->master_sendMotorRightRequest(false);
	return true;
}

bool Running::master_LBR_Unblocked() {
	data->setRampFBM1Blocked(false);
	return true;
}

bool Running::slave_LBA_Blocked() {
	return true;
}

bool Running::slave_LBA_Unblocked() {
	return true;
}

bool Running::slave_LBW_Blocked() {
	return true;
}

bool Running::slave_LBW_Unblocked() {
	return true;
}

bool Running::slave_LBE_Blocked() {
	return true;
}

bool Running::slave_LBE_Unblocked() {
	return true;
}

bool Running::slave_LBR_Blocked() {
	data->setRampFBM2Blocked(true);
	return true;
}

bool Running::slave_LBR_Unblocked() {
	data->setRampFBM2Blocked(false);
	return true;
}

bool Running::master_heightResultReceived(EventType event, float average) {
	Logger::debug("[MainFSM] Received FBM1 height result: " + EVENT_TO_STRING(event) + " - avg: " + std::to_string(average) + " mm");
	// TODO: Handle height result -> convert EventType to WorkpieceType and update workpiece data
	if(event == EventType::HM_M_WS_F) {
		//wp.setFBM1Type(WorkpieceType::WS_F);
	} else if(event == EventType::HM_M_WS_OB) {
		//wp.setFBM1Type(WorkpieceType::WS_OB);
	} else if(event == EventType::HM_M_WS_BOM) {
		//wp.setFBM1Type(WorkpieceType::WS_BOM);
	} else if(event == EventType::HM_M_WS_UNKNOWN) {
		//wp.setFBM1Type(WorkpieceType::WS_UNKNOWN);
	}
	return true;
}

bool Running::slave_heightResultReceived(EventType event, float average) {
	Logger::debug("[MainFSM] Received FBM1 height result: " + EVENT_TO_STRING(event) + " - avg: " + std::to_string(average) + " mm");
	// TODO: Handle height result -> convert EventType to WorkpieceType and update workpiece data
	return true;
}

bool Running::master_metalDetected() {
	// TODO: Update workpiece: metal was detected at FBM1
	return true;
}

bool Running::slave_metalDetected() {
	// TODO: Update workpiece: metal was detected at FBM2
	return true;
}

bool Running::master_btnStop_Pressed() {
	exit();
	new(this) Standby;
	entry();
	return true;
}

bool Running::slave_btnStop_Pressed() {
	exit();
	new(this) Standby;
	entry();
	return true;
}

bool Running::master_EStop_Pressed() {
	exit();
	new(this) EStop;
	entry();
	return true;
}

bool Running::slave_EStop_Pressed() {
	exit();
	new(this) EStop;
	entry();
	return true;
}
