/*
 * Running.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include "Running.h"
#include "configuration/Configuration.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "EStop.h"
#include "Error.h"
#include "Standby.h"
#include "logger/logger.hpp"

MainState Running::getCurrentState() {
	return MainState::RUNNING;
}

void Running::entry() {
	Logger::info("Entered Running mode");
	data->wpManager->printCurrentOrder();
	Logger::user_info("Put new workpieces at start of FBM1 to start sorting");
	actions->setRunningMode();
	if (data->isRampFBM1Blocked()) {
		setRampBlocked_M(true);
	}
	if (data->isRampFBM2Blocked()) {
		setRampBlocked_S(true);
	}
}

void Running::exit() {
	previousState = MainState::RUNNING;
}

void Running::entryHistory() {
	actions->slave_openGate(false);
	Logger::info("Entered Running mode - restored previous state");
	data->wpManager->printCurrentOrder();
	actions->setRunningMode();
	if (data->isRampFBM1Blocked()) {
		setRampBlocked_M(true);
	}
	if (data->isRampFBM2Blocked()) {
		setRampBlocked_S(true);
	}
}

bool Running::master_LBA_Blocked() {
	if (data->wpManager->isFBM_MEmpty()) {
		actions->master_sendMotorRightRequest(true);
	}
	Workpiece *wp = data->wpManager->addWorkpiece();   // addWorkpiece
	return true;
}

bool Running::master_LBA_Unblocked() {
	return true;
}

bool Running::master_heightResultReceived(EventType event, float average) {
	Logger::debug("[MainFSM] Received FBM1 height result: " + EVENT_TO_STRING(event) + " - avg: " + std::to_string(average) + " mm");

	if (!data->wpManager->isQueueempty(AreaType::AREA_A)) {
		Workpiece *wp = data->wpManager->getHeadOfArea(AreaType::AREA_A);
		data->wpManager->setHeight(AreaType::AREA_A, average);    // setheight()
		data->wpManager->setTypeEvent(event, AreaType::AREA_A);   // setType()
		data->wpManager->moveFromAreaToArea(AreaType::AREA_A, AreaType::AREA_B); // moveFromArea_AtoArea_B()
	}
	return true;
}

bool Running::master_metalDetected() {
	if (!data->wpManager->isQueueempty(AreaType::AREA_B)) {
		data->wpManager->setMetal(AreaType::AREA_B);   // setmetal()
		Workpiece *wp = data->wpManager->getHeadOfArea(AreaType::AREA_B);

		if (wp->M_type == WorkpieceType::WS_BOM)   // setType()
				{
			wp->M_type = WorkpieceType::WS_BUM;
		}

		std::stringstream ss;
		ss << "FBM1 metal detected - type=" << WP_TYPE_TO_STRING(wp->M_type);
		Logger::info(ss.str());
	}
	return true;
}

bool Running::master_LBW_Blocked() {
	if (!data->wpManager->isQueueempty(AreaType::AREA_B)) {
		Workpiece *wp = data->wpManager->getHeadOfArea(AreaType::AREA_B);
		Logger::info(data->wpManager->to_string_Workpiece(wp));

		WorkpieceType detected_type = wp->M_type;
		WorkpieceType config_type = data->wpManager->getNextWorkpieceType();

		bool rampOneBlocked = data->wpManager->getRamp_one();
		bool rampTwoBlocked = data->wpManager->getRamp_two();

		if (!rampOneBlocked && rampTwoBlocked) {  //compare()
			Logger::debug("Erste Rampe frei und zweite Belegt");
			wp->sortOut = detected_type != config_type;
		} else if ((rampOneBlocked && !rampTwoBlocked) || (rampOneBlocked && rampTwoBlocked)) {
			Logger::debug("Erste Rampe belegt und zweite frei oder beide belegt");
			wp->sortOut = false;
		} else {
			Logger::debug("beide frei");
			wp->sortOut = detected_type == WorkpieceType::WS_F && detected_type != config_type;
		}

		std::stringstream ss;
		ss << "(FBM1) WS at switch -> Expected: " << WP_TYPE_TO_STRING(config_type);
		ss << ", Detected: " << WP_TYPE_TO_STRING(detected_type);
		ss << (wp->sortOut ? " -> sort out" : " -> go to FBM2");
		Logger::info(ss.str());

		if (wp->sortOut) {
			if (data->wpManager->getRamp_one()) {
				Logger::error("Ramp at FBM1 is full -> Sorting out not possible!");
				actions->master_manualSolvingErrorOccurred();
				return true;
			}

			actions->master_openGate(false);					//closegate()

			Logger::info("WP id: " + std::to_string(wp->id) + " kicked out");
		} else {
			actions->master_openGate(true);    // closegate()

			data->wpManager->moveFromAreaToArea(AreaType::AREA_B, AreaType::AREA_C);
			Logger::info("WP id: " + std::to_string(wp->id) + " Passed to Area_C");
		}
	}
	return true;
}

bool Running::master_LBW_Unblocked() {
	return true;
}

bool Running::master_LBE_Blocked() {
	if (!data->wpManager->isQueueempty(AreaType::AREA_C)) {
		Workpiece *wp = data->wpManager->getHeadOfArea(AreaType::AREA_C);
		if (!data->wpManager->isFBM_SEmpty())
		/*{
		 // If FBM2 empty -> start FBM2 motor
		 actions->slave_sendMotorRightRequest(true);
		 } else */{
			// If FBM2 occupied -> stop FBM1 motor
			actions->master_sendMotorRightRequest(false);
		}
		// Close switch if open
		if (!data->master_pusherMounted) {
			actions->master_openGate(false);
		}
	}
	return true;
}

bool Running::master_LBE_Unblocked() {
	if (!data->wpManager->isQueueempty(AreaType::AREA_C)) {
		if (data->wpManager->isQueueempty(AreaType::AREA_D)) {
			data->wpManager->moveFromAreaToArea(AreaType::AREA_C, AreaType::AREA_D);
			actions->slave_sendMotorRightRequest(true);
		}
		return true;
	}
	return false;
}

bool Running::master_LBR_Blocked() {
	setRampBlocked_M(true);

	if (!data->wpManager->isQueueempty(AreaType::AREA_B)) {
		data->wpManager->removeFromArea(AreaType::AREA_B);
		if (data->wpManager->isFBM_MEmpty()) {
			actions->master_sendMotorRightRequest(false);
		}
		return true;
	} else {
		return false;
	}
}

bool Running::master_LBR_Unblocked() {
	setRampBlocked_M(false);
	return true;
}

//---------------------------------------------------------------------------------
bool Running::slave_LBA_Blocked() {
	if (!data->wpManager->isQueueempty(AreaType::AREA_D)) {
		if (data->wpManager->isFBM_MEmpty()) {
			actions->master_sendMotorRightRequest(false);
		}
	}
	return true;
}

bool Running::slave_LBA_Unblocked() {
	return true;
}

bool Running::slave_heightResultReceived(EventType event, float average) {
	Logger::debug("[MainFSM] Received FBM1 height result: " + EVENT_TO_STRING(event) + " - avg: " + std::to_string(average) + " mm");

	if (!data->wpManager->isQueueempty(AreaType::AREA_D)) {
		data->wpManager->setHeight(AreaType::AREA_D, average);    // setheight()
		data->wpManager->setTypeEvent(event, AreaType::AREA_D);   // setType()
	}

	return true;
}

bool Running::slave_metalDetected() {
	if (!data->wpManager->isQueueempty(AreaType::AREA_D)) {
		Workpiece *wp = data->wpManager->getHeadOfArea(AreaType::AREA_D);
		data->wpManager->setMetal(AreaType::AREA_D);   // setMetal()

		if (wp->S_type == WorkpieceType::WS_BOM) {     // setType()
			wp->S_type = WorkpieceType::WS_BUM;
		}

		std::stringstream ss;
		ss << "FBM2 metal detected - type=" << WP_TYPE_TO_STRING(wp->S_type);
		Logger::info(ss.str());
	}
	return true;
}

bool Running::slave_LBW_Blocked() {
	if (!data->wpManager->isQueueempty(AreaType::AREA_D)) {
		Workpiece *wp = data->wpManager->getHeadOfArea(AreaType::AREA_D);
		Logger::info(data->wpManager->to_string_Workpiece_FBM2(wp));

		WorkpieceType slave_type = wp->S_type;
		WorkpieceType expected_type = data->wpManager->getNextWorkpieceType();

		wp->sortOut = expected_type != slave_type;   // sortOut()

		std::stringstream ss;
		ss << "(FBM2) WS at switch -> Expected: " << WP_TYPE_TO_STRING(expected_type);
		ss << ", Detected: " << WP_TYPE_TO_STRING(slave_type) << (wp->sortOut ? " -> sort out" : " -> go to End");
		Logger::info(ss.str());

		if (wp->sortOut) {
			if (data->wpManager->getRamp_two()) {
				Logger::error("Cannot sort out, because ramp at FBM2 is full!");
				actions->slave_manualSolvingErrorOccurred();
				return true;
			}
			actions->slave_openGate(false);   // opengate()
			Logger::info("WP id: " + std::to_string(wp->id) + " kicked out");
		} else {
			actions->slave_openGate(true);    // closegate()
			data->wpManager->rotateNextWorkpieces();
			Logger::info("WP id: " + std::to_string(wp->id) + " passed to end");
		}
	}
	return true;
}

bool Running::slave_LBW_Unblocked() {
	return true;
}

bool Running::slave_LBE_Blocked() {
	if (!data->wpManager->isQueueempty(AreaType::AREA_D)) {
		Workpiece *wp = data->wpManager->getHeadOfArea(AreaType::AREA_D);
		if (!data->wpManager->isQueueempty(AreaType::AREA_D)) {
			if (wp->M_type != wp->S_type)
				wp->flipped = true;
			Logger::info(data->wpManager->to_string_Workpiece_FBM2(wp)); // print()
			actions->slave_sendMotorRightRequest(false);
		}
		// Close switch if open
		if (!data->slave_pusherMounted) {
			actions->slave_openGate(false);
		}
	}

	return true;
}

bool Running::slave_LBE_Unblocked() {
	Workpiece *wp = data->wpManager->removeFromArea(AreaType::AREA_D);
	if (wp == nullptr)
		return false;

	if (!data->wpManager->isFBM_MEmpty()) {
		// FBM1 waiting for FBM2 to get free -> start motor of FBM1 as well
		actions->master_sendMotorRightRequest(true);
	} else {
		// FBM1 not waiting -> stop FBM2
		actions->slave_sendMotorRightRequest(false);
	}

	return true;
}

bool Running::slave_LBR_Blocked() {
	setRampBlocked_S(true);

	Workpiece *wp = data->wpManager->removeFromArea(AreaType::AREA_D);
	if (wp == nullptr)
		return false;

	if (!data->wpManager->isFBM_MEmpty()) {
		// FBM1 waiting for FBM2 to get free -> start motor of FBM1 as well
		actions->master_sendMotorRightRequest(true);
		actions->slave_sendMotorRightRequest(false);
	} else {
		// FBM1 not waiting -> stop FBM2
		actions->slave_sendMotorRightRequest(false);
	}

	return true;
}

bool Running::slave_LBR_Unblocked() {
	setRampBlocked_S(false);
	return true;
}

bool Running::master_btnStop_Pressed() {
	bool warning = data->wpManager->getRamp_one() || data->wpManager->getRamp_two();
	if (warning) {
		Logger::warn("You cannot go to Standby mode, because a warning is active. Fix the warning first!");
		return false;
	}
	exit();
	new (this) Standby;
	entry();
	return true;
}

bool Running::slave_btnStop_Pressed() {
	bool warning = data->wpManager->getRamp_one() || data->wpManager->getRamp_two();
	if (warning) {
		Logger::warn("You cannot go to Standby mode, because a warning is active. Fix the warning first!");
		return false;
	}
	exit();
	new (this) Standby;
	entry();
	return true;
}

bool Running::master_btnReset_PressedLong() {
	data->wpManager->revertNextWorkpiece();
}

bool Running::slave_btnReset_PressedLong() {
	data->wpManager->revertNextWorkpiece();
}

bool Running::master_EStop_Pressed() {
	exit();
	new (this) EStop;
	entry();
	return true;
}

bool Running::slave_EStop_Pressed() {
	exit();
	new (this) EStop;
	entry();
	return true;
}

bool Running::selfSolvableErrorOccurred() {
	exit();
	new (this) Error;
	entry();
	selfSolvableErrorOccurred();
	return true;
}

bool Running::nonSelfSolvableErrorOccurred() {
	exit();
	new (this) Error;
	entry();
	nonSelfSolvableErrorOccurred();
	return true;
}

void Running::setRampBlocked_M(bool blocked) {
	data->setRampFBM1Blocked(blocked);

	if (blocked) {
		// If still blocked after 1s -> display warning
		std::thread t([=]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			if (data->isRampFBM1Blocked()) {
				actions->master_warningOn();
				actions->master_q2LedOn();
			}
		});
		t.detach();
	} else {
		actions->master_warningOff();
		actions->master_q2LedOff();
	}
}

void Running::setRampBlocked_S(bool blocked) {
	data->setRampFBM2Blocked(blocked);

	if (blocked) {
		// If still blocked after 1s -> display warning
		std::thread t([=]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			if (data->isRampFBM2Blocked()) {
				actions->slave_warningOn();
				actions->slave_q1LedOn();
			}
		});
		t.detach();
	} else {
		actions->slave_warningOff();
		actions->slave_q1LedOff();
	}
}
