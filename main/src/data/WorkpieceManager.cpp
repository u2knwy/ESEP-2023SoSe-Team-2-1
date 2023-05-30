/*
 * WorkpieceManager.cpp
 *
 *  Created on: 24.05.2023
 *      Author: Maik
 */

#include <data/WorkpieceManager.h>
#include "configuration/Configuration.h"

WorkpieceManager::WorkpieceManager() {
	nextId = 1;
	workpieceOnFBM2 = nullptr;
	for(const auto& wp : Configuration::getInstance().getDesiredOrder()) {
		desiredOrder.push_back(wp);
	}
}

WorkpieceManager::~WorkpieceManager() {
	for(Workpiece* wp : workpiecesOnFBM1) {
		delete wp;
	}
	workpiecesOnFBM1.clear();
	delete workpieceOnFBM2;
}

void WorkpieceManager::rotateWorkpieceOrder() {
	WorkpieceType first = desiredOrder.front();
	desiredOrder.pop_front();
	desiredOrder.push_back(first);
}

Workpiece* WorkpieceManager::addWorkpiece() {
	Workpiece* wp = new Workpiece(nextId);
	this->workpiecesOnFBM1.push_back(wp);
	nextId++;
	return wp;
}

int WorkpieceManager::nWorkpiecesOnFBM1() {
	return this->workpiecesOnFBM1.size();
}

bool WorkpieceManager::heightResultFBM1Received(WorkpieceType result, float avgHeight) {
	if (workpiecesOnFBM1.empty())
		return false;

	// Get first WP waiting for result from HeightSensor
	for (auto it = workpiecesOnFBM1.begin(); it != workpiecesOnFBM1.end(); ++it) {
		Workpiece* wp = *it;
		if(wp->getStatus() == WorkpieceStatus::FBM1_START_HM) {
			wp->setType(result);
			wp->setAvgHeightFBM1(avgHeight);
			wp->setStatus(WorkpieceStatus::FBM1_HM_MD);
			return true;
		}
	}

	return false;
}

bool WorkpieceManager::metalDetectedFBM1(bool isMetal) {
	if (workpiecesOnFBM1.empty())
		return false;

	// Get first WP waiting for result from MetalSensor
	for (auto it = workpiecesOnFBM1.begin(); it != workpiecesOnFBM1.end(); ++it) {
		Workpiece* wp = *it;
		if(wp->getStatus() == WorkpieceStatus::FBM1_HM_MD && wp->getType() == WorkpieceType::WS_BOM) {
			wp->setType(WorkpieceType::WS_BUM);
			wp->setStatus(WorkpieceStatus::FBM1_MD_SW);
			return true;
		}
	}
}

bool WorkpieceManager::nextWorkpieceAtFBM1Valid() {
	if (workpiecesOnFBM1.empty())
		return true;

	// Get first WP next arriving at switch
	for (auto it = workpiecesOnFBM1.begin(); it != workpiecesOnFBM1.end(); ++it) {
		Workpiece* wp = *it;
		WorkpieceStatus state = wp->getStatus();
		// If no metal was detected, it would be still "waiting for HeightResult"
		if(state == WorkpieceStatus::FBM1_MD_SW || state == WorkpieceStatus::FBM1_HM_MD) {
			if(wp->getType() == desiredOrder.front()) {
				return true;
			}
			return false;
		}
	}

	return false;
}

bool WorkpieceManager::nextWorkpieceAtFBM1SetSortOut() {
	if (workpiecesOnFBM1.empty())
		return false;

	for (auto it = workpiecesOnFBM1.begin(); it != workpiecesOnFBM1.end(); ++it) {
		Workpiece* wp = *it;
		WorkpieceStatus state = wp->getStatus();
		if(state == WorkpieceStatus::FBM1_MD_SW || state == WorkpieceStatus::FBM1_HM_MD) {
			wp->setStatus(WorkpieceStatus::FBM1_SW_RAMP);
			return true;
		}
	}
	return false;
}

bool WorkpieceManager::workpieceAtFBM1SortedOut() {
	if (workpiecesOnFBM1.empty())
		return false;

	// Get first WP waiting for sorted out at ramp
	for (auto it = workpiecesOnFBM1.begin(); it != workpiecesOnFBM1.end(); ++it) {
		Workpiece* wp = *it;
		if(wp->getStatus() == WorkpieceStatus::FBM1_SW_RAMP) {
			workpiecesOnFBM1.erase(it);
			return true;
		}
	}

	return false;
}

bool WorkpieceManager::isFBM2Occupied() {
	return this->workpieceOnFBM2 != nullptr;
}

bool WorkpieceManager::workpieceAtFBM2SortedOut() {
	if (this->workpieceOnFBM2 == nullptr) {
		return false;
	} else {
		this->workpieceOnFBM2 = nullptr;
		return true;
	}
}

bool WorkpieceManager::transferWorkpieceToFBM2() {
	if (this->workpieceOnFBM2 != nullptr) {
		return false;
	} else {
		Workpiece* wp = workpiecesOnFBM1.front();
		workpiecesOnFBM1.erase(workpiecesOnFBM1.begin());
		workpieceOnFBM2 = wp;
		return true;
	}
}
