/*
 * Running.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "Running.h"
#include "Standby.h"
#include "EStop.h"
#include "Error.h"
#include "logger/logger.hpp"
#include <chrono>
#include <thread>

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
}

bool Running::master_LBA_Blocked() {
	actions->master_sendMotorRightRequest(true);
	return true;
}

bool Running::master_LBA_Unblocked() {
	Workpiece* wp = data->wpManager->addWorkpiece();
	data->wpManager->addToArea_A(wp);
	Logger::info("Workpiece with id: " + std::to_string(wp->id) + " created and added to Area_A");
	return true;
}

bool Running::master_heightResultReceived(EventType event, float average) {
	Logger::debug("[MainFSM] Received FBM1 height result: " + EVENT_TO_STRING(event) + " - avg: " + std::to_string(average) + " mm");
	// TODO: Handle height result -> convert EventType to WorkpieceType and update workpiece data
	Workpiece* wp = data->wpManager->getter_head_Area_A();

	// setHeight()
	wp->avgHeight=average/10;

	//setType()
	if(event == EventType::HM_M_WS_F) {
		wp->WP_M_type=WorkpieceType::WS_F;
	} else if(event == EventType::HM_M_WS_OB) {
		wp->WP_M_type=WorkpieceType::WS_OB;
	} else if(event == EventType::HM_M_WS_BOM) {
		wp->WP_M_type=WorkpieceType::WS_BOM;
	} else if(event == EventType::HM_M_WS_UNKNOWN) {
		wp->WP_M_type=WorkpieceType::WS_UNKNOWN;
	}

	//moveFromArea_AtoArea_B()
	data->wpManager->moveFromArea_AtoArea_B();
	wp = data->wpManager->getter_head_Area_B();
	Logger::info("Workpiece with id: " + std::to_string(wp->id) + " -> type= "+ std::to_string(wp->WP_M_type) + ", height= " + std::to_string(wp->avgHeight));
	return true;
}

bool Running::master_metalDetected() {
	Workpiece* wp= data->wpManager->getter_head_Area_B();
	wp->metal=true;

	if(wp->WP_M_type == WorkpieceType::WS_BOM){
		wp->WP_M_type=WorkpieceType::WS_BUM;
	}
	return true;
}

bool Running::master_LBW_Blocked() {

	Workpiece* wp = data->wpManager->getter_head_Area_B();

	wp->sortOut= wp->WP_M_type==WorkpieceType::WS_F && wp->WP_M_type!=data->wpManager->getNextWorkpieceType();

	if(data->wpManager->getSortOut_M()){
		actions->master_openGate(false);
		Logger::info("Workpiece with id: " + std::to_string(wp->id) +" you shall not Pass throungh Master switch");
	}
	else
		actions->master_openGate(true);
		data->wpManager->moveFromArea_BtoArea_C();
		Logger::info("Workpiece with id: " + std::to_string(wp->id) +" Passed through Master switch and transfered to Area_C");
	return true;
}

bool Running::master_LBW_Unblocked() {
	return true;
}

bool Running::master_LBE_Blocked() {
	Workpiece* wp = data->wpManager->getter_head_Area_C();
	if(data->wpManager->fbm_S_Occupied()){
		actions->master_sendMotorRightRequest(false);
		Logger::info("Workpiece with id: " + std::to_string(wp->id) +" setMotorFastMaster(false)");
	}
	while(data->wpManager->fbm_S_Occupied()){
		//wait
		std::this_thread::sleep_for(std::chrono::seconds(2));
		Logger::info("Workpiece with id: " + std::to_string(wp->id) +" waiting for fbm_S to get free");
	}
	actions->master_sendMotorRightRequest(true);
	Logger::info("Workpiece with id: " + std::to_string(wp->id) +" actions->master_sendMotorStopRequest(true)");

	return true;
}

bool Running::master_LBE_Unblocked() {
	data->wpManager->moveFromArea_CtoArea_D();
	actions->slave_sendMotorRightRequest(true);

	return true;
}

bool Running::master_LBR_Blocked() {
	data->setRampFBM1Blocked(true);

	data->wpManager->removeFromArea_A();
	if(!data->wpManager->WP_ON_FBM_M()){
		actions->master_sendMotorRightRequest(false);
	}

	// If still blocked after 1s -> display warning
    std::thread t([=]() {
    	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    	if(data->isRampFBM1Blocked()) {
    		actions->master_warningOn();
    	}
    });
    t.detach();

	return true;
}

bool Running::master_LBR_Unblocked() {
	if(data->isRampFBM1Blocked()) {
		data->setRampFBM1Blocked(false);
		actions->master_warningOff();
		return true;
	}
	return false;
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

	// If still blocked after 1s -> display warning
    std::thread t([=]() {
    	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    	if(data->isRampFBM2Blocked()) {
    		actions->slave_warningOn();
    	}
    });
    t.detach();

	return true;
}

bool Running::slave_LBR_Unblocked() {
	if(data->isRampFBM2Blocked()) {
		data->setRampFBM2Blocked(false);
		actions->slave_warningOff();
		return true;
	}
	return false;
}



bool Running::slave_heightResultReceived(EventType event, float average) {
	Logger::debug("[MainFSM] Received FBM1 height result: " + EVENT_TO_STRING(event) + " - avg: " + std::to_string(average) + " mm");
	// TODO: Handle height result -> convert EventType to WorkpieceType and update workpiece data
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

bool Running::selfSolvableErrorOccurred() {
	exit();
	new(this) Error;
	entry();
	selfSolvableErrorOccurred();
	return true;
}

bool Running::nonSelfSolvableErrorOccurred() {
	exit();
	new(this) Error;
	entry();
	nonSelfSolvableErrorOccurred();
	return true;
}
