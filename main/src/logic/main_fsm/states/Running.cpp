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
	Logger::debug("Running::exit");
}

bool Running::master_LBA_Blocked() {
	actions->master_sendMotorRightRequest(true);
	return true;
}

bool Running::master_LBA_Unblocked() {
	Workpiece* wp = data->wpManager->addWorkpiece();                          	//addWorkpiece
	Logger::info("Workpiece with id: " + std::to_string(wp->id) + " created and added to Area_A");
	return true;
}

bool Running::master_heightResultReceived(EventType event, float average) {
	Logger::debug("[MainFSM] Received FBM1 height result: " + EVENT_TO_STRING(event) + " - avg: " + std::to_string(average) + " mm");

	data->wpManager->setHeight(AreaType::AREA_A, average);   					// setheight()
	data->wpManager->setTypeEvent(event, AreaType::AREA_A);   					// setType()
	data->wpManager->moveFromAreaToArea(AreaType::AREA_A, AreaType::AREA_B); 	//moveFromArea_AtoArea_B()

	//Logger::info("Workpiece with id: " + std::to_string(wp->id) + "type= "+ std::to_string(wp->WP_M_type) +
		//" height= " + std::to_string(wp->avgHeight));
	return true;
}

bool Running::master_metalDetected() {
	data->wpManager->setMetal(AreaType::AREA_B);
	Workpiece* wp = data->wpManager->getHeadOfArea(AreaType::AREA_B);

	if(wp->WP_M_type == WorkpieceType::WS_BOM){
		wp->WP_M_type = WorkpieceType::WS_BUM;
	}
	return true;
}

bool Running::master_LBW_Blocked() {

	Workpiece* wp = data->wpManager->getHeadOfArea(AreaType::AREA_B);
	WorkpieceType detected_type = wp->WP_M_type;
	WorkpieceType config_type = data->wpManager->getNextWorkpieceType();

	wp->sortOut= detected_type==WorkpieceType::WS_F && detected_type!=config_type;

	if(wp->sortOut){
		actions->master_openGate(false);
		Logger::info("WP id: " + std::to_string(wp->id) +" kicked out");
	}
	else
		actions->master_openGate(true);
		data->wpManager->moveFromAreaToArea(AreaType::AREA_B, AreaType::AREA_C);
		Logger::info("WP id: " + std::to_string(wp->id) +" Passed to Area_C");
	return true;
}


bool Running::master_LBW_Unblocked() {
	return true;
}

bool Running::master_LBE_Blocked() {
	Workpiece* wp = data->wpManager->getHeadOfArea(AreaType::AREA_C);
	if(data->wpManager->isFBM_SOccupied()){
		actions->master_sendMotorRightRequest(false);
	}
	while(data->wpManager->isFBM_SOccupied()){
		//wait
		std::this_thread::sleep_for(std::chrono::seconds(3));
		Logger::info("Workpiece with id: " + std::to_string(wp->id) +" waiting for fbm_S to get free");
	}
	actions->master_sendMotorRightRequest(true);
	return true;
}

bool Running::master_LBE_Unblocked() {
	data->wpManager->moveFromAreaToArea(AreaType::AREA_C, AreaType::AREA_D);
	actions->slave_sendMotorRightRequest(true);

	return true;
}

bool Running::master_LBR_Blocked() {
	data->wpManager->removeFromArea(AreaType::AREA_B);
	if(data->wpManager->isFBM_MEmpty()){
		actions->master_sendMotorRightRequest(false);
	}

	// code was allready there
	data->setRampFBM1Blocked(true);
	actions->master_sendMotorRightRequest(false);
	return true;
}

bool Running::master_LBR_Unblocked() {
	data->setRampFBM1Blocked(false);
	return true;
}

//---------------------------------------------------------------------------------
bool Running::slave_LBA_Blocked() {
	if(data->wpManager->isFBM_MEmpty()){
		actions->master_sendMotorRightRequest(false);
	}
	return true;
}

bool Running::slave_LBA_Unblocked() {
	return true;
}

bool Running::slave_heightResultReceived(EventType event, float average) {
	Logger::debug("[MainFSM] Received FBM1 height result: " + EVENT_TO_STRING(event) + " - avg: " + std::to_string(average) + " mm");

	data->wpManager->setHeight(AreaType::AREA_D, average);   					// setheight()
	data->wpManager->setTypeEvent(event, AreaType::AREA_D);   					// setType()

	return true;
}

bool Running::slave_metalDetected() {
	Workpiece* wp = data->wpManager->getHeadOfArea(AreaType::AREA_D);
	data->wpManager->setMetal(AreaType::AREA_D);                            	//setMetal()

	if(wp->WP_S_type == WorkpieceType::WS_BOM){									//setType()
		wp->WP_S_type = WorkpieceType::WS_BUM;
	}
	return true;
}

bool Running::slave_LBW_Blocked() {
	Workpiece* wp = data->wpManager->getHeadOfArea(AreaType::AREA_D);
	WorkpieceType slave_type= data->wpManager->getNextWorkpieceType();
	WorkpieceType detected_type= wp->WP_S_type;

	wp->sortOut= detected_type != slave_type;

	if(wp->sortOut){
		actions->slave_openGate(false);
		Logger::info("WP id: " + std::to_string(wp->id) +" kicked out");
	}
	else
		actions->slave_openGate(true);
		Logger::info("WP id: " + std::to_string(wp->id) +" Passed to Area_C");
	return true;
}

bool Running::slave_LBW_Unblocked() {
	return true;
}

bool Running::slave_LBE_Blocked() {
	Workpiece* wp = data->wpManager->getHeadOfArea(AreaType::AREA_D);
	if(!data->wpManager->isQueueempty(AreaType::AREA_D)){
		data->wpManager->printWorkpiece(wp);
		actions->slave_sendMotorRightRequest(false);
	}

	return true;
}

bool Running::slave_LBE_Unblocked() {
	data->wpManager->removeFromArea(AreaType::AREA_D);
	return true;
}

bool Running::slave_LBR_Blocked() {
	actions->slave_sendMotorRightRequest(false);
	data->wpManager->removeFromArea(AreaType::AREA_D);

	data->setRampFBM2Blocked(true);
	return true;
}

bool Running::slave_LBR_Unblocked() {
	data->setRampFBM2Blocked(false);
	return true;
}


//---------------------------------------------------------------------------------



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
