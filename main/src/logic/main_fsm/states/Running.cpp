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
	actions->setMotorFastMaster(false);
	actions->setMotorFastSlave(false);
	actions->setMotorStopMaster(false);
	actions->setMotorStopSlave(false);

}

void Running::exit() {
	Logger::debug("Running::exit");
}

bool Running::master_LBA_Blocked() {
	actions->setMotorFastMaster(true);
	return true;
}

bool Running::master_LBA_Unblocked() {
	Workpiece* wp = data->wpManager->addWorkpiece();
	Logger::info("New workpiece created: " + std::to_string(wp->id) );

	data->wpManager->addToArea_A(wp);
	Logger::info("Workpiece with id: " + std::to_string(wp->id) + " added to Area_A");
	return true;
}

/*bool Running::heightAtMasterReceived_M(Eventmanger type , float height){
	Workpiece* wp = data->wpManager->getter_head_Area_A();
	wp->avgHeight = height;
	wp->WP_M_type = type;
	return true;
}
*/

/*bool Running::metalDetected_M(){
	Workpiece* wp = data->wpManager->getter_head_Area_A();
	wp->Metal = height;
	return true;
}
*/

bool Running::master_LBW_Blocked() {
	Workpiece* wp = data->wpManager->getter_head_Area_A();
	//data->wpManager->setType_M(type);
	Logger::info("Workpiece with id: " + std::to_string(wp->id) +" master Type set to: ");
	//data->wpManager->setSortOut_M()
	if(data->wpManager->getSortOut_M()){
		//closegate()
		Logger::info("Workpiece with id: " + std::to_string(wp->id) +" you shall not Pass throungh Master switch");
	}
	else
		//opengate();
		Logger::info("Workpiece with id: " + std::to_string(wp->id)+"Passed through Master switch");
		data->wpManager->moveFromArea_BtoArea_C();
		Logger::info("Workpiece with id: " + std::to_string(wp->id) +" transfered to Area_C");

	return true;
}

bool Running::master_LBW_Unblocked() {

	return true;
}

bool Running::master_LBE_Blocked() {
	Workpiece* wp = data->wpManager->getter_head_Area_A();
	if(data->wpManager->fbm_S_Occupied()){
		actions->setMotorFastMaster(false);
		Logger::info("Workpiece with id: " + std::to_string(wp->id) +" setMotorFastMaster(false)");
	}
	while(data->wpManager->fbm_S_Occupied()){
		//wait
		Logger::info("Workpiece with id: " + std::to_string(wp->id) +" waiting for fbm_S to get free");
	}
	actions->setMotorFastMaster(true);
	Logger::info("Workpiece with id: " + std::to_string(wp->id) +" setMotorFastMaster(true)");
	return true;
}

bool Running::master_LBE_Unblocked() {
	data->wpManager->moveFromArea_CtoArea_D();
	actions->setMotorFastSlave(true);
	return true;
}

bool Running::master_LBR_Blocked() {
	data->wpManager->removeFromArea_B();
	if(data->wpManager->WP_ON_FBM_M())
		actions->setMotorFastMaster(false);
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
